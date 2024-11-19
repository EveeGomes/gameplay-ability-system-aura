// Copyright Eveline Gomes.

/** To capture attributes */
#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

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


}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
   
}
