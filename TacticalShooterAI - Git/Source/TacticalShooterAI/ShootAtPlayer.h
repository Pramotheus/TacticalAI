#pragma once
#include "GOAPAction.h"

class ShootAtPlayer : public GOAPAction
{
public:
	ShootAtPlayer();
	~ShootAtPlayer();

private:
	bool ShotPlayer;
	const float fireTime{0.5f};
	float fireDelay;

public:
	virtual void Reset() override;
	virtual bool IsDone() override;
	virtual bool checkProceduralPrecondition(AActor* agent) override;
	virtual bool perform(AActor* agent) override;

private:
	bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, AActor* agent);
};

