// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "GOAPAgent.h"
#include "CoverPoint.h"
#include "CoverGridGenerator.h"
#include "TacticalShooterAIProjectile.h"
#include "TacticalPawn.generated.h"

UCLASS()
class TACTICALSHOOTERAI_API ATacticalPawn : public APawn , public IIGoap
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATacticalPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "GOAP", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UGOAPAgent* GoapAgent;

	UPROPERTY(Category = "Movement", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UMovementComponent* MovementComponent;

	UPROPERTY(Category = "Cover", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ACoverPoint* currentCover;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ATacticalShooterAIProjectile> ProjectileClass;

	TArray<ACoverPoint*> allCoverPoints;

	UPROPERTY(Category = "Debug", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString LastPlanUpdate;

	UPROPERTY(Category = "Debug", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString CurrentPlan;

	UPROPERTY(Category = "Debug", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString CurrentAction;

	//Implement IGoap function overrides
	virtual TMap<FString, bool> GetWorldState() override;
	virtual TMap<FString, bool> CreateGoalState() override;
	virtual void PlanFailed(TMap<FString, bool> failedGoal) override;
	virtual void PlanFound(TMap<FString, bool> goal, std::queue<GOAPAction*> actions) override;
	virtual void ActionsFinished() override;
	virtual void PlanAborted(GOAPAction *aborter) override;

	ACoverGridGenerator * GetCoverGenerator();

private:
	ACoverGridGenerator * coverGenerator;
};
