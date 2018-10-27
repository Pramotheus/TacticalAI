#pragma once
#include <vector>
#include <map>
#include "CoverGridGenerator.h"

typedef std::pair<float, GridPoints*> NavPath;

class PathFinder
{
public:
	PathFinder();
	~PathFinder();
	
	std::vector<FVector> BuildPath(FVector actorLocation, FVector targetLocation);

	void SetCoverGeneratorReference(ACoverGridGenerator * ref);

private:
	void AStarSearch(GridPoints* start, GridPoints* goal, std::map<GridPoints*, GridPoints*>& cameFrom,
		std::map<GridPoints*, float>& costSoFar);

	float Heuristic(FVector currentLocation, FVector nextLocation);

	ACoverGridGenerator* coverGenRef;
};

