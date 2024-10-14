// Copyright Eveline Gomes.


#include "Actor/AuraEffectActor.h"

/** GAS Library */
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	/** 
	* Both Static mesh and the Sphere we'll be added in the designer domain, i.e. on Blueprint side, instead of here in C++. That's because the
	*  visual representation of the actor (static mesh) and the shape of overlap sphere could be determined by designers (works more on the BP side).
	*  More technical details can be determined here in C++.
	* 
	* As we remove the static mesh and sphere from C++, we'll also remove the overlap callback functions and the binding lines. We'll do that in order
	*  to give the designers side some control on how this actor should work. We should make this actor flexible and easily configurable on the BP side.
	*  So, if we want to add a mesh, or box, or sphere component, we can do that and add it to the root component. For that we'll set the root component
	*  to a USceneComponent created here:
	*/

	// Create a USceneComponent and assign it to the root component.
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	// For Infinite Effect, we should check both Application and Removal policies!
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	// Remove the GE from the ASC that we linked up with (when we added the key-value pair of effect handles to ASCs - when we apply the 
	//  effect in ApplyEffectToTarget)
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		/** 
		* We'll loop through our map to check if there's an ASC linking an active effect to the ASC that we're dealing with in this function!
		* For that we'll create a local TargetASC pointer from the TargetActor passed to this function. Then we'll use it to search through
		*  the map.
		* Once we find, we should use the ASC to remove the GE. And by doing that we should also remove that pair from the map! However, we can't 
		*  remove an element from the container we're currently looping through... that would cause a crash! So, we could have some sort of container
		*  of key-value pairs that we'd like to remove after we're done with the for loop. And while we find the and remove the active GE, we'd add it
		*  to this container. We'll use a TArray of the type we'd like to remove.
		*/
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return; // only proceed if it's valid, otherwise return

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		// Now remove the element from our map by key (because the key is the ActiveGameplayEffectHandle!)
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/** 
	* Another option for getting the ASC of other actor is using static functions from static classes. GAS has their own library we could use
	*  and it has a function that would cast an actor to the ASC interface or it would search for a component in the actor with ASC interface.
	* Then, once we have the target ASC, we can apply the gameplay effect!
	* To safeguard our code, we'll check the Actor pointer so this function won't do anything in case something that doesn't have an ASC  
	*  overlaps with the volume. And since we don't want to crash our program, we'll use an if() check.
	* On the other hand, the GameplayEffectClass needs to be set! So in that case we'll use check() and if it's null we'll get a crash intentionally.
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
	* So, as soon as the TSubclassOf<UGameplayEffect> is set to some GameplayEffect (done in BP), this function ApplyEffectToTarget() should work.
	* We can call this function from BP as we have our Sphere (or any other collision component) setup there. And we'll call it when overlapping.
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
	* Now, we'll also store the returned FActiveGameplayEffectHandle in a local variable that will be linked in a map we'll create, to the ASC 
	*  of the target actor (instead of the target actor to then get the ASC... we have the ASC already). That link will be made only if the duration 
	*  policy is infinite.
	* The map will map ActiveGameplayEffectHandles to ASC pointers (key, value)!
	*/
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	/** 
	* In order to check the GE duration policy, we can use the EffectSpecHandle since it wrappes the GE.
	* Once we find the Duration policy to be infinite, we need to store this infinite effect somehow. So, to store it, we can use the return value from 
	*  ApplyGameplayEffectSpecToSelf, which is an FActiveGameplayEffectHandle that has the GE! And besides storing it we should also link it up with 
	*  the target actor!
	* To have two values related to each other, we can use a map! :)
	* 
	* Now, we should only add the ActiveEffectHandle and the ASC pointer of the target actor, if we plan to remove the effect, if not we shouldn't even
	*  store it. i.e. we should check the state of EEffectRemovalPolicy type variable! So, in addition to checking bIsInfinite, we should also check
	*  this state.
	* Then, in the function OnEndOverlap() we'll remove the GE! While in OnOverlap we apply the effect.
	*/
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}
