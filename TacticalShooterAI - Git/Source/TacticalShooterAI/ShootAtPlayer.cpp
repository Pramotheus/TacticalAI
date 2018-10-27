#include "ShootAtPlayer.h"
#include "TacticalPawn.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "TacticalShooterAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ShootAtPlayer::ShootAtPlayer()
	:ShotPlayer(false), fireDelay(fireTime)
{
	Name = TEXT("Shoot at Player");

	AddPrecondition("In Cover", true);
	AddEffect("Kill Enemy", true);
}


ShootAtPlayer::~ShootAtPlayer()
{
}

void ShootAtPlayer::Reset()
{
	ShotPlayer = false;
	fireDelay = fireTime;
}

bool ShootAtPlayer::IsDone()
{
	return ShotPlayer;
}

bool ShootAtPlayer::checkProceduralPrecondition(AActor * agent)
{
	/*ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);
	if (AiCharecter)
	{
		if(AiCharecter->currentCover)
			return (AiCharecter->currentCover->Active);
	}
	return false;*/
	return true;
}

bool ShootAtPlayer::perform(AActor * agent)
{
	ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);
	target = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(agent->GetWorld(), 0));

	if ((AiCharecter != nullptr) && (target != nullptr))
	{
		AiCharecter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(AiCharecter->GetActorLocation()* FVector(1, 1, 0), target->GetActorLocation()*FVector(1, 1, 0)));

		FHitResult RV_Hit(ForceInit);
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, agent);

		if (DoTrace(&RV_Hit, &RV_TraceParams, agent))
		{

			float deltaTime = agent->GetWorld()->DeltaTimeSeconds;
			fireDelay -= deltaTime;

			if (fireDelay <= 0)
			{
				if (AiCharecter->ProjectileClass != NULL)
				{
					UWorld* World = AiCharecter->GetWorld();
					if (World != NULL)
					{
						const FRotator SpawnRotation = agent->GetActorRotation();
						const FVector SpawnLocation = agent->GetActorLocation() + FVector(0, 50.0f, 120.0f);

						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						World->SpawnActor<ATacticalShooterAIProjectile>(AiCharecter->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					}
				}
				ShotPlayer = true;
			}

			return true;
		}
	}
	return false;
}

bool ShootAtPlayer::DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, AActor* agent)
{
	FVector Start = agent->GetActorLocation() + FVector(0, 50.0f, 120.0f);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(agent->GetWorld(), 0);
	FVector End = player->GetActorLocation() + FVector(0, 0, 50.0f);

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	DrawDebugLine(agent->GetWorld(), Start, End, FColor::Emerald, false, 0.0f, 0, 0.5f);

	bool DidTrace = agent->GetWorld()->LineTraceSingleByChannel(*RV_Hit, Start, End, ECC_Pawn, *RV_TraceParams);

	ATacticalShooterAICharacter* hitactor = Cast<ATacticalShooterAICharacter>(RV_Hit->GetActor());

	if (hitactor != nullptr)
	{
		return true;
	}

	return false;
}