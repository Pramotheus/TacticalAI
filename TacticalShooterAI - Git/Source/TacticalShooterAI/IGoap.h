// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <queue>
#include "UObject/Interface.h"
#include "GOAPAction.h"
#include "IGoap.generated.h"

UINTERFACE(MinimalAPI)
class UIGoap : public UInterface
{
	GENERATED_BODY()
};

class TACTICALSHOOTERAI_API IIGoap
{
	GENERATED_BODY()

public:
	
	/**
	* The starting state of the Agent and the world.
	* Supply what states are needed for actions to run.
	*/
	virtual TMap<FString, bool> GetWorldState() = 0;

	/**
	* Give the planner a new goal so it can figure out
	* the actions needed to fulfill it.
	*/
	virtual TMap<FString, bool> CreateGoalState() = 0;

	/**
	* No sequence of actions could be found for the supplied goal.
	* You will need to try another goal
	*/
	virtual void PlanFailed(TMap<FString, bool> failedGoal) = 0;

	/**
	* A plan was found for the supplied goal.
	* These are the actions the Agent will perform, in order.
	*/
	virtual void PlanFound(TMap<FString, bool> goal, std::queue<GOAPAction*> actions) = 0;

	/**
	* All actions are complete and the goal was reached.
	*/
	virtual void ActionsFinished() = 0;

	/**
	* One of the actions caused the plan to abort.
	* That action is returned.
	*/
	virtual void PlanAborted(GOAPAction *aborter) = 0;
	
};
