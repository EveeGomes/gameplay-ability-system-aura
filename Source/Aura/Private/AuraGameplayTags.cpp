// Copyright Eveline Gomes.


#include "AuraGameplayTags.h"

/** Use the GT manager */
#include "GameplayTagsManager.h"

/**
 * @param GTVariable The gameplay tag variable name
 * @param AttributeOrder If it's either primary or secondary attribute
 * @param GTName The last part of the gameplay tag after Attributes.Secondary separated by a dot(.) -> Attribute.Name
 * @param Description Any comment to help as a tooltip for that attribute
 */
#define ASSIGN_AND_ADD_ATTRIBUTES_GTS(GTVariable, AttributeOrder, GTName, Description) \
	GameplayTags.GTVariable = UGameplayTagsManager::Get().AddNativeGameplayTag( \
		FName("Attributes" "." #AttributeOrder "." #GTName), \
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

	/**
	 * Primary Attributes
	 */
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Primary_Strength, Primary,
		Strength, "Increases physical damage");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Primary_Intelligence, Primary,
		Intelligence, "Increases magical damage");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Primary_Resilience, Primary,
		Resilience, "Increases Armor and Armor Penetration");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Primary_Vigor, Primary,
		Vigor, "Increases Health");
	
	/**
	 * Secondary Attributes
	 */
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_Armor, Secondary,
		Armor, "Reduces damage taken, improves Block Chance");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_ArmorPenetration, Secondary,
		ArmorPenetration, "Ignored Percentage of enemy Armor, increases Critical Hit Chance");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_BlockChance, Secondary,
		BlockChance, "Chance to cut incoming damage in half");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_CriticalHitChance, Secondary,
		CriticalHitChance, "Chance to double damage plus critical hit bonus");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_CriticalHitDamage, Secondary,
		CriticalHitDamage, "Bonus damage when a critical hit is scored");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_CriticalHitResistance, Secondary,
		CriticalHitResistance, "Reduces Critical Hit Chance of attacking enemies");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_HealthRegeneration, Secondary,
		HealthRegeneration, "Amount of Health regenerated every 1 second");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_ManaRegeneration, Secondary,
		ManaRegeneration, "Amount of Mana regenerated every 1 second");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_MaxHealth, Secondary,
		MaxHealth, "Maximum amount of Health obtainable");
	ASSIGN_AND_ADD_ATTRIBUTES_GTS(Attributes_Secondary_MaxMana, Secondary,
		MaxMana, "Maximum amount of Mana obtainable");

	/**
	* InputAction Tags
	*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
	);
}