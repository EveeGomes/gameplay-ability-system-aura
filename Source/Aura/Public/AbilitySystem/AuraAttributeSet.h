// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

/** Components; Access to macros */
#include "AbilitySystemComponent.h"

#include "AuraAttributeSet.generated.h"

/** Attribute Accessors macro definition */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	FGameplayEffectContextHandle EffectContextHandle;

	// SOURCE variables
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	// TARGET
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;


};


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
	*  FGameAttributeData. When the variable is replicated (it'll hold its new replicated value), OnRep is called and if it has the arg, �t'll hold the 
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
	//  returned from querying the modifier. Later operations recalculate the current value from all modifiers, so we'll need to clamp again.
	// https://github.com/tranek/GASDocumentation/blob/master/README.md#445-preattributechange
	// Removed clamping: https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39784058#questions/20594972
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

	/**
	* Primary Attributes
	*/
	/** 
	* To initialize these attributes using a data table, we need to expose the ASC to BP so we can set a specific asset, a data table, on that component.
	* So, in AuraPlaterState, where our ASC variable is, we'll add VisibleAnywhere specifier to its UPROPERTY!
	* 
	* The other way (and preferable way) is to initialize the attributes using a gameplay effect (GE). We'll do that through a function that'll
	*  apply this GE.
	* The function will be added on the character, AuraCharacterBase. It can have some sort of default primary attributes GE.
	*/
	
	// STRENGTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	// INTELLIGENCE
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
	
	// RESILIENCE
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
	
	// VIGOR
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	/** 
	* Secondary Attributes
	*/
	// ARMOR - Resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	// ARMOR PENETRATION - Resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	// BLOCK CHANCE - Armor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);
	
	// CRITICAL HIT CHANCE - Armor Penetration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
	
	// CRITICAL HIT DAMAGE - Armor Penetration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);
	
	// CRITICAL HIT RESISTANCE - Armor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);
	
	// HEALTH REGENERATION - Vigor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
	
	// MANA REGENERATION - Intelligence
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);
	
	// MAX HEALTH - Vigor and player Level
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	// MAX MANA - Intelligence and player Level
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/** 
	* Vital Attributes 
	*/
	// HEALTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	// MANA
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	/** 
	* OnRep functions - used to inform the ability system that the attribute has just been replicated
	*/
	// Primary Attributes
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	// Secondary Attributes
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	// Vital Attributes
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	/** Fill in the data in the FEffectProperties */
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
