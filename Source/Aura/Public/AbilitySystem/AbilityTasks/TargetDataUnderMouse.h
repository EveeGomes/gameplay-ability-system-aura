// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/**
 * Custom Ability Task used to get data under the mouse cursor.
 * That data can/will be used to redirect the player character to the direction of the attack.
 * ps: The term TargetData, in GAS, means something specific.
 */

/**
 * Delegate types/signatures used to create variables that will represent output execution pin on the
 *  CreateTargetDataUnderMouse(...) method.
 */

// One param to broadcast the value of the mouse cursor location
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * This function is special internal function. It's a factory that creates and returns an instance of this task.
	 *
	 * It is an asynchronous BP node, meaning it can have multiple output execution pins.
	 * Now, to create those multiple output execution pins, we got to broadcast delegates, and they MUST BE variables
	 *  on this class. (IE we must create the delegates and then variables of those types!)
	 *  For the output execution pin to be executed though, the delegate variable needs to be broadcasted, otherwise the 
	 *   pin will exist on the BP node but never be used/executed.
	 * 
	 * The OwningAbility param is set as a default parameter so when a GA calls this function, it will be passed as the
	 *	self (in BP) or 'this' (in C++). To achieve that, we need the 'meta' specifier and pass the DefaultToSelf
	 *	property. We'll also hide this pin so we'll use HidePin.
	 * 
	 * @param OwningAbility the GA that calls and runs this task.
	 * @return an instance of this class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	/**
	 * Variables of delegates types used to broadcast data; they'll be output execution pins of
	 *  CreateTargetDataUnderMouse() method.
	 */
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
};
