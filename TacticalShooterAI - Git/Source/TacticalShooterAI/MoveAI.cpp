#include "MoveAI.h"
#include "TacticalPawn.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

MoveAI::MoveAI()
	:nearPlayer(false)
{
	AddEffect("Near Enemy", true);
}


MoveAI::~MoveAI()
{
}

void MoveAI::Reset()
{
	nearPlayer = false;
}

bool MoveAI::IsDone()
{
	return nearPlayer;
}

bool MoveAI::checkProceduralPrecondition(AActor * agent)
{
	//Currently No preconditions just run
	return true;
}

bool MoveAI::perform(AActor * agent)
{
	ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);
	target = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(agent->GetWorld(), 0));
	float deltaTime = agent->GetWorld()->DeltaTimeSeconds;

	if ((AiCharecter != nullptr) && (target != nullptr))
	{
		FVector vectorToDestination = target->GetActorLocation() - AiCharecter->GetActorLocation();
		if (vectorToDestination.Size() > 20)
		{
			AiCharecter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(AiCharecter->GetActorLocation()* FVector(1,1,0), target->GetActorLocation()*FVector(1, 1, 0)));
			AiCharecter->SetActorLocation((AiCharecter->GetActorLocation() + (UKismetMathLibrary::Normal(vectorToDestination) * speed * deltaTime)));
		}
		else
		{
			nearPlayer = true;
		}
		return true;

		/*AAIController* controller = Cast<AAIController>(AiCharecter->GetController());
		if(controller != nullptr)
		{
			controller->MoveToActor(target, 5.0f);
			return true;
		}*/
	}
	return false;
}
