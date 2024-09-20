// Copyright Eveline Gomes.


#include "Actor/AuraEffectActor.h"

/** Components */
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	// Construct the sphere and instead of making it the root or attaching it to the root, make a static mesh so it becomes the root and 
	//  then attach the sphere to it. That way if the mesh moves, the sphere moves with it.
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}


