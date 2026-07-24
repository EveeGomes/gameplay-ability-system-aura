// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * So the next step is to bind callback functions to input actions. That is already done on the PlayerController using
 *  the EnhancedInputComponent by calling the function BindAction(...).
 *  By checking that function definition, one of its parameters is a function that can take zero to many arguments. Then,
 *  for our case, we'd like a way to pass say gameplay tags of even the InputConfig and have a function taking care of
 *  binding callbacks to their respective InputActions.
 * The best way to approach this is to create a custom EnhancedInputComponent in order to make these bindings there since
 *  that component is the one used by the PlayerController to call BindAction. So it's better to have the component deal
 *  with this responsibility while the PlayerController only cares on calling a BindAction function for example.
*/

/**
 * This component is set to be used as the default EnhancedInputComponent for this project.
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	
public:
	/** 
	 * Called from the PlayerController. It binds Inputs to callbacks.
	 * The callbacks should be parameters to this function!
	 *
	 * Since it's a template function and therefore defined here on the .h file, we cannot just forward declare a type
	 *  we're using, we must include the header file.
	 *  UserClass type will be used because we'll call 'BindAction' inside this function, and it requires a User object.
	 *  PressedFuncType - the type of the function we'd like to bind for when an input is pressed.
	 *  ReleasedFuncType - for when an input is released.
	 *  HeldFuncType - for when an input is held.
	 *  So this templated function is capable of receiving function pointers, and it doesn't matter those functions signatures
	 *   because this is a template function so it'll handle whatever passed in function.
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	/**
	 * Check InputConfig.
	 * Then, loop through the InputConfig's AbilityInputActions!
	 * For each Action, we'll bind the callback function that is valid (that was passed in to this function). The last
	 *  argument to BindAction will be the tag which will be passed to the callback function.
	 */

	check(InputConfig);

	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
