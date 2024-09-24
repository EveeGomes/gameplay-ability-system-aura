// Copyright Eveline Gomes.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
   Super::BeginPlay();

   // Create the widget and add to the viewport
   UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
   Widget->AddToViewport();
}
