// Copyright Eveline Gomes.


#include "UI/WidgetController/OverlayWidgetController.h"

/** */
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
   // No need to call super since it's empty

   /** 
   * Take the delegates and broadcast some values.
   * Instead of hardcoding, we need to broadcast the real value of the attribute.
   * To get access to AuraAttributeSet we'll cast AttributeSet ptr using CastChecked because it should never fail as long as we call
   *  this function, BroadcastInitialValues after the AttributeSet member variable has been initialized. So we just need to make sure to only
   *  call this after AttributeSet has become valid.
   * Then, we can just get access to AuraAttributeSets attribute accessors.
   * 
   * Now, where we should call this function? Since we have to make sure we have valid AttributeSet already, and we should also call this after
   *  any widgets have had their widget controller set.
   *  In AuraHUD -> InitOverlay(), we're tiding the widget to its widget controller after creating and setting the widget controller with all the
   *  4 key variables in it. So, that's where we can do things like bind to the widget controllers delegates. i.e. only after that tied up should
   *  we tell the widget controller to broadcast its initial values.
   * So as long as we're setting the widget controller and binding those widget BPs events to the widget controllers delegates, as long as that's
   *  happening FIRST (tie widget-widget controller), then we can call BroadcastInitialValues() and our widget BPs can respond to them.
   */
   
   const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
   // Broadcast initial values
   OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
   OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
