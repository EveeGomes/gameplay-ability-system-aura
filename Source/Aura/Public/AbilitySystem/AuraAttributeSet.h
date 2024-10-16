// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

/** Components; Access to macros */
#include "AbilitySystemComponent.h"

#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	/** 
	* Declare the attributes
	* These are of type FGameplayAttributeData and most of them will be replicated. When a variable replicates, rep notify is called automatically.
	*  So, when a server replicates a variable down to a client, the client will have a rep notify for that variable triggered as a result.
	*  To use that, we'll use instead, of Replicated specifier in UPROPERTY, ReplicatedUsing = name_of_rep_notify. This name is conventionally created
	*  as OnRep_attribute_name, which is a function we'll create associated to that attribute.
	* That function has to be a UFUNCTION in order to be OnRep. It might have no arg or 1 arg of the same type as the attribute, ie of type
	*  FGameAttributeData. When the variable is replicated (it'll hold its new replicated value), OnRep is called and if it has the arg, ít'll hold the 
	*  "old" value of that attribute, so it can be useful for example to compare the old and current values.
	* 
	* Once we set an OnRep notify to a replicated attribute, we must inform the Ability System of that change so it can bookkeep the changes and make
	*  sure the whole AS is working cohesively.
	* 
	* There's another step that must be done in order to register a variable for replication: override a function where we register variables for
	*  replication!
	* 
	* Attributes consist of two values:
	*  - Base value: this is the permanent value of an attribute.
	*  - Current value: the base value plus any temporary modification caused by Gameplay Effects (buffs and/or debuffs)
	*  IMPORTANT: the Base value IS-NOT the max value. Max value for an attribute is set separately from the attribute itself. In fact the max value
	*   should be its own a separate attribute since it can/might change. By separating it, we can have Gameplay Effects affecting the attribute itself
	*   or the Max value separately. And the fraction of a health bar, for example, could be calculated as Health attribute set/MaxHealth attribute set.
	*/
	
	/** Begin UObject */
	// Register variables for replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** End UObject */

	/** Begin UAttributeSet */
	// Called right before an attribute changes (changes to CurrentValue), either from a GE or changed directly through attribute accessors.
	//  This function is used to do Campling, not to call other functionality. However, it doesn't permanently change the modifier, just the value
	//  returned from querying the modifier. Later operations recalculate the current value from all modifiers, so we'll need to clam again.
	// https://github.com/tranek/GASDocumentation/blob/master/README.md#445-preattributechange
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** 
	* Executed after a GE changes an attribute, more precisely to the BaseValue of an attribute from an Instant GE.
	* We have access to a lot of information via the parameter Data, which will be based on the effect that's just been applied. Therefore, this is
	*  a great place to do more attribute manipulation when they change from a GE. 
	*  https://github.com/tranek/GASDocumentation/blob/master/README.md#446-postgameplayeffectexecute
	*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	/** End UAttributeSet */

	/**
	* Since we haven't learned about Gameplay Effects yet, we'll use some functions to access the attributes to retrieve and/or set them.
	* Note: we normally don't set them from code directly, but with gameplay effect!
	*
	* So, for getters and setters we can use some macros (Check them by checking the AttributeSet.h).
	* 
	* But, instead of writing all macros we need, we'll copy the definition from AttributeSet.h and paste it here right after the includes.
	* That's yet another boilerplate code we'll add to this class.
	* Therefore, if we need to say initialize an attribute, we can use the Init function defined by those macros in AttributeSet.h. We'd do
	*  that in the constructor (use the setter is too early, so we use initter) like: 
	*  InitHealth(100.f);
	* To verify the values have been set, we can open the editor, click on the viewport, ~, and write showdebug abilitysystem.
	*/

	/** Attributes */
	// HEALTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	// MANA
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/** OnRep functions */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
