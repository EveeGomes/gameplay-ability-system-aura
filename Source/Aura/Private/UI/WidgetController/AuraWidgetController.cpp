// Copyright Eveline Gomes.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
   PlayerController = WCParams.fPlayerController;
   PlayerState = WCParams.fPlayerState;
   AbilitySystemComponent = WCParams.fAbilitySystemComponent;
   AttributeSet = WCParams.fAttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{

}

void UAuraWidgetController::BindCallbacksToDependencies()
{

}
