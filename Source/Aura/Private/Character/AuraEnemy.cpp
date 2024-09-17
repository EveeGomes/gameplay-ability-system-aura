// Copyright Eveline Gomes.


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

void AAuraEnemy::HighlightActor()
{
   // Set Render Custom Depth so the mesh uses the material we've added to the post process volume
   GetMesh()->SetRenderCustomDepth(true);
   GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); // once we set it, we don't need to set it again so it's a redundent operation (but it's cheap so we'll leave it)
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

AAuraEnemy::AAuraEnemy()
{
   PrimaryActorTick.bCanEverTick = true;
}
