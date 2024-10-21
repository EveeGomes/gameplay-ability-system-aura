// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
   // Bind to a delegate. We use AddObject() because it's not a dynamic delegate (we can see by checking its declaration)
   // Now EffectApplied is a callback that'll be called in response to any effect that gets applied to this ASC.
   OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
   // For testing:
   GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("EffectApplied!"));
}
