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
   /** 
   * To show things in the HUD we need to know about our dependencies.
   * AuraASC know nothing about our WidgetController, but the WidgetController is what broadcasts data to the widgets! Now, if we want to show something
   *  to the screen, that's in the VIEW domain, which receives data from the Controller domain, which consequently receives info from the ASC, so our ASC
   *  should have a delegate. The question is: what our delegate is going to broadcast? What information do we want? A good way to identify things are with
   *  Gameplay Tags! So, if an effect carries a GT, we can broadcast that to the WidgetController, which can then broadcast it to the HUD!
   * So, here we'll get some gameplay tags from the GEs, and broadcast them in a delegate to use them to display things to the screen.
   */

   // Get a GT: create a container that will carry all the tags
   FGameplayTagContainer TagContainer;
   EffectSpec.GetAllAssetTags(TagContainer);

   // Broadcast asset tags. This happens in response to having any effect applied to this ASC!
   // The WidgetController will be responsible for parsing the data (in our case the OverlayWidgetController)
   EffectAssetTags.Broadcast(TagContainer);
}
