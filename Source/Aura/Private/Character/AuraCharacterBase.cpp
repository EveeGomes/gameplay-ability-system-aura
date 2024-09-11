// Copyright Eveline Gomes.


#include "Character/AuraCharacterBase.h"


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the skeletal mesh component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); // FName() doesn't need TEXT() macro, but FString does since it takes a string literal like that and makes a implicit conversion to a wide character string.


}

