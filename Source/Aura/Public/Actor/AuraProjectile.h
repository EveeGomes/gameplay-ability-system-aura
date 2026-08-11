// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

/**
 * This should be an object that can hit something, so overlap events must be implemented.
 *	On idea is to add the volume on BP and have BP callable functions here in C++ so they can be called there.
 *	Another idea is to add the logic here if we know we're only gonna use a sphere instead of another specific format,
 *	like a box or capsule.
 *
 * Since it's a projectile, and it should move as it's flying, we'll make use of the Projectile Movement Component, also
 *  recommended for multiplayer. 
 */
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
protected:
	virtual void BeginPlay() override;

	/* The Sphere component will bind to it */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
