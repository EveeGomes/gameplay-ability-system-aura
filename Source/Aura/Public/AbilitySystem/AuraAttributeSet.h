// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

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
	*/
	
	// Register variables for replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Attributes */
	// HEALTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	// MANA
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;


	/** OnRep functions */
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
