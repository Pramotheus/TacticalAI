#include "MoveAIToLocation.h"
#include "TacticalPawn.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

MoveAIToLocation::MoveAIToLocation()
	:atCover(false), pathSet(false), path(), navigator()
{
	Name = TEXT("Move to Coverpoint");

	//AddPrecondition("No Cover", false);
	AddPrecondition("Has Cover", true);
	AddEffect("In Cover", true);
}


MoveAIToLocation::~MoveAIToLocation()
{
}

void MoveAIToLocation::Reset()
{
	atCover = false;
	pathSet = false;
	path.clear();
}

bool MoveAIToLocation::IsDone()
{
	return atCover;
}

bool MoveAIToLocation::checkProceduralPrecondition(AActor * agent)
{
	/*ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);

	if (!AiCharecter->currentCover)
	{
		if (!AiCharecter->FindCoverPoint())
		{
			return false;
		}
	}*/

	return true;
}

bool MoveAIToLocation::perform(AActor * agent)
{
	ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);
	target = AiCharecter->currentCover;

	if (Cast<ACoverPoint>(target)->Active)
	{
		float deltaTime = agent->GetWorld()->DeltaTimeSeconds;

		if ((AiCharecter != nullptr) && (target != nullptr))
		{
			if (path.empty())
			{
				if (pathSet)
				{
					atCover = true;
				}
				else
				{
					navigator.SetCoverGeneratorReference(AiCharecter->GetCoverGenerator());
					path = navigator.BuildPath(AiCharecter->GetActorLocation(), target->GetActorLocation());
					pathSet = true;
				}
			}

			/*FVector vectorToDestination = target->GetActorLocation() - AiCharecter->GetActorLocation();
			if (vectorToDestination.Size() > 20)
			{
				AiCharecter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(AiCharecter->GetActorLocation()* FVector(1, 1, 0), target->GetActorLocation()*FVector(1, 1, 0)));
				AiCharecter->SetActorLocation((AiCharecter->GetActorLocation() + (UKismetMathLibrary::Normal(vectorToDestination) * speed * deltaTime)));
			}
			else
			{
				atCover = true;
			}*/

			FVector vectorToDestination = path.back() - AiCharecter->GetActorLocation();
			if (vectorToDestination.Size() > 20)
			{
				AiCharecter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(AiCharecter->GetActorLocation()* FVector(1, 1, 0), path.back()*FVector(1, 1, 0)));
				AiCharecter->SetActorLocation((AiCharecter->GetActorLocation() + (UKismetMathLibrary::Normal(vectorToDestination) * speed * deltaTime)));
			}
			else
			{
				path.pop_back();
			}
			return true;
		}
	}
	return false;
}
