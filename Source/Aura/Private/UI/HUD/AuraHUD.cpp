// Copyright Eveline Gomes.


#include "UI/HUD/AuraHUD.h"

/** Create a widget */
#include "UI/Widget/AuraUserWidget.h"

/** Create an overlay widget controller */
#include "UI/WidgetController/OverlayWidgetController.h"

/** For initialize the widget and widget controller */
#include "AbilitySystemComponent.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
   if (OverlayWidgetController == nullptr)
   {
      // Create an overlay widget controller
      OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
      // Set the widget controller params
      OverlayWidgetController->SetWidgetControllerParams(WCParams);
   }

   return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
   // Perform a check that will also print a formatted string to the crash log if the condition fails and a crash happens
   checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
   checkf(OverlayWidgetControllerClass, TEXT("OVerlay Widget Controller Class uninitialize, please fill out BP_AuraHUD"));

   // Create the widget
   UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
   // Cast Widget to AuraUserWidget so we can set its widget controller
   OverlayWidget = Cast<UAuraUserWidget>(Widget);
   // Before using the OverlayWidget set functions, we need to create the widget controller first.
   // Create the params struct and pass the parameters received by this function
   const FWidgetControllerParams WidgetControllerParams{ PC, PS, ASC, AS };
   // Construct using the AuraHUD GetOverlayWidgetController() and store the return value into a pointer of type UOverlayWidgetController
   UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

   // Tie the Overlay widget and overlay widget controller together
   OverlayWidget->SetWidgetController(WidgetController);
   // Broadcast initial values
   WidgetController->BroadcastInitialValues();


   // Finally, add the widget to the viewport.
   Widget->AddToViewport();
}