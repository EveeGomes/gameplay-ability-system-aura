// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAttributeSet.h"

/** Components */
#include "AbilitySystemComponent.h"

UAuraAttributeSet::UAuraAttributeSet()
{
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
   // Inform the ability system that Health has just been replicated
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}
