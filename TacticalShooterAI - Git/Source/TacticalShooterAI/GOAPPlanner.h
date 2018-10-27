#pragma once
#include "IGoap.h"
#include <list>

class GOAPPlanner
{
private:
	//	Used for building up the graph and holding the running costs of actions.
	class Node 
	{
	public:
		Node* parent;
		float runningCost;
		TMap<FString, bool> state;
		GOAPAction* action;
		
		Node(Node* parentRef, float runningCostRef, TMap<FString, bool> stateRef, GOAPAction* actionRef);
	};

public:
	GOAPPlanner();
	~GOAPPlanner();

	// Plan what sequence of actions can fulfill the goal.
	// Returns empty queue if a plan could not be found
	std::queue<GOAPAction*> Plan(AActor* agent, TArray<GOAPAction*> availableActions, TMap<FString, bool> worldState, TMap<FString, bool> goal);

private:
	// Returns true if at least one solution was found.
	// The possible paths are stored in the leaves list.
	bool BuildGraph(Node* parent, std::list<Node*> & leaves, TArray<GOAPAction*> & usableActions, TMap<FString, bool> & goal);

	// Helper fucntion to check prescondions match between two node actions
	bool InState(TMap<FString, bool> test, TMap<FString, bool> state);

	// Helper fucnction to apply the stateChange to the currentState
	TMap<FString, bool> PopulateState(TMap<FString, bool> currentState, TMap<FString, bool> stateChange);

	// Create a subset of the actions excluding the removeMe one. Creates a new set.
	TArray<GOAPAction*> ActionSubset(TArray<GOAPAction*> actions, GOAPAction* removeMe);
};

