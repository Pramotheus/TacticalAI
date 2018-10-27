#include "FindCover.h"
#include "TacticalPawn.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

FindCover::FindCover()
	:hasCover(false)
{
	Name = (TEXT("Find Coverpoint"));

	AddEffect("Has Cover", true);
}


FindCover::~FindCover()
{
}

void FindCover::Reset()
{
	hasCover = false;
}

bool FindCover::IsDone()
{
	return hasCover;
}

bool FindCover::checkProceduralPrecondition(AActor * agent)
{
	return true;
}

bool FindCover::perform(AActor * agent)
{
	ATacticalPawn * AiCharecter = Cast<ATacticalPawn>(agent);

	if (AiCharecter != nullptr)
	{
		for (auto cover : AiCharecter->allCoverPoints)
		{
			if (cover->Active)
			{
				AiCharecter->currentCover = cover;
				hasCover = true;
				return true;
			}
		}
	}
	return false;
}
