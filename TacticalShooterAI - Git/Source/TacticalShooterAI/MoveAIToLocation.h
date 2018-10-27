#pragma once
#include "GOAPAction.h"
#include "PathFinder.h"

class MoveAIToLocation : public GOAPAction
{
public:
	MoveAIToLocation();
	~MoveAIToLocation();

private:
	bool atCover;
	bool pathSet;
	PathFinder navigator;
	std::vector<FVector> path;

	const float speed{ 800 };

public:
	virtual void Reset() override;
	virtual bool IsDone() override;
	virtual bool checkProceduralPrecondition(AActor* agent) override;
	virtual bool perform(AActor* agent) override;
};

