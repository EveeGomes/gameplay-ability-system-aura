// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
/**
 * All function should be static since we don't plan on creating an instance of this class!
 * Now, static functions can't access objects that exist in the world. That's because the class that the static function
 *  belongs to, might not exist in the world either, so the static function is called directly. However, it still needs
 *  its way to what it should access, and for that reason many function libraries in the Engine require a WorldContextObject,
 *  which is a reference that we can use, making possible to trace its way up to find whatever we want that exists in the world.
 *  So, the reference to this object is always a param that we pass in static functions from libraries in order to have context
 *  for the world we are in.
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Getter for the OverlayWidgetController.
	 * Called and used from a widget (as widgets need their widget controllers - for this project ofc).
	 */
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
