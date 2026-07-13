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

	/**
	 * The following strategy works, but we'd have to type the same code for each and every attribute, the existing ones
	 *  and future ones we might want to create. That would add boilerplate code to this class as well (there's a lot in
	 *  AuraAttributeSet already).
	 * So, the idea is to have a way of broadcasting each and every attribute (existent or new ones) no matter how many
	 *  there are or what are they.
	 * To start, we'll create a map, TMap, mapping Gameplay Tags to FGameplayAttribute because there are delegates broadcasting
	 *  whenever an attribute changes.
	 *  Luckily we have the Attribute Accessors for attributes declared in our AttributeSet. With the getter function we can
	 *  have the FGameplayAttribute to add to the map! This getter function is static, so if we want to map a GT to a static
	 *  function that returns an FGameplayAttribute (in this case), we can make a delegate!
	 *  So, in AuraAttributeSet we can declare a delegate that's able to have a function bound to it (the getter in this case,
	 *  that is a static function that returns an FGameplayAttribute). Also, this delegate that can bind a function that
	 *  RETURNS a value.
	 *  Then, we should create a map to map a GT to that delegate that returns an FGameplayAttribute! That map should be
	 *  accessible always, and using a GT we can find the attribute we need. That map will also be declared in our
	 *  AttributeSet class.
	 * Finally, this widget controller no longer needs to know how many attributes there are. To broadcast attributes
	 *  values, all it needs to do in this function is loop through the map with GT and FGameplayAttribute, find the
	 *  FAuraAttributeInfo associated with the tag, set the attribute value on that info and then broadcast so the widget
	 *  bound to the delegate can have the value and display to the player!! :)
	 *  **When getting the Value from the map, since it's a delegate, we should Execute so get the FGameplayAttribute.
	 *   Then from it, we can get the attribute value by calling GetNumericValue passing our AttributeSet.
	 */

	for (auto& Pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		FGameplayAttribute Attr = Pair.Value.Execute();
		Info.AttributeValue = Attr.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributesMenuWidgetController::BindCallbacksToDependencies()
{

}
