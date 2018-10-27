// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverGridGenerator.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "CoverPoint.h"

using namespace std;

ACoverGridGenerator::ACoverGridGenerator()
	:ObjectWidth(10), GridNumber(1), CrouchHeight(120.0f), StandHeight(250)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACoverGridGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid();
}

void ACoverGridGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoverGridGenerator::GenerateGrid()
{
	//Reinitialize Grid
	ClearAndInitializeGrid();

	//Calculate position of first grid object
	basePosition = GetActorLocation();
	FVector currentPosition = basePosition;
	float oneSide = GridNumber / 2;
	float startX;
	currentPosition.X -= (oneSide * ObjectWidth);
	currentPosition.Y -= (oneSide * ObjectWidth);
	startX = currentPosition.X;

	//Itterate through all grid elements
	for (int i = 0; i < GridNumber; ++i)
	{
		for (int j = 0; j < GridNumber; ++j)
		{
			//Set grid point location
			Grid[i][j]->Location = currentPosition;

			//Set neibhours to each grid point
			if (j != 0)
			{
				Grid[i][j]->Neibhour[0] = Grid[i][j - 1];
				Grid[i][j - 1]->Neibhour[1] = Grid[i][j];
			}
			if (i != 0)
			{
				Grid[i][j]->Neibhour[2] = Grid[i - 1][j];
				Grid[i - 1][j]->Neibhour[3] = Grid[i][j];
			}

			CheckCollition(Grid[i][j]);

			currentPosition.X += ObjectWidth;
		}
		currentPosition.X = startX;
		currentPosition.Y += ObjectWidth;
	}

	//Check for collition
	CheckSpawnPoints();
}

void ACoverGridGenerator::SpawnPoints()
{
	if (coverPoint != nullptr)
	{
		UWorld* world = GetWorld();
		
		if (world)
		{
			for (auto & gridRow : Grid)
			{
				for (auto & element : gridRow)
				{
					if (element->CanSpawnCoverPoint)
					{
						FActorSpawnParameters spawnParameters;
						FRotator rotator;
						ACoverPoint *cover = world->SpawnActor<ACoverPoint>(element->Location, rotator, spawnParameters);
						if (cover != nullptr)
						{
							cover->SetHeightScale(element->HeightScale);
						}
					}
				}
			}
		}
	}
}

void ACoverGridGenerator::ClearAndInitializeGrid()
{
	for (auto & gridRow : Grid)
	{
		for (auto & element : gridRow)
		{
			delete element;
		}

		gridRow.clear();
		gridRow.shrink_to_fit();
	}

	Grid.clear();
	Grid.shrink_to_fit();

	for (int i = 0; i < GridNumber; ++i)
	{
		Grid.push_back(vector<GridPoints*>());

		for (int j = 0; j < GridNumber; ++j)
		{
			Grid[i].push_back(new GridPoints());
		}
	}
}

