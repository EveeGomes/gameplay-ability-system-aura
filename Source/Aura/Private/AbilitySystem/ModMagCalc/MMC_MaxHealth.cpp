// Copyright Eveline Gomes.

#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

/** To capture attributes */
#include "AbilitySystem/AuraAttributeSet.h"

/** Interface */
#include "Interaction/CombatInterface.h"



UMMC_MaxHealth::UMMC_MaxHealth()
{
   /** 
   * Capture attribute. Access the attribute in AuraAttributeSet using a static function from the Attribute_Accessors macro. That macro allows
   *  the creation of some functions and one of them is a property getter function resulting in GetAttributeNameAttribute(), which is static so
   *  we won't need a pointer to a UAuraAttributeSet object to access that function and get the attribute itself.
   * 
   * Then, we need to define if we're capturing the attribute from the target or the source. In our case we're applying a GE from the AuraCharacter
   *  to the Auracharacter, so they're both the same. However we're specifying it here. We can find the same thing in the GE but here in C++ we use
   *  an enum.
   * We should also define whether or not we'll use snapshot. Snapshot has to do with timing... when do we want to capture the attribute: when the
   *  GE Spec is created or when the GE Spec is applied? Well, in this case these things happens one after the other, immediately so those settings
   *  won't matter. Snapshotting is capturing the attribute right way, as soon as the GE Spec is created. Not snapshotting is getting the true value
   *  at the time of the application of the effect.
   *  Since in this case it doesn't matter, we'll set it to false.
   */
   VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
   VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
   VigorDef.bSnapshot = false;

   /** 
   * Now, our Modifier Magnitude Calculation needs one of its variables, which is an array of attributes to capture, and we need to add VigorDef to it.
   * This way, when the modifier is executed, when the effect is applied, we'll be sure to have VigorDef captured from the target at the time of
   *  application. Then, we can acces it in CalculateBaseMagnitude_Implementation().
   */
   RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
   /** 
   * Here we can get access to GT if we want and they can affect things if we also want to.
   */
   // Gather tags from source and target
   const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
   const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

   /** 
   * Now, in order to capture an attribute and get that attribute's magnitude value, we have to create an FAggregatorEvaluateParameters.
   * These are parameters that we have to pass in to a specific function in order to capture the attribute (in our case, VigorDef).
   * We get that captured value with the function GetCapturedAttributeMagnitude().
   * 
   * This function takes 4 args: 
   *  @the captured attribute we've set in the contructor (VigorDef);
   *  @GE Spec which is passed to this function we're in;
   *  @the evaluation parameters
   *  @a float magnitude which is gonna be filled with the value of the vigor attribute on the target! So we'll create a float and pass it in.
   * 
   * Then, we'll clamp that attribute's value and a trick is to use Max<>() function from FMath to guarantee the value won't go below zero.
   */
   FAggregatorEvaluateParameters EvaluationParameters;
   EvaluationParameters.SourceTags = SourceTags;
   EvaluationParameters.TargetTags = TargetTags;

   float Vigor = 0.f;
   GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
   Vigor = FMath::Max<float>(Vigor, 0.f);

   /** 
   * Now that we have Vigor, we need to get the player level as we want MaxHealth to be dependent not only on Vigor but also on the player's level.
   * The purpose of creating the CombatInterface is so we can cast this GE source object to it. We can always get the source object of this GE,
   *  from the Spec. In our case, the source object is going to be AuraCharacter.
   * Once we have the CombatInterface pointer set, we can get the player level.
   */

   ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
   const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

   /** 
   * With both Vigor and Player level, we can finally decide what this function returns for this modifier calculation.
   * The design chosen is to have a base value of 80.f plus 
   *  the Vigor multiplied by 2.5 (so for every Vigor point we get 2.5) plus 
   *  ten times the player level (for every PlayerLevel we get ten).
   * This way, as the player levels up, maybe they gain some points that they can spend on their attributes and they upped their vigor.
   * If they spend one point and up the vigor by one, then their MaxHealth will go up by 2.5. If the player levels up, their level goes from 1 to 2,
   *  then they get ten more added to their MaxHealth.
   */
   return 80.f + 2.5 * Vigor + 10.f * PlayerLevel;
}
