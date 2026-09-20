// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * This function is special internal function. It's a factory that creates and returns an instance of this task.
	 * 
	 * The OwningAbility param is set as a default parameter so when a GA calls this function, it will be passed as the
	 *	self (in BP) or 'this' (in C++). To achieve that, we need the 'meta' specifier and pass the DefaultToSelf
	 *	property. We'll also hide this pin so we'll use HidePin.
	 * 
	 * @param OwningAbility the GA that calls and runs this task.
	 * @return an instance of this class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"), BlueprintInternalUseOnly = "true")
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
};
