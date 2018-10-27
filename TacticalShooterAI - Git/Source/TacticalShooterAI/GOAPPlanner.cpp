#include "GOAPPlanner.h"
#include "TacticalPawn.h"

using namespace std;

GOAPPlanner::Node::Node(Node * parentRef, float runningCostRef, TMap<FString, bool> stateRef, GOAPAction * actionRef)
	:parent(parentRef), runningCost(runningCostRef), state(stateRef), action(actionRef)
{
}

GOAPPlanner::GOAPPlanner()
{
}


GOAPPlanner::~GOAPPlanner()
{
}

std::queue<GOAPAction*> GOAPPlanner::Plan(AActor * agent, TArray<GOAPAction*> availableActions, TMap<FString, bool> worldState, TMap<FString, bool> goal)
{
	//Reset all actions to normal state
	for (auto action : availableActions)
	{
		action->DoReset();
	}

	TArray<GOAPAction*> usableActions;
	for(GOAPAction* action : availableActions) {
		if (action->checkProceduralPrecondition(agent))
			usableActions.Add(action);
	}

	//build a graph with the available actions
	list<Node*> leaves;
	Node* start = new Node(nullptr, 0, worldState, nullptr);

	if (!BuildGraph(start, leaves, usableActions, goal))
	{
		//if fails return empty queue
		return queue<GOAPAction*>();
	}

	//find cheapest path
	Node* cheapest = nullptr;
	for (Node* & leaf : leaves)
	{
		if (cheapest == nullptr)
			cheapest = leaf;
		else
		{
			if (leaf->runningCost < cheapest->runningCost)
				cheapest = leaf;
		}
	}

	//find all actions for the cheapest goal action
	list<GOAPAction*> correctOrderList;
	while (cheapest != nullptr) 
	{
		if (cheapest->action != nullptr)
		{
			correctOrderList.push_front(cheapest->action); // insert the action in the front
		}
		cheapest = cheapest->parent;
	}

	//Debug print stuff
	FString plan;

	// push the correct order of actions into a queue to send it out
	queue<GOAPAction*> result;
	for(GOAPAction* val : correctOrderList)
	{
		//Debug
		plan += val->Name;
		plan += "-> ";
		//Debug

		result.push(val);
	}

	//Debug
	Cast<ATacticalPawn>(agent)->CurrentPlan = plan;

	for (Node* & leaf : leaves)
	{
		delete leaf;
	}

	return result;
}

bool GOAPPlanner::BuildGraph(Node* parent, std::list<Node*> & leaves, TArray<GOAPAction*> & usableActions, TMap<FString, bool> & goal)
{
	bool foundPath = false;

	for (GOAPAction* action : usableActions)
	{
		//check for precondition match with start node
		if (InState(action->GetPreconditions(), parent->state))
		{
			//if match found add effect to world state
			TMap<FString, bool> currentState = PopulateState(parent->state, action->GetEffects());

			Node *node = new Node(parent, parent->runningCost + action->GetCost(), currentState, action);

			if (InState(goal, currentState))
			{
				// Found a solution
				leaves.push_back(node);
				foundPath = true;
			}
			else 
			{
				// not at a solution yet, so test all the remaining actions and branch out the tree
				TArray<GOAPAction*> subset = ActionSubset(usableActions, action);

				if (BuildGraph(node, leaves, subset, goal))
				{
					foundPath = true;
				}
			}
		}
	}

	return foundPath;
}

bool GOAPPlanner::InState(TMap<FString, bool> test, TMap<FString, bool> state)
{
	bool match = true;

	for(TPair<FString, bool> condition : test)
	{
		bool currentMatch = false;
		for(TPair<FString, bool> matchCondition : state)
		{
			if (matchCondition == condition)
			{
				currentMatch = true;
				break;
			}
		}
		if (!currentMatch)
			match = false;
	}
	return match;
}

TMap<FString, bool> GOAPPlanner::PopulateState(TMap<FString, bool> currentState, TMap<FString, bool> stateChange)
{
	TMap<FString, bool> state = currentState;

	for(TPair<FString, bool> change : stateChange)
	{
		// if the key exists in the current state, update the Value
		bool exists = false;

		if (state.Contains(change.Key))
		{
			state.Remove(change.Key);
		}

		state.Add(change.Key, change.Value);
	}
	return state;
}

TArray<GOAPAction*> GOAPPlanner::ActionSubset(TArray<GOAPAction*> actions, GOAPAction * removeMe)
{
	TArray<GOAPAction*> subset(actions);
	subset.Remove(removeMe);
	return subset;
}

