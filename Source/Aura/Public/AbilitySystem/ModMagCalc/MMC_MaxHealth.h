// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * MaxHealth will be determined by the calculations created in this class!
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	/** Begin UGameplayModMagnitudeCalculation */
	/**
	* This function returns a float which is the result that our modifier should produce. So all the calculations that result in that value, we'll
	*  determine in this function!
	* The param @Spec is the Effect Spec of whatever GE that has a modifier using this custom calculation. I.E. as we have access to that GE spec,
	*  we have access to all information inside of that.
	*/
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	/** End UGameplayModMagnitudeCalculation */

private:
	/** 
	* ModMagCalc classes are also able to capture attributes. To do that we have to create a variable for the attribute we want to capture, and the
	*  type is FGameplayEffectAttributeCaptureDefinition. That attribute will be capture in this class constructor.
	*/
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
