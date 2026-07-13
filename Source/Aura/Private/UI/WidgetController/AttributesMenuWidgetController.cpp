// Copyright Eveline Gomes.


#include "UI/WidgetController/AttributesMenuWidgetController.h"

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
	 *
	 *  However, there's a much better way of doing it. Now we still need a lot of boilerplate code in our AttributeSet.
	 *   That's because we're using a delegate. But looking at the Bind function we use, BindStatic, it accepts a function.
	 *   And checking the signature of BindStatic we see that it accepts a type, TBaseStaticDelegateInstance. In the description
	 *   of BindStatic, it says "Binds a raw C++ pointer global function delegate". These delegates are just types that can
	 *   store function pointers. The idea here is to use that type, accepted by BindStatic, in the map! Ie we should
	 *   map a GT to a TBaseStaticDelegateInstance.
	 *   To do that we'll create an instance of TBaseStaticDelegateInstance. That's a templated type which we pass some
	 *   "parameters" to the template <> brackets. It asks for the function we want, by asking for the return type and params.
	 *   In out case we're intereted in Get##AttributeName##Attribute() which is one of ATTRIBUTE_ACCESSORS that returns an
	 *   FGameplayAttribute and receives no input. So we'd have something like: for the 1st arg it would be the type of
	 *   function signature we want. Then we pass a default arg for delegate usage.
	 *   TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>.
	 *   What we really want is a function pointer, and that we can find in the instance of TBaseStaticDelegateInstance we're
	 *   creating. In the template class definition we see it has a public FFuncPtr, which assembles the template inputs into
	 *   a function pointer with the correct signature:
	 *   TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FunctionPointer;
	 *   In essence, a function pointer can hold a function with the signature specified! Eg:
	 *		FunctionPointer = GetIntelligenceAttribute; // no need to use the (), here we're passing only the function address!
	 *		FunctionPointer(); // this will CALL the function, which in this case is the getter function and will return a value
	 *		FGameplayAttribute Attribute = FunctionPointer();
	 *	>> Now, since we need to specify the TYPE of a TBaseStaticDelegateInstance to map the GT in the GT to Attribute map,
	 *	 the map declaration will be:
	 *	 TMap<TGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	 */

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributesMenuWidgetController::BindCallbacksToDependencies()
{
	/**
	 * In order to broadcast the values of attributes when they change, we need to bind functions (or lambdas) to attributes
	 *  delegates that broadcast new value whenever their values change! That comes from the ASC and it's similar to what is
	 *  done in other WidgetController (like the Overlay one).
	 * When capturing some values in the lambda, sometimes it is better to capture by value. For example the Pair variable:
	 *  by the time am attribute changes and the delegate gets broadcast the Pair variable which is local to the for loop
	 *  will have long since gone out of scope. So, we just wanna capture by value to have a copy stored within the lambda.
	 */

	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributesMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}