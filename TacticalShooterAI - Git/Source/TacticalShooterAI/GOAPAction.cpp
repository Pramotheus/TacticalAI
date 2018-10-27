#include "GOAPAction.h"



GOAPAction::GOAPAction()
	:preconditions(), effects()
{
	preconditions.Reserve(10);
	effects.Reserve(5);
}

GOAPAction::~GOAPAction()
{
}

void GOAPAction::DoReset()
{
	target = nullptr;
	Reset();
}

void GOAPAction::AddPrecondition(FString key, bool value)
{
	preconditions.Add(key, value);
}

void GOAPAction::RemovePrecondition(FString key)
{
	preconditions.Remove(key);
}

void GOAPAction::AddEffect(FString key, bool value)
{
	effects.Add(key, value);
}

void GOAPAction::RemoveEffect(FString key)
{
	effects.Remove(key);
}

TMap<FString, bool> GOAPAction::GetPreconditions()
{
	return preconditions;
}

TMap<FString, bool> GOAPAction::GetEffects()
{
	return effects;
}

float GOAPAction::GetCost()
{
	return cost;
}
