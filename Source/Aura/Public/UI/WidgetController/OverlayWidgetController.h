// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

/** 
* Add/Make dynamic multicast delegates to
* Dynamic so that we can assign events in in BP (in our widget BP specifically);
* Multicast because multiple BPs, widget BPs, may want to bind to these delegates so that they can update.
* 
* Once the types are created and the variables, they can be used in BP. If a WBP has access to the widget controller, they an assign an event
*  to receive that health if we broadcasted it.
* So, to broadcast values, we take the delegate and call .Broadcast()
*/
// These can send a single parameter. It can broadcast a float.
// Below we're just declaring the type, we'll need some member variables of these types so then we can assign in BP.
/** Delegates types */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);

/**
 * 
 */
UCLASS()
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;

	/** Delegates themselves */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
};
