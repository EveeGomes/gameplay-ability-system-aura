// Copyright Eveline Gomes.


#include "AuraGameplayTags.h"

/** Use the GT manager */
#include "GameplayTagsManager.h"

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
	 */

	UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance.")
		);

	
}
