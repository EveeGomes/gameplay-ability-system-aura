// Copyright Eveline Gomes.


#include "Character/AuraEnemy.h"

#include "DrawDebugHelpers.h"

void AAuraEnemy::HighlightActor()
{
   Highlighted = true;

   // Set Render Custom Depth so the mesh uses the material we've added to the post process volume
   GetMesh()->SetRenderCustomDepth(true);
   GetMesh()->SetCustomDepthStencilValue(250);
   Weapon->SetRenderCustomDepth(true);
   Weapon->SetCustomDepthStencilValue(250);

}

void AAuraEnemy::UnHighlihtActor()
{
   Highlighted = false;
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
