// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

/** 
* For now this class will be used only to demonstrate some limitations of not using Gameplay Effects to apply modifications to attributes.
* So we'll implement some overlap and then make modifications to attributes.
*/

/** Forward Declarations */
class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	// Create a GameplayEffect in BP, use the Sphere created there and use this function
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	/** 
	* We'll create a function to apply an effect to the other actor that overlaps with this actor. For that we'll create a GameplayEffect variable
	*  that can be accessed from BP, and is TSubclassOf. That will represent the class of the instant gameplay effect we want to apply.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// Used by GE that has duration policy as Has Duration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
};
