// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

/** Forward Declaration */
struct FOnAttributeChangeData;

/** 
* Add/Make dynamic multicast delegates to broadcast any change in attributes. Events will be bound to these delegates to let us perform other
*  updates as needed.
* Dynamic so that we can assign events in BP (in our widget BP specifically);
* Multicast because multiple BPs, widget BPs, may want to bind to these delegates so that they can update.
* 
* Once the types are created and the variables, they can be used in BP. If a WBP has access to the widget controller, they an assign an event
*  to receive that health if we broadcasted it.
* So, to broadcast values, we take the delegate and call .Broadcast()
*/
// These can send a single parameter. It can broadcast a float.
// Below we're just declaring the type, we'll need some member variables of these types so then we can assign in BP.
/** Delegates types */
// Health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
// Mana
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * We'll make this a BluprintType and Blueprintable.
 * BlueprintType means we can use it as a type in Event Graph (for example if we need to cast to it).
 * Blueprintable means we can make a blueprint based on this class.
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	/** Delegates themselves */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:
	/** Callbacks to bind to delegates that broadcast changes to attributes */
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

};
