// Copyright Eveline Gomes.

#pragma once

/** Interface */
#include "AbilitySystemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


/** Forward Declaration */
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	// GETTERS:
	/** Begin IAbilitySystemInterface Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End IAbilitySystemInterface Interface */

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/** 
	* Declare those pointers here since our player controlled character won't have them.
	*/
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
