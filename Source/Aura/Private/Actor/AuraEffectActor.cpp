// Copyright Eveline Gomes.


#include "Actor/AuraEffectActor.h"

/** Components */
#include "Components/SphereComponent.h"

/** To cast and check in overlap callbacks */
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

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

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// Check if the other actor (Target in this case) implements the ability system interface so we can get its ability system.
}
