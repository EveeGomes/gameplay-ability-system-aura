// Copyright Eveline Gomes.


#include "Actor/AuraEffectActor.h"

/** GAS Library */
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	/** 
	* Both Static mesh and the Sphere we'll be adding in the designer domain, i.e. on Blueprint side, instead of here in C++. That's because the
	*  visual representation of the actor (static mesh) and the shape of overlap sphere could be determined by designers (works more on the BP side).
	*  More technical details can be determined here in C++.
	* 
	* As we remove the static mesh and sphere from C++, we'll also remove the overlap callback functions and the binding lines. We'll do that in order
	*  to give the designers side some control on how this actor should work. We should make this actor flexible and easily configurable on the BP side.
	*  So, if we want to add a mesh, or box, or sphere component, we can do that and add it to the root component. For that we'll set the root component
	*  to a USceneComponent we created:
	*/

	// Create a USceneComponent and assign it to the root component.
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/** 
	* Another option for getting the ASC of other actor is using static functions from static classes. GAS has their own library we could use
	*  and it has a function that would cast an actor to the ASC interface or it would search for a component in the actor with ASC interface.
	* Then, once we have the target ASC, we can apply the gameplay effect!
	* To safeguard our code, we'll check the Actor pointer so this function won't do anything in case something that doesn't have an ASC  
	*  overlaps with the volume. And since we don't want to crash our program, we'll use an if() check.
	* Now, the GameplayEffectClass in other words needs to be set! So in that case we'll use check() and if it's null we'll get a crash intentionally.
	*/
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	/** 
	* There are many ways to apply gameplay effects.
	* One way is described in L38 @15:08.
	* We'll make use of the function ApplyGameplayEffectSpecToSelf that requires some parameters.
	* @GameplayEffect -> const reference of type FGameplayEffectSpec. For that we'll have to make one from the ASC. 
	* @PredictionKey -> default/optional param. Prediction has to do with lag compensation sense (which will be discussed later).
	* 
	* To create an FGameplayEffectSpec, We can use the Target ASC, since any ASC can make an effect spec given some gameplay effect class
	*  (which we have as param of our function). 
	*  Another param we'll need to pass is a float called Level since gameplay effects can have their 
	*  own levels, and as a game project gets bigger and more serious, more different levels are needed for everything.
	*  Following we'll have to pass a context, which is something a gameplay effect is closely related to, that we associate with the gameplay effect!
	*   Context meaning "what's the context of the effect?", "what's the context of the situation?", "who's causing the effect?, who's the target?",
	*   "is the effect a fire effect?, a lightning effect?" i.e. there's no end on how we can describe a gameplay effect and the context surrounding it!
	*  That param is of type FGameplayEffectContextHandle. Handle is a lightweight thing that contains the pointer to the context itself (it's a 
	*  wrapper!). So, to create that we'll use the ASC as it has a function for everything lol, called MakeEffectContext(). Once we have the handle,
	*  we can use its methods/functionalities/utility functions to do anything we want. (Check definition of FGameplayEffectContextHandle).
	* 
	* So, as soon as the TSubclassOf<UGameplayEffect> is set to some GameplayEffect, this function ApplyEffectToTarget() should work.
	* We can call this function from BP as we have our Sphere setup there. And we'll call it when overlapping.
	*/

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// Store what object cause this effect
	EffectContextHandle.AddSourceObject(this);
	// Create a FGameEffectSpecHandle so it can be passed in ApplyGameplayEffectSpecToSelf().
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	/** 
	* EffectSpecHandle wrappes the Data (like other handles!), the real FGameplayEffectSpec. That Data is yet another wrapper, a TSharedPtr of type
	*  FGameplayEffectSpec. So in ApplyGameplayEffectSpecToSelf, we need to pass a const reference of a FGameplayEffectSpec, and to get it we'll use
	*  the handle, access the Data, call its Get() function which will give us the raw FGameplayEffectSpec pointer and use * to finally dereference it!
	*/
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
