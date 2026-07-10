// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

/**
 * This struct stores information related to a given attribute.
 * It'll be used to broadcast information from the attribute that has changed. The widget BP that needs those data
 *  will use them in order to accordingly update the widgets associated with the attribute changed.
 */

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// Identify each of this struct object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	/**
	 * Pieces of information that will be passed to widgets (use FText since this is the type used in widgets for user facing info):
	 * - AttributeName -> so it can be set in the AttributeMenu
	 * - AttributeDescription
	 * - AttributeValue -> not available for editing through the data asset
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
};

/**
 * Data Assets allows for storing asset information in a blueprintable settable class.
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Look up function. Returns the correct AttributeInfo based on the gameplay tag.
	 * @param AttributeTag attribute tag to find the corresponding info struct
	 * @param bLogNotFound if trying to find a AuraAttributeInfo that doesn't exist, use this log to log an error. Then,
	 *	if passing true to this param, a log message is shown in that condition of not finding a struct for an attribute.
	 */
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	// Filled in BP with attributes information
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
