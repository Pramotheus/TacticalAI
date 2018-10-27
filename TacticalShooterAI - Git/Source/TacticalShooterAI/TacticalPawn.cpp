// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalPawn.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MoveAI.h"
#include "MoveAIToLocation.h"
#include "FindCover.h"
#include "ShootAtPlayer.h"

ATacticalPawn::ATacticalPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	GoapAgent = CreateDefaultSubobject<UGOAPAgent>(TEXT("GoapAgent"));
	
	//MoveAI * moveAction = new MoveAI();
	//GoapAgent->AddAction(moveAction);

	FindCover * findCover = new FindCover();
	GoapAgent->AddAction(findCover);

	MoveAIToLocation * moveToCover = new MoveAIToLocation();
	GoapAgent->AddAction(moveToCover);

	ShootAtPlayer * shootPlayer = new ShootAtPlayer();
	GoapAgent->AddAction(shootPlayer);
}

// Called when the game starts or when spawned
void ATacticalPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), foundActors);
	
	for(auto actor : foundActors)
	{
		allCoverPoints.Add(Cast<ACoverPoint>(actor));
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverGridGenerator::StaticClass(), foundActors);

	for (auto actor : foundActors)
	{
		coverGenerator = Cast<ACoverGridGenerator>(actor);
	}
}

// Called every frame
void ATacticalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATacticalPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TMap<FString, bool> ATacticalPawn::GetWorldState()
{
	TMap<FString, bool> state;

	state.Add("Kill Enemy", (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == nullptr));
	
	if (currentCover)
	{
		//state.Add("No Cover", false);
		state.Add("Has Cover", currentCover->Active);
		state.Add("In Cover", currentCover->Active && ((currentCover->GetActorLocation() - GetActorLocation()).Size() < 20));
	}
	else
	{
		//state.Add("No Cover", true);
		state.Add("Has Cover", false);
		state.Add("In Cover", false);
	}

	return state;
}

TMap<FString, bool> ATacticalPawn::CreateGoalState()
{
	TMap<FString, bool> goal;
	goal.Add("Kill Enemy", true);
	//goal.Add("In Cover", true);
	return goal;
}

void ATacticalPawn::PlanFailed(TMap<FString, bool> failedGoal)
{
	LastPlanUpdate = TEXT("Plan Failed");
}

void ATacticalPawn::PlanFound(TMap<FString, bool> goal, std::queue<GOAPAction*> actions)
{
	LastPlanUpdate = TEXT("Plan Found");
}

void ATacticalPawn::ActionsFinished()
{
	LastPlanUpdate = TEXT("Plan Finished");
}

void ATacticalPawn::PlanAborted(GOAPAction * aborter)
{
	LastPlanUpdate = TEXT("Plan Aborted");
}

ACoverGridGenerator * ATacticalPawn::GetCoverGenerator()
{
	return coverGenerator;
}

