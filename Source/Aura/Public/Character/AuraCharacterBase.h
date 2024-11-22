// Copyright Eveline Gomes.
#pragma once


/** Interface */
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

/** Forward Declaration */
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

/** 
* Adding IAbilitySystemInterface will allow us to check any actor to see if they have this interface, so we can use the overriden
*  function to get that actor's ability system component!
*/
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
public:	
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/** 
	* Add a Skeletal Mesh component to this base class so both children can inheret and both can have a weapon.
	* This weapon will be attached to the socket on the skeleton, therefore we need to make sure the skeleton meshes 
	*  have the socket we specified.
	*
	* Using TObjecPtr wrapper for members, adds features such as access tracking and optional lazy load behavior.
	*  - Access tracking allows us to know when an object is being used, ie we can track how often a pointer is
	*  accessed or dereferenced.
	*  - Lazy load behavior "means that an asset can not be loaded until it's actually needed or used in the program".
	* In packaged build it behaves the same as a raw pointer though, but add those features in editor!
	* 
	* Nice to check: https://forums.unrealengine.com/t/why-should-i-replace-raw-pointers-with-tobjectptr/232781
	* 
	* As all pointers, we'll make it a UPROPERTY()
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** 
	* Add pointers to store the AbilitySystemComponent and AttributeSet in this Base class for all children inheret them. However, not all
	*  children will construct them. For example we'll construct and own those in the enemy class, but for the playe controlled character
	*  those will be owned by the PlayerState! 
	*/
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	/** 
	* Gameplay Effect classes and functions to initialize Primary, Secondary, and Vital Attributes from AuraAttributeSet
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;
};
