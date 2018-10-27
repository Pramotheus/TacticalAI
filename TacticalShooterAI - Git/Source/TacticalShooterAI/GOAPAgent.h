// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IGoap.h"
#include "GOAPPlanner.h"
#include "GOAPAgent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALSHOOTERAI_API UGOAPAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOAPAgent();
	~UGOAPAgent();

	//Add actions into list of current actions
	void AddAction(GOAPAction * action);

	//Remove action from list of current actions
	void RemoveAction(GOAPAction * action);

	//Check for plan validity
	bool HasActionPlan();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//List of all avilable actions
	TArray<GOAPAction*> availableActions;
	
	//List of current actions to execute
	std::queue<GOAPAction*> currentActions;

	//Planner Reference
	GOAPPlanner planner;

	//Reference to AI's goap base fucntions
	IIGoap* actorBase;

	//Helper function for getting plan
	void GetPlan();
};
