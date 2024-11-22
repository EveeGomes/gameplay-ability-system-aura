// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * MaxMana will be set according to the calculation returned by a function in this class.
 * The value returned will be used by the modifier for MaxMana, just like MaxHealth.
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();
	
	/** Begin UGameplayModMagnitudeCalculation */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	/** End UGameplayModMagnitudeCalculation */

private:
	// Attribute to be captured
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
