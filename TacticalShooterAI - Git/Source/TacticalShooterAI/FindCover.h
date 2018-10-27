#pragma once
#include "GOAPAction.h"

class FindCover : public GOAPAction
{
public:
	FindCover();
	~FindCover();

private:
	bool hasCover;

public:
	virtual void Reset() override;
	virtual bool IsDone() override;
	virtual bool checkProceduralPrecondition(AActor* agent) override;
	virtual bool perform(AActor* agent) override;
};

