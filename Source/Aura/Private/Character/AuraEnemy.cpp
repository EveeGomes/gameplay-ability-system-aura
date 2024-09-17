// Copyright Eveline Gomes.


#include "Character/AuraEnemy.h"

/** Macros */
#include "Aura/Aura.h"

/** GAS classes */
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
   PrimaryActorTick.bCanEverTick = true;

   // Set the collision response for the mesh
   GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

   // Construct AuraAbilitySystemComponent
   AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
   // Make sure it is replicated
   AbilitySystemComponent->SetIsReplicated(true);

   // Construct AttributeSet
   AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
   // Set Render Custom Depth so the mesh uses the material we've added to the post process volume
   GetMesh()->SetRenderCustomDepth(true);
   GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); // once we set it, we don't need to set it again so it's a redundent operation (but it's cheap so we'll leave it)
   // Since weapon is created in the parent class (AuraCharacterBase) we don't expect Highlight being called before Weapon is a valid ptr
   Weapon->SetRenderCustomDepth(true);
   Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

}

void AAuraEnemy::UnHighlihtActor()
{
   GetMesh()->SetRenderCustomDepth(false);
   Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}
