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

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance.")
	);

	GameplayTags.Attributes_Secondary_Armor_Penetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor.Penetration"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Block_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Critical_Hit_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.HitChance"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Critical_Hit_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.HitDamage"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Critical_Hit_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.HitResistance"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Health_Regeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Health.Regeneration"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Mana_Regeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Mana.Regeneration"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Max_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Max.Health"),
		FString("")
	);

	GameplayTags.Attributes_Secondary_Max_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Max.Mana"),
		FString("")
	);

	ASSIGN_AND_ADD_ATTRIBUTES_SECONDARY_GTS(Attributes_Secondary_Max_Mana, Max.Mana);
}