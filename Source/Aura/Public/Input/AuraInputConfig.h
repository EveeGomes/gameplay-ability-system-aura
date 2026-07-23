// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

/**
 * Used to hold information about input actions linked to GTs.
 * With a pointer or reference of this AuraInputConfig, we can look up in the AbilityInputActions array any of our
 *  InputActions that has a specific tag!
 */

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction {nullptr};

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* Returns an InputAction associated with a GT, instead of an FAuraInputAction so it's more straight forward! */
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	/* Set on the Data Asset BP */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
