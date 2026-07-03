// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"

/** To use the GameplayTag type */
#include "GameplayTagContainer.h"

/**
 * This class is used to store the GTs. This results in storing the GTs in a centralized location. So it'll be our singleton for GTs.
 * It'll actually be changed to a struct, so the class declaration has been deleted, and we declare now a struct instead.
 * In summary, this is a Singleton containing native Gameplay Tags (native in a sense that they're created from within C++ and available in C++. 
 *  They'll be available in the editor/BP as well!).
 * 
 * Since it's a singleton, we have to create a static getter function, so we don't need an instance of this class to exist to be able to call
 *  one of its static functions! It'll return the only instance of AuraGameplayTags which is a static variable on this class/struct. That'll be a
 *  private static FAuraGameplayTags variable. So, once one of FAuraGameplayTags is constructed, then we can just call the static function Get() to
 *  get the one and only instance of gameplay tags.
 */

struct FAuraGameplayTags
{
public:
   /** Returns a const reference to the one and only instance of AuraGameplayTags that'll exist in the game. */
   static const FAuraGameplayTags& Get() { return GameplayTags; }

   /** Initialize (create) the GTs. */
   static void InitializeNativeGameplayTags();
   
   /** Gameplay Tags Variables */

   ///////////////////////// Primary Attributes /////////////////////////
   FGameplayTag Attributes_Primary_Strength;
   FGameplayTag Attributes_Primary_Intelligence;
   FGameplayTag Attributes_Primary_Resilience;
   FGameplayTag Attributes_Primary_Vigor;
   ///////////////////////// Primary Attributes /////////////////////////
   
   ///////////////////////// Secondary Attributes /////////////////////////
   FGameplayTag Attributes_Secondary_Armor;
   FGameplayTag Attributes_Secondary_ArmorPenetration;
   FGameplayTag Attributes_Secondary_BlockChance;
   FGameplayTag Attributes_Secondary_CriticalHitChance;
   FGameplayTag Attributes_Secondary_CriticalHitDamage;
   FGameplayTag Attributes_Secondary_CriticalHitResistance;
   FGameplayTag Attributes_Secondary_HealthRegeneration;
   FGameplayTag Attributes_Secondary_ManaRegeneration;
   FGameplayTag Attributes_Secondary_MaxHealth;
   FGameplayTag Attributes_Secondary_MaxMana;
   ///////////////////////// Secondary Attributes /////////////////////////
   
protected:
   
private:
   /** One and only instance of this struct. */
   static FAuraGameplayTags GameplayTags;
};

