// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

/** Forward Declaration */
class UAuraUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * A HUD is a way of projecting information in form of text, images, animations etc, to inform the player what's happening to its character, or
 *  to the environment. It can aso represent menu screens, credits, etc.
 *  (https://jollymonsterstudio.com/2019/03/12/unreal-engine-c-fundamentals-ahud-uuserwidget-and-uwidgetanimation/)
 * 
 * A HUD class is a great place to do HUD related things such as drawing widgets to the screen.
 * Here we'll add the necessary code to add the overlay widget to the screen.
 * 
 * InitOverlay() neeeds the 4 key variables a widget controller has. To call it we must know where those variables are initialized already,
 *  and a good place to do it is in AuraCharacter InitAbilityActorInfo() function, which is called in both PossessedBy() for taking care of
 *  the server, and called in OnRep_PlayerState() which takes care of the client.
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	/**
	* Create a widget controller if it hasn't been created yet, or return if there is already one. i.e. we only ever have
	*  one overlay widget controller (similar to a singleton, we'll only be able to get it from a getter which also creates one if it still doesn't
	*  exist).
	*/
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// Construct the widget, the widget controller, set the widget's widget controller and add it to the viewport
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:


private:
	// As we need to know which class we need to create the widget, we gotta store it in a UClass kind of pointer variable. Set it in BP.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	// Store the overlay widget controller
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	// To create the overlay widget controller, we need a UClass of type UOverlayWidgetController
	UPROPERTY(EditAnywhere) // since we need to set it from BP
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
