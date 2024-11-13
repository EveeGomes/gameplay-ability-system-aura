// Copyright Eveline Gomes.
#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the skeletal mesh component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); // FName() doesn't need TEXT() macro, but FString does since it takes a string literal like that and makes a implicit conversion to a wide character string.
	// Attach the weapon to a socket on the mesh
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	// Remove any collision from the weapon 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitilizePrimaryAttributes() const
{
	/** 
	* If we're gonna call ApplyGameplayEffectSpecToSelf or ToTarget or something similar, we'll need a GameplayEffectSpec to apply.
	* So, we'll access that function through the ASC, and create a GameplayEffectSpec 
	* (https://github.com/tranek/GASDocumentation/blob/master/README.md#459-gameplay-effect-spec) to pass to it as one of its params.
	* At the end we'll have a GameplayEffectSpec to a target ASC (the one we specify).
	* 
	* Since we're using an ASC and a UClass pointer (DefaultPrimaryAttributes), we should check assert them to make sure we're not using nullptr
	* 
	* Now that we have this function, where do we call it? It'll depend on the circumstances, if we're in AuraCharacter or AuraEnemy for example.
	* In AuraCharacter, we'd only need to apply this GE on the server side. That's because all those attributes are marked to be replicated, so if we
	*  change them on the server, they'll change on the clients as well! But, if we wanted to add it to the client side so it wouldn't have to wait
	*  for the values to be replicated, that's fine too... both ways would work just fine! Therefore, this function will be called in 
	*  InitAbilityActorInfo() (which is called in both server and client sides). Then, all we need is to make sure we have the GE and that we
	*  have configured some default values (we'll do it on BP side, in L66).
	*/
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	
	// Make a ContextHandle to be used when creating a GameplayEffectSpec
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	// Create a GameplayEffectSpec
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());

}
