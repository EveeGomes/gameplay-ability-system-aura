// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AttributesMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FAuraAttributeInfo;
/**
 * To broadcast information we need a delegate. This will broadcast information from an attribute using the struct that
 *  holds all information the AttributeMenu widget needs.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, AttributeInfo);

/**
 * Used by the Attribute Menu, meaning it needs its widget controller set (this widget controller!) - or make this set itself.
 *  We need to find a place to construct this widget controller (eg the OverlayWidgetController is constructed in AuraHUD).
 *  The idea will be to create a Blueprint Function Library with some static functions that can be used in BP so widgets once
 *  created can get their widget controller and set.
 *  The Overlay is a bit different, as well as the globe widgets which depends on the perfect timing to have their widget
 *  controllers set (ie it needs that the overlay widget had been already constructed).
 * Retrieve information from Attributes and send over to the Attributes Menu to be displayed. 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributesMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	/**
	 * It'll broadcast Attributes initial values so the AttributesManu widget can display.
	 * So, to broadcast info to the AttributesManu widget, we can use the struct that we made and holds all info to be
	 *  displayed.
	 */
	virtual void BroadcastInitialValues() override;

	
	virtual void BindCallbacksToDependencies() override;

	// Broadcast AttributeInfo
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
protected:
	/**
	 * Set in BP, and used to look up the AttributeInfo based on the gameplay tag!
	 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
