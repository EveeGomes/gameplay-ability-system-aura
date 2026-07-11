// Copyright Eveline Gomes.


#include "UI/WidgetController/AttributesMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributesMenuWidgetController::BroadcastInitialValues()
{
	/**
	 * Get the AuraAttributeSet by casting the AttributeSet variable.
	 * Check the AttributeInfo to make sure it'd been set in BP.
	 * Then use it to find the AttributeInfo passing the tag. The tag we can get using the FAuraGamplayTags native tags!
	 * Then, we set the attribute value in the AttributeInfo since other information is already set on the DataAsset we
	 *  created in the editor! This attribute value isn't set on the data asset because we want the correct value that
	 *  we get from the AttributeSet at the time that we broadcast it.
	 *  Then, our widget needs to bind to the AttributeInfoDelegate BEFORE this BroadcastInitialValues() is called!
	 */

	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();

	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributesMenuWidgetController::BindCallbacksToDependencies()
{

}
