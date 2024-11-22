// Copyright Eveline Gomes.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

/** Capture attributes */
#include "AbilitySystem//AuraAttributeSet.h"

/** Interface to get the player level */
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
   // Capture the attribute
   IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
   IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
   IntelligenceDef.bSnapshot = false;

   // Add the configured attribute to MMC array of captured attributes
   RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
   // Gather tags from source and target
   const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
   const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

   // Create an aggregator evaluate parameter to set the source and target tags
   FAggregatorEvaluateParameters EvaluationParameters;
   EvaluationParameters.SourceTags = SourceTags;
   EvaluationParameters.TargetTags = TargetTags;

   // Get the attribute's magnitude value and store in a local variable passed to the function to be filled
   float Intelligence = 0.f;
   GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
   // "Clamp it", make sure it's not negative
   Intelligence = FMath::Max<float>(Intelligence, 0.f);

   // Get the player level from CombatInterface by casting the source object to a combat interface object
   // The pointer won't be checked because we want the game to crash if the applied GE that uses this calculation doesn't implement a Combat Interface
   ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
   const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

   return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
}
