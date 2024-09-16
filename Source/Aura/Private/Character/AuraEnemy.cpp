// Copyright Eveline Gomes.


#include "Character/AuraEnemy.h"

#include "DrawDebugHelpers.h"

void AAuraEnemy::HighlightActor()
{
   Highlighted = true;
}

void AAuraEnemy::UnHighlihtActor()
{
   Highlighted = false;
}

void AAuraEnemy::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   if (Highlighted)
   {
      UWorld* World = GetWorld();
      if (World)
      {
         DrawDebugSphere(World, GetActorLocation(), 50.f, 15, FColor::Red);
      }
   }
}

AAuraEnemy::AAuraEnemy()
{
   PrimaryActorTick.bCanEverTick = true;
}
