#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

class GOAPAction
{

public:
	GOAPAction();
	virtual ~GOAPAction();

private:
	TMap<FString, bool> preconditions;
	TMap<FString, bool> effects;

	// The cost of performing the action. 
	float cost{ 1.0f };

public:
	// Target of the current action
	// Can be null
	AActor * target;

	//Name for action for debug
	FString Name;

	// Reset base class variables as well as derived class
	void DoReset();

	// Reset any variables that need to be reset before planning happens again.
	virtual void Reset() = 0;

	// Check for action complete
	virtual bool IsDone() = 0;

	// Procedurally check if this action can run.
	virtual bool checkProceduralPrecondition(AActor* agent) = 0;

	//Run the action.
	// Returns True if the action performed successfully or false
	virtual bool perform(AActor* agent) = 0;

	void AddPrecondition(FString key, bool value);
	void RemovePrecondition(FString key);

	void AddEffect(FString key, bool value);
	void RemoveEffect(FString key);

	TMap<FString, bool> GetPreconditions();
	TMap<FString, bool> GetEffects();
	float GetCost();
};

