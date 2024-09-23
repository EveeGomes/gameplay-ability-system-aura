// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/**
 * This is our class responsible for getting data from the system, from the MODEL in our architecture, and then broadcast to our user widgets.
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
	/** 
	* Here we'll have a set of variables from which it's going to get any data.
	* As for this game project, we're interested in 4 classes to retrieve data, so we'll have to have access to them:
	*  - the ASC;
	*  - the attribute set;
	*  - the player state;
	*  - the player controller
	* We'll make these as protected variables, so children can have access to it.
	*/

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
