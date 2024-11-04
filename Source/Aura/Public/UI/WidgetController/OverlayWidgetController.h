// Copyright Eveline Gomes.

#pragma once

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

/** Forward Declaration */
struct FOnAttributeChangeData;
class UAuraUserWidget;
class UDataTable;

/* Row structure of data table to hold information about GTs */
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	/* Used to display messages to the screen */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag(); // make sure it's initialized. In this case it's initialized to an empty FGameplayTag

	/* Message to display. As for widgets, when displaying text to the user in the form of widgets, we use FText */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	/* Have a widget class so that we can create and show to the screen any kind of widget depending on the GT we receive in the form of a GE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	/**
	* This message widget may or may not want to have an image associated with it... or any other data we'd like to pass up, if we wanted to harvest
	*  more data, such as the magnitude of the modifier. For this we'll add an image that can be optional.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

/** 
* Add/Make dynamic multicast delegates to broadcast any change in attributes. Events will be bound to these delegates to let us perform other
*  updates as needed.
* Dynamic so that we can assign events in BP (in our widget BP specifically);
* Multicast because multiple BPs, widget BPs, may want to bind to these delegates so that they can update.
* 
* Once the types are created and the variables, they can be used in BP. If a WBP has access to the widget controller, they can assign an event
*  to receive that health if we broadcasted it.
* So, to broadcast values, we take the delegate and call .Broadcast()
*/
// These can send a single parameter. It can broadcast a float.
// Below we're just declaring the type, we'll need some member variables of these types so then we can assign in BP.
/** Delegates types */
// Delegate type for broadcasting any kind of attributes values
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

// Delegate to send out messages from a DT
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


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
	/** Begin UAuraWidgetController */
	virtual void BroadcastInitialValues() override;
	
	// 
	virtual void BindCallbacksToDependencies() override;
	/** End UAuraWidgetController */

	/** Delegates themselves */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	/** 
	* Get access to the message DT so it can be used in the lambda bound to EffectAssetTags in BindCallbacksToDependencies() to perform
	*  some kind of look up and broadcast data from that DT up to widgets.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/** 
	* Returns any type of DataTable (DT) row by using a DataTable and a Tag.
	* This function is more versitle than what this class needs, so it's a good example of a function that could be placed in a static class (as a 
	*  static function library).
	*/
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
