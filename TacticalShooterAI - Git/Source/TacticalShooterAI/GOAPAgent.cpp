// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAPAgent.h"
#include "TacticalPawn.h"

using namespace std;

// Sets default values for this component's properties
UGOAPAgent::UGOAPAgent()
	:currentActions(), availableActions(), planner()
{
	PrimaryComponentTick.bCanEverTick = true;

	availableActions.Reserve(10);
}


void UGOAPAgent::AddAction(GOAPAction * action)
{
	if(action != nullptr)
		availableActions.Add(action);
}

void UGOAPAgent::RemoveAction(GOAPAction * action)
{
	if (action != nullptr)
	{
		availableActions.Remove(action);
	}
}

bool UGOAPAgent::HasActionPlan()
{
	return !(currentActions.empty());
}

void UGOAPAgent::BeginPlay()
{
	Super::BeginPlay();
	
	actorBase = Cast<IIGoap>(GetOwner());
}


// Called every frame
void UGOAPAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasActionPlan())
	{
		//actorBase->ActionsFinished();
		GetPlan();

	}
	else
	{
		GOAPAction* action = currentActions.front();
		if (action == nullptr)
		{
			currentActions.pop();
		}

		action = currentActions.front();
		if (action->IsDone())
		{
			currentActions.pop();
		}

		if (HasActionPlan()) 
		{
			// perform the next action
			action = currentActions.front();

			//Debug
			Cast<ATacticalPawn>(GetOwner())->CurrentAction = action->Name;

			if (!action->perform(GetOwner()))
			{
				// action failed, we need to plan again
				actorBase->PlanAborted(action);

				while(!currentActions.empty())
					currentActions.pop();
			}
		}
		else 
		{
			// no actions left, move to Plan state
			actorBase->ActionsFinished();
		}
	}
}

void UGOAPAgent::GetPlan()
{
	TMap<FString, bool> worldState = actorBase->GetWorldState();
	TMap<FString, bool> goal = actorBase->CreateGoalState();

	queue<GOAPAction*> plan = planner.Plan(GetOwner(), availableActions, worldState, goal);
	if (!(plan.empty()))
	{
		// we have a plan
		currentActions = plan;
		actorBase->PlanFound(goal, plan);
	}
	else
	{
		actorBase->PlanFailed(goal);
	}
}

UGOAPAgent::~UGOAPAgent()
{
	for (auto value : availableActions)
	{
		RemoveAction(value);
	}
}