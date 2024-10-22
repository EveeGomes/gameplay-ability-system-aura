// Copyright Eveline Gomes.


#include "UI/WidgetController/OverlayWidgetController.h"

/** BindCallbacksToDependencies() */
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

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
   /** Broadcast initial values */
   // Health
   OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
   OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
   // Mana
   OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
   OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

   /** 
   * For us to be able to respond to when those attributes change, we'll use a function from the Ability System Component that requires the
   *  attribute we're talking about and then returns a delegate we can bind to. We bind to it using .AddUObject() (if it was a dynamic multicast
   *  delegate we'd use AddDynamic()). When calling AddUObject we need to also pass a callback function that must have a specific signature so it 
   *  can be bound. But instead of doing this directly in this function, we'll create one in the parent class that will take care of the bindings.
   * 
   * Binding callbacks to the dependencies of this widget controller is something all widget controllers need to do. Therefore we'll make a function
   *  we can use to bind our callbacks to all those dependencies. That will be done in the parent class, AuraWidgetController so all widget controllers
   *  can implement that function!
   * Then, we create (in this class/child class!) the bind functions using the specific signature needed.
   */
   //AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &callback fully qualified);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
   // No need to call Super (it's empty)
   
   /** Bind callback function to be called whenever the attribute, related to it, changes. */
   const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

   AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
      AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

   AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
      AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

   AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
      AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

   AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
      AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

   /** 
   * Get AuraASC to bind to its delegate: EFfectAssetTags.
   * To bind to it, we need a callback function. But we can also use a Lambda which is an anonymous function (it doesn't have a name, doesn't get
   *  declared anywhere, isn't a member function... it's just a function we can define where we want some functionality to happen). The signature
   *  for a lambda is basically: [](@input parameter list){//whatever this function does}
   * This WidgetController is receiving the data from our ASC and parsing this data to do something, which for now is looping through the container
   *  and printing to the screen!
   * 
   * Now, we'd like to use the tags and broadcast information the our widgets can receive. Rather than broadcasting a GT up to the widget side, we
   *  could have an asset that can look up information that we can broadcast to the widget. Eg: we can have an asset like a data table that can 
   *  look things up by tag (by GT), and retrieve some kind of information, like a struct, that we can pass up to the widgets.
   * We'll create a data table that has information related to GTs specifically to show messages to the screen. The row structute will be defined 
   *  here in C++ as a struct, in the .h file of this class.
   */
   Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
      [](const FGameplayTagContainer& AssetTags) {
         /** 
         * Make a look up in our widget data table:
         * Find the data table row that corresponds to the gameplay tag (in our case each element in the GTContainer). So, we'd like to take a DT
         *  and a GT, and return the row. We'll then create a function that'll be versitle and return any sort of roll, DT roll... by just taking
         *  a generic table and a GT.
         */

         // Broadcast the GTs associated with the GE (we added those tags in the GE BP)
         for (const FGameplayTag& Tag : AssetTags)
         {
            // Broadcast tag(s) to the WidgetController
            const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
            GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
         }
      }
   );
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
   /** 
   * All we want to do when the health changes is to broadcast our OnHealthChanged delegate so the widgets can respond to it.
   * We use Data.NewValue to get the value that has just changed (instead of using AuraAttributeSet->GetHealth()).
   */
   OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
   OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
   OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
   OnMaxManaChanged.Broadcast(Data.NewValue);
}
