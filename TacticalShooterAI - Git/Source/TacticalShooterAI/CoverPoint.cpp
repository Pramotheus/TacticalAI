// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverPoint.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "TacticalShooterAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ACoverPoint::ACoverPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CoverRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent0"));

	if (CoverRootComponent)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

		if (Mesh)
		{
			Mesh->SetupAttachment(CoverRootComponent);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			//Mesh->bHiddenInGame = true;

			ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
			if (SphereMeshAsset.Succeeded()) 
			{
				Mesh->SetStaticMesh(SphereMeshAsset.Object);
				Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
				Mesh->SetWorldScale3D(FVector(0.3f));
			}

			ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterialActive(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP_Mat.TutorialTPP_Mat"));
			if (SphereMaterialActive.Succeeded())
			{
				ActiveMaterial = SphereMaterialActive.Object;
			}

			ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterialInactive(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/TutorialMaterial.TutorialMaterial"));
			if (SphereMaterialInactive.Succeeded())
			{
				InactiveMaterial = SphereMaterialInactive.Object;
			}
		}
	}
}

// Called when the game starts or when spawned
void ACoverPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoverPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);

	if (DoTrace(&RV_Hit, &RV_TraceParams))
	{
		Active = false;
		Mesh->SetMaterial(0, InactiveMaterial);
	}
	else
	{
		Active = true;
		Mesh->SetMaterial(0, ActiveMaterial);
	}
}

void ACoverPoint::SetHeightScale(int value)
{
	HeightScale = value;
}

bool ACoverPoint::DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams)
{
	FVector Start = GetActorLocation();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector End = player->GetActorLocation();

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.0f, 0, 0.5f);

	bool DidTrace = GetWorld()->LineTraceSingleByChannel(*RV_Hit, Start, End, ECC_Pawn, *RV_TraceParams);

	ATacticalShooterAICharacter* hitactor = Cast<ATacticalShooterAICharacter>(RV_Hit->GetActor());
	
	if (hitactor != nullptr)
	{
		return true;
	}
	
	return false;
}