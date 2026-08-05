// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/**
	 * Bind to a delegate. We use AddObject() because it's not a dynamic delegate (we can see by checking its declaration)
	 * Now, EffectApplied is a callback that'll be called in response to any effect that gets applied to this ASC.
	 *
	 * There's an issue with using OnGameplayEffectAppliedDelegateToSelf: it its called just on server. So when testing
	 *  with 2 players on PIE, the client version isn't getting the message on screen when picking up a potion.
	 *  To fix that: we can take the callback &UAuraAbilitySystemComponent::EffectApplied and make it an RPC, or more
	 *  specifically a client RPC, it'll be called on the server but also exececuted on the owning client. Ie client RPCs
	 *  are designed to be called on the server and executed on the client!! If the call is on the server owned ASC, the
	 *  hosting player then the client RPC will only be called on the server and it won't be replicated down to the owning
	 *  client as the owning client is on the server already!
	 *  - So on .h we add a UPROPERTY and add keywords:
	 *   - Client: so it's a client RPC
	 *   - Reliable: it's guaranteed it reaches the client.
	 *  - Another convension is to prefix the callback with Client
	 *  - And it also needs an "_Implementation" added to the end of the name here on .cpp file!
	 */

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
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
	/**
	 * Check if there's any activatable abilities associated with the InputTag passed. Then, activate them if they're not
	 *  already activated (that's to avoid activating it every single frame since this is a held function).
	 *  There's a function we can call that returns all the activatable abilities. We need to check that because abilities
	 *  can be blocked by tags, so this function will give us an array of abilities we can activate.
	 *  So we loop through them and check the one that has InputTag since to activate only that one.
	 *  Abilities have the concept of acknolidging when an input that activates it is pressed or released. We can even
	 *  override those function to add anything we want to happen when they're called.
	 *  We'll use AbilitySpecInputPressed to tell the ability that its input has being pressed before activating it.
	 */

	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	/**
	 * Instead of simply end the ability if its input is release, we'll let the ability determine that because not all
	 *  abilities need to be cancelled or ended when their input is no longer pressed.
	 *  So here we'll tell the ability that its input is not being pressed anymore.
	 */

	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
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
