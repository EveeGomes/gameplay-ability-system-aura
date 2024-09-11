// Copyright Eveline Gomes.


#include "Character/AuraCharacterBase.h"


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

