// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "CoverGridGenerator.generated.h"

struct GridPoints
{
	FVector Location;
	TArray<GridPoints*> Neibhour;
	int HeightScale;
	bool NoCollition;
	bool CanSpawnCoverPoint;
	GridPoints* Path;

	GridPoints() :Location(), NoCollition(false), HeightScale(0), CanSpawnCoverPoint(false), Path(nullptr)
	{
		for(int i=0;i<4;++i)
			Neibhour.Add(nullptr);
	}
};

UCLASS()
class TACTICALSHOOTERAI_API ACoverGridGenerator : public AActor
{
	GENERATED_BODY()

	
public:	
	ACoverGridGenerator();
	virtual ~ACoverGridGenerator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cover Grid Generator")
		void GenerateGrid();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cover Grid Generator")
		void SpawnPoints();

	UPROPERTY(EditAnywhere)
		float ObjectWidth;

	UPROPERTY(EditAnywhere)
		int GridNumber;

	UPROPERTY(EditAnywhere)
		float CrouchHeight;

	UPROPERTY(EditAnywhere)
		float StandHeight;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACoverPoint> coverPoint;

private:
	void ClearAndInitializeGrid();
	void CheckCollition(GridPoints* gridPoint);
	void CheckSpawnPoints();
	
public:
	std::vector<std::vector<GridPoints*>> Grid;
	FVector basePosition;
};
