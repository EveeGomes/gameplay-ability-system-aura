// Copyright Eveline Gomes.


#include "UI/HUD/AuraHUD.h"

/** Create a widget */
#include "UI/Widget/AuraUserWidget.h"

/** Create an overlay widget controller */
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
   if (OverlayWidgetController == nullptr)
   {
      // Create an overlay widget controller
      OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
      // Set the widget controller params
      OverlayWidgetController->SetWidgetControllerParams(WCParams);

      return OverlayWidgetController;
   }

   return OverlayWidgetController;
}

void AAuraHUD::BeginPlay()
{
   Super::BeginPlay();

   // Create the widget and add to the viewport
   UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
   Widget->AddToViewport();
}
