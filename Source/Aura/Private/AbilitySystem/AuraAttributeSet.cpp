// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAttributeSet.h"

/** Register variables for replication */
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
   InitHealth(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   /** 
   * Call the following macro to register variables to be replicated.
   * COND_None means it'll be replicated without any conditions.
   * REPNOTIFY_Always means that if the value is set on the server and replicated, then on the client it'll be updated and set. The other option,
   *  the default one, _OnChanged, if the variable is set to a value on the server and that value hasn't changed, then there won't be any replication;
   *  but for GAS we want to replicate anyways, even if there's no change (that's because if we're setting it we want to respond to the act of setting it, 
   *  whether if we set it to a new value or its own same value).
   */
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
   // Inform the ability system that Health has just been replicated
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
