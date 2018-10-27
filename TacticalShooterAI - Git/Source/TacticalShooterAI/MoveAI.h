#pragma once
#include "GOAPAction.h"

class MoveAI : public GOAPAction
{
public:
	MoveAI();
	~MoveAI();

private:
	bool nearPlayer;
	const float speed {200};

public:
	virtual void Reset() override;
	virtual bool IsDone() override;
	virtual bool checkProceduralPrecondition(AActor* agent) override;
	virtual bool perform(AActor* agent) override;
};

