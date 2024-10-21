// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
/** 
* We need a place to bind the delegate, but the constructor is too early.
* We could create a function that'll be called as soon as something happens. In AuraCharacter we have the function InitAbilityActorInfo that we call
*  whenever we want to call InitAbilityActorIndo on the ability system itself. So, we could make this a virtual function in AuraCharacterBase in order
*  to let it be implemented in both AuraEnemy and AuraCharacter. Then, whenever they call it, they can also call some function on the ability system
*  to let it know it's time to bind to any delegate it wants to bind to.
*/

public:
	/** 
	* When this function is called, we know the AbilityActorInfo has been set. This way the ASC can do important things such as bind to delegates safely.
	* We've added the call in both AuraEnemy and AuraCharacter. However, once we do that now the character is dependent on the ASC. So we gotta make sure
	*  that our ASC doesn't know about the character, in order to keep the dependency one-way.
	*/
	void AbilityActorInfoSet();

protected:
	/** Begin UAbilitySystemComponent */
	// Callback to bind to the multicast delegate on UASC class of type FOnGameplayEffectAppliedDelegate
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	/** End UAbilitySystemComponent */


};
