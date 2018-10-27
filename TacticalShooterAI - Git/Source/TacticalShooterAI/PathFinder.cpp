#include "PathFinder.h"
#include <queue>
#include <functional>

using namespace std;

PathFinder::PathFinder()
	:coverGenRef(nullptr)
{
}

PathFinder::~PathFinder()
{
}

std::vector<FVector> PathFinder::BuildPath(FVector actorLocation, FVector targetLocation)
{
	vector<FVector> path;

	if (coverGenRef != nullptr)
	{
		FVector midPoint = coverGenRef->basePosition;
		float oneSide = coverGenRef->GridNumber / 2;
		float ObjectWidth = coverGenRef->ObjectWidth;
		midPoint.X -= ((oneSide * ObjectWidth) + ObjectWidth / 2);
		midPoint.Y -= ((oneSide * ObjectWidth) + ObjectWidth / 2);

		//calculate start and end grid point index
		int startY = (actorLocation.X - midPoint.X) / ObjectWidth;
		int startX = (actorLocation.Y - midPoint.Y) / ObjectWidth;

		int endY = (targetLocation.X - midPoint.X) / ObjectWidth;
		int endX = (targetLocation.Y - midPoint.Y) / ObjectWidth;

		//build node path maps
		map<GridPoints*, GridPoints*> cameFrom;
		map<GridPoints*, float> costSoFar;

		//search grid
		AStarSearch(coverGenRef->Grid[startX][startY], coverGenRef->Grid[endX][endY], cameFrom, costSoFar);

		GridPoints* current = coverGenRef->Grid[endX][endY];
		while (current != coverGenRef->Grid[startX][startY])
		{
			path.push_back(current->Location);
			current = cameFrom[current];
		}

		path.push_back(coverGenRef->Grid[startX][startY]->Location);

		//arrange in correct order
		//reverse(path.begin(), path.end());
	}
	return path;
}

void PathFinder::SetCoverGeneratorReference(ACoverGridGenerator * ref)
{
	if(ref != nullptr)
		coverGenRef = ref;
}

void PathFinder::AStarSearch(GridPoints* start, GridPoints* goal, map<GridPoints*, GridPoints*>& cameFrom, map<GridPoints*, float>& costSoFar)
{
	priority_queue<NavPath, vector<NavPath>, greater<NavPath>> frontier;

	frontier.emplace(0.0f, start);

	cameFrom[start] = start;
	costSoFar[start] = 0.0f;

	while (!frontier.empty())
	{
		GridPoints* current = frontier.top().second;
		frontier.pop();

		if (current == goal) 
		{
			break;
		}

		for (GridPoints* next : current->Neibhour)
		{
			if (next != nullptr && next->NoCollition)
			{
				float newCost = costSoFar[current] + 1;			//Assumed cost to travel to next node is always constant 1
				if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next])
				{
					costSoFar[next] = newCost;
					float priority = newCost + Heuristic(next->Location, goal->Location);
					cameFrom[next] = current;
					frontier.emplace(priority, next);
				}
			}
		}
	}
}

float PathFinder::Heuristic(FVector currentLocation, FVector nextLocation)
{
	return (currentLocation * FVector(1, 1, 0) - nextLocation * FVector(1, 1, 0)).Size();
}
