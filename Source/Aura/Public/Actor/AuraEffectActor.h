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

/** 
* Effect application Enum: used to handle the GEs in this EffectActor class, depending on which ones are set. We'll need to decide how this class will apply and remove
*  the GEs. So we'd like to change how the actor applies these GEs from the way it is done in BP. For that we'll create a policy: an enum for the effect application and
*  the effect removal. In this policy we'll determine if the effects will be applied on overlap, or end overlap and so on.
* Create a scoped enum: EEffectApplicationPolicy (i.e. how we're going to apply the effect)
* Then, we can create a variable of this enum type for each of the GE (variable).
* 
* Create another scoped enum: EEffectRemovalPolicy that will be used to deal with how these effects will be removed. So in this case this enum only applies to an infinite
*  GE since a duration based GE is able to remove itself.
*/
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Used by GE that has duration policy as Has Duration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// To make an Infinite GE, this class should have an infinite GE TSubclassOf variable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};
