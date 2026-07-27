// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// Bind to a delegate. We use AddObject() because it's not a dynamic delegate (we can see by checking its declaration)
	// Now EffectApplied is a callback that'll be called in response to any effect that gets applied to this ASC.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	/**
	 * To grant an ability we need to create an AbilitySpec from an ability class.
	 *  Then, we use a function from the ASC to grant the ability, GiveAbility().
	 *  Another way of granting is using GiveAbilityAndActivateOnce();
	 *
	 * Here's the place where we want to check if the ability has a valid Startup input tag.
	 * We want our GameplayAbilities to have the concept of an InputTag, that we'd like to change at runtime. Having a
	 *  variable on AuraGameplayAbility is great for Startup input tags, but if we'd like to be able to change the tags
	 *  we can't use variables on AuraGameplayAbility (e.g. have an ability mapped to the LMB and want to change to the RMB)!
	 * The GameplayAbilitySpec has a specific GameplayTag container for tags that can be added or removed dinamically
	 *  throughout the game, and that's perfect for our InputTag idea :).
	 * Since this method adds Startup abilities for the first time at the beginning of the game, this is a good place
	 *  to check the Startup InputTags.
	 * And we can add those to the AbilitySpec for that given ability (AbilitySpec is the represensation of an ability).
	 * We need to cast to our AuraGameplayAbility class because the ability we get from the AbilitySpec is a simple
	 *  UGameplayAbility and ours has the StartupInputTag.
	 * Tags in DynamicAbilityTags can be added and/or removed at runtime!
	 *
	 * Then, we'll implement the callback functions on the PlayerController that are called by pressing any key bound to
	 *  an IA from AuraInputConfig! Since when those IAs are fired they pass an InputTag, we're able to use the ASC will
	 *  that will be able to know which ability has that tag (InputTag) to perform something like activate the ability!
	 */

	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		/**
		 * While looping through abilities, if any are derived from AuraGameplayAbility, get the StartupInputTag and
		 *  add to the DynamicAbilityTags. Then, give the ability.
		 */
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
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
