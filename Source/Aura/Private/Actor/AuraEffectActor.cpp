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

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	// Construct the sphere and instead of making it the root or attaching it to the root, make a static mesh so it becomes the root and 
	//  then attach the sphere to it. That way if the mesh moves, the sphere moves with it.
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/** 
	* Change one of the attributes in the AuraAttributeSet (for which we should actually be doing using Gameplay Effects!). As for now, 
	*  Lesson 27 Effect Actor, we'll be learning an approach that isn't efficient, but will show how to access other actor's ASC, and then
	*  we'll learn how to do it with the Gameplay Effect which gives us more freedom and flexibility.
	* 
	* So, starting with it we'll cast the other actor to see if it implements the interface that contains the AS, ie we don't need to care
	*  about the type of the other actor (whether it's a character, a pawn etc).
	* Then, if the cast succeeds, we find the AttributeSet on that ASC, which is of of type AuraAttributeSet (that's a limited approach because 
	*  we care about the type of the AttributeSet, while when using Gameplay Effect there's no need to care what type is the AttributeSet in the
	*  ASC when we want to change a certain value in an attribute).
	*  For that we have to use the pointer of AbilitySystemInterface type and call the pure virtual function GetAbilitySystemComponent() function.
	*  As we know this will return a valid pointer, because we didn't implement this function in any actor that should return a nullptr, we won't
	*  check it. Then, with that call, we'll get the ASC, so we can call a getter for the attribute set.
	*  The function we use, GetAttributeSet() asks for a TSubclassOf of type UAttributeSet in order to limit which type of AttributeSet to look for.
	*  So we pass as Name_of_the_class::StaticClass(). Once that whole expression is evaluated, it'll return an object of type UAttributeSet, therefore
	*  we'll have to cast to UAuraAttributeSet. The pointer must be const as GetAttributeSet returns a const pointer.
	* Now we have the other actor's AuraAttributeSet, if it has one.
	* We know that in our game every actor that implements the AbilitySystemInterface, has an AuraAttributeSet, so we won't check that pointer either.
	* With that AuraAttributeSet pointer we'll have access to the attributes thanks to the boilerplate attribute accessors macro! However, we'll have
	*	to cast away the constness of AuraAttributeSet pointer ie remove the 'constness' in order to change the attribute (which is something we 
	*  shouldn't be doing, for it breaks encapsulation. Also, GetAttributeSet returns a const pointer for a reason! And that's why we gotta 'break'
	*  the rule by removing the const).
	*/

	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		MutableAuraAttributeSet->SetMana(AuraAttributeSet->GetMana() - 10.f);
		Destroy();
	}

}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind callback functions to their dynamic multicast delegates
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}


