// Copyright Eveline Gomes.


#include "AuraGameplayTags.h"

/** Use the GT manager */
#include "GameplayTagsManager.h"

/**
 * @param GTVariable The gameplay tag variable name
 * @param GTName The last part of the gameplay tag after Attributes.Secondary separated by a dot(.) -> Attribute.Name
 * @param Description Any comment to help as a tooltip for that attribute
 */
#define ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(GTVariable, GTName, Description) \
	GameplayTags.GTVariable = UGameplayTagsManager::Get().AddNativeGameplayTag( \
		FName("Attributes.Secondary." #GTName), \
		FString(Description) \
	);

/**
 * @param GTVariable The gameplay tag variable name
 * @param GTName The last part of the gameplay tag after Attributes.Primary separated by a dot(.) -> Attribute.Name
 * @param Description Any comment to help as a tooltip for that attribute
 */
#define ASSIGN_AND_ADD_ATTRIBUTES_PRIMARY_GTS(GTVariable, GTName, Description) \
	GameplayTags.GTVariable = UGameplayTagsManager::Get().AddNativeGameplayTag( \
		FName("Attributes.Primary." #GTName), \
		FString(Description) \
	);

/** For a static variable, we need to explicitly declare the type. We have to declare the variable fully qualified */
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * When a GT is created, it's registered with the gameplay tag manager (only one for the whole project).
	 * Get the GT manager by calling a getter function, that returns the one and only instance of that manager, since it's a singlenton!
	 * Then, we can have access to many function such as AddNativeGameplayTag that allows us to declare GTs.
	 *
	 * This method we're defining here, InitializeNativeGameplayTags(), should be called early on in the project.
	 *	It has to be as soon as we can before any of those tags are attempted to be used!
	 *  To achieve that, we'll create yet another singleton, our version of an asset manager that'll have to do with loading
	 *	primary assets, and it's where we can initialize this GTs.
	 *
	 *	After creating a FGameplayTag variable we need to associate the Tag we're creating here with that variable.
	 *	 We store the GT returned by AddNativeGameplayTag in their corresponding variable. However, we need to use the
	 *	 GameplayTags only object of this class since the current function we're in is static. So, we need to store the
	 *	 GT into this object's variables: GameplayTags.Variable = GT returned by AddNativeGameplayTag.
	 *	 Now, with those GTs stored in their respective variables, they can be accessed by other classes!
	 */

	ASSIGN_AND_ADD_ATTRIBUTES_PRIMARY_GTS(Attributes_Primary_Strength,
		Strength, "Increases physical damage");
	ASSIGN_AND_ADD_ATTRIBUTES_PRIMARY_GTS(Attributes_Primary_Intelligence,
		Intelligence, "Increases magical damage");
	ASSIGN_AND_ADD_ATTRIBUTES_PRIMARY_GTS(Attributes_Primary_Resilience,
		Resilience, "Increases Armor and Armor Penetration");
	ASSIGN_AND_ADD_ATTRIBUTES_PRIMARY_GTS(Attributes_Primary_Vigor,
		Vigor, "Increases Health");

	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_Armor,
		Armor, "Reduces damage taken, improves Block Chance");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_ArmorPenetration,
		ArmorPenetration, "Ignored Percentage of enemy Armor, increases Critical Hit Chance");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_BlockChance,
		BlockChance, "Chance to cut incoming damage in half");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_CriticalHitChance,
		CriticalHitChance, "Chance to double damage plus critical hit bonus");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_CriticalHitDamage,
		CriticalHitDamage, "Bonus damage when a critical hit is scored");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_CriticalHitResistance,
		CriticalHitResistance, "Reduces Critical Hit Chance of attacking enemies");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_HealthRegeneration,
		HealthRegeneration, "Amount of Health regenerated every 1 second");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_ManaRegeneration,
		ManaRegeneration, "Amount of Mana regenerated every 1 second");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_MaxHealth,
		MaxHealth, "Maximum amount of Health obtainable");
	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_MaxMana,
		MaxMana, "Maximum amount of Mana obtainable");
}