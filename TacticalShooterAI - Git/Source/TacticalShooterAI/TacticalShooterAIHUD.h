// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TacticalShooterAIHUD.generated.h"

UCLASS()
class ATacticalShooterAIHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATacticalShooterAIHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