void ACoverGridGenerator::CheckCollition(GridPoints* gridPoint)
{
	//Initialize Values to use in collition detection
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Push(EObjectTypeQuery::ObjectTypeQuery1);
	TSubclassOf<AActor> allActors;
	UClass* ActorClassFilter = allActors;
	TArray<AActor*> ActorsToIgnore, output;

	FVector currentPosition = gridPoint->Location;
	
	//Check for collition at the grid point
	if (!(UKismetSystemLibrary::BoxOverlapActors(GetWorld(), currentPosition, FVector(ObjectWidth / 2, ObjectWidth / 2, 1.0f), ObjectTypes, ActorClassFilter, ActorsToIgnore, output)))
	{
		gridPoint->NoCollition = true;

		//Debug draw squares
		DrawDebugLine(GetWorld(), currentPosition + FVector(-ObjectWidth / 2, -ObjectWidth / 2, 0), currentPosition + FVector(-ObjectWidth / 2, ObjectWidth / 2, 0), FColor(255, 0, 0), false, 20, 0, 5.0);
		DrawDebugLine(GetWorld(), currentPosition + FVector(-ObjectWidth / 2, -ObjectWidth / 2, 0), currentPosition + FVector(ObjectWidth / 2, -ObjectWidth / 2, 0), FColor(255, 0, 0), false, 20, 0, 5.0);
		DrawDebugLine(GetWorld(), currentPosition + FVector(ObjectWidth / 2, ObjectWidth / 2, 0), currentPosition + FVector(ObjectWidth / 2, -ObjectWidth / 2, 0), FColor(255, 0, 0), false, 20, 0, 5.0);
		DrawDebugLine(GetWorld(), currentPosition + FVector(ObjectWidth / 2, ObjectWidth / 2, 0), currentPosition + FVector(-ObjectWidth / 2, ObjectWidth / 2, 0), FColor(255, 0, 0), false, 20, 0, 5.0);
	}
	else if (!(UKismetSystemLibrary::BoxOverlapActors(GetWorld(), currentPosition + FVector(0, 0, CrouchHeight), FVector(ObjectWidth / 2, ObjectWidth / 2, 1.0f), ObjectTypes, ActorClassFilter, ActorsToIgnore, output)))
	{
		gridPoint->HeightScale = 1;

		//Debug draw Line
		DrawDebugLine(GetWorld(), currentPosition, currentPosition + FVector(0, 0, CrouchHeight), FColor(0, 0, 255), false, 20, 0, 5.0);
		DrawDebugPoint(GetWorld(), currentPosition + FVector(0, 0, CrouchHeight), 20, FColor(150, 0, 255), false, 20);
		
	}
	else if (!(UKismetSystemLibrary::BoxOverlapActors(GetWorld(), currentPosition + FVector(0, 0, StandHeight), FVector(ObjectWidth / 2, ObjectWidth / 2, 1.0f), ObjectTypes, ActorClassFilter, ActorsToIgnore, output)))
	{
		gridPoint->HeightScale = 2;
		//Debug draw Line
		DrawDebugLine(GetWorld(), currentPosition, currentPosition + FVector(0, 0, StandHeight), FColor(0, 0, 255), false, 20, 0, 5.0);
		DrawDebugPoint(GetWorld(), currentPosition + FVector(0, 0, StandHeight), 20, FColor(150, 0, 255), false, 20);
	}
	else
	{
		gridPoint->HeightScale = 3;
	}
}

void ACoverGridGenerator::CheckSpawnPoints()
{
	//Iterate through elements
	for (auto & gridRow : Grid)
	{
		for (auto & element : gridRow)
		{
			//Check if the element is ouside objects
			if (element->NoCollition)
			{
				//If outside objects go through its neibhours and see if they are inside objects
				for (auto & neibhour : element->Neibhour)
				{
					if (neibhour != nullptr)
					{
						if (neibhour->NoCollition == false)
						{
							//If neibhour inside objects, element can spawn cover points
							if (neibhour->HeightScale == 2)
							{
								element->CanSpawnCoverPoint = true;
								element->HeightScale = neibhour->HeightScale;
								DrawDebugPoint(GetWorld(), element->Location, 20, FColor(0, 0, 255), false, 20);
							}
							else if (neibhour->HeightScale == 3)
							{
								int insideBox = 0;
								for (auto & farNeibhour : neibhour->Neibhour)
								{
									if (farNeibhour != nullptr)
									{
										if (!farNeibhour->NoCollition)
										{
											++insideBox;
										}
									}
									else
									{
										++insideBox;
									}
								}
								if (insideBox <= 2)
								{
									element->CanSpawnCoverPoint = true;
									element->HeightScale = neibhour->HeightScale;
									DrawDebugPoint(GetWorld(), element->Location, 20, FColor(0, 255, 0), false, 20);
								}
								else
								{
									DrawDebugPoint(GetWorld(), element->Location, 20, FColor(255, 0, 0), false, 20);
								}
							}
							else if (neibhour->HeightScale == 1)
							{
								//If lower than crouch height do not spawn cover point
								DrawDebugPoint(GetWorld(), element->Location, 20, FColor(255, 0, 0), false, 20);
							}
							
						}
					}
				}
			}
		}
	}


}

ACoverGridGenerator::~ACoverGridGenerator()
{
	for (auto & gridRow : Grid)
	{
		for (auto & element : gridRow)
		{
			delete element;
		}

		gridRow.clear();
		gridRow.shrink_to_fit();
	}
}