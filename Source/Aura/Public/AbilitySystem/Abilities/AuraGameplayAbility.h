// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Now that we have the AuraInputComponent used in AuraPlayerController to bind callbacks to InputActions as well as
	 *  those IAs in the AuraInputConfig Data asset, we need to link the Abilities with GameplayTags in order to use the
	 *  tags to link the Abilities with InputActions!
	 *
	 * We've set up a function to grant startup abilities using our ASC at the beginning of the game! Since we start with those
	 *  abilities, they should be associated with some InputTag (the ones from InputConfig) by default. This way we know if
	 *  we press a particular input, that's in InputConfig, the ability associated with it will be activated!!
	 *  For that reason, we'll add an FGameplayTag variable to our AuraGameplayAbility. This variable will be ONLY for startup
	 *  abilities since we want to be able to change abilities' InputTags at runtime.
	 */
	
	/**
	 * Set at the beginning of the game when giving startup abilities to the character. Should be unchanged at runtime!
	 * This will be set for our Class Default Object, for any given ability. But that's not something we should change
	 *  at runtime since our abilities have different instancing policies. Also, it's the GameplayAbilitySpec that will
	 *  be using throughout the game. So, this variable is only good to check at the very beginning when giving abilities
	 *  to the character.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
