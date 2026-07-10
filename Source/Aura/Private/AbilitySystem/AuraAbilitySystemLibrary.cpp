// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	/**
	* Since a widget only exists for a local player, we'll be calling in that perspective (of a local player).
	*  For that we can get the player controller associated with that game instance (session). In other words, we can
	*  get the first player controller.
	* Then, from the PlayerController (PC) we can always access the HUD!
	* From the HUD, which in our case we're using the AuraHUD, we can get the OverlayWidgetController. However the getter
	*  function requires a reference to a FWidgetControllerParams, and consequently we need to fill it in with its 4 key
	*  variables.
	*  We need the PlayerState which we can get from the PC.
	*  ASC and AS as well. Since our AuraPlayerState has functions to get them we can use it for that. :)
	*/

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetPawn()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributesMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributesMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetPawn()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetAttributesMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
