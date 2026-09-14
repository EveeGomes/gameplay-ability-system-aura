// Copyright Eveline Gomes.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/**
	 * The idea is to spawn the projectile on the server instead of locally, so it's a replicated actor.
	 * As the server spawns it, then the server will be in charge to move it, handle its location and more, while the
	 *  client will only see the replication of all those actions.
	 * Then, the projectile should only be spawned if we're in the server. How to know that?
	 *  In GameplayAbility.h, there's a method "HasAuthority" that returns true if this is the server or single player.
	 *  That's a way of finding out whether we're in the server but there are other ways as well.
	 *  HasAuthority asks for an arg, a pointer of type FGameplayAbilityActivationInfo. Since we have a param passed to
	 *  this function we're in, we can use it. But as it's not a pointer, we can send the address of ActivationInfo.
	 */

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	/**
	 * To spawn an actor we can use SpawnActor() method from World.
	 *  That's a template method where we can set any actor to be spanw. But in this case we want an spawned actor that
	 *  hits something and applies a GE, which in other words we're going to set a GE or GE spec on that actor. Then, if
	 *  we're going to set variables and things on the actor to be spawned, we can handle having all things set by the
	 *  time the actor has finished being spawned. For that we should use SpawnActorDeferred.
	 * The idea for the SpawnTransform values is that the projectile should spawn at the tip of the staff that's Aura's
	 *  weapon. But to do that we'd have to cast to AuraCharacter (because that's where we'd like to spawn it), however
	 *  that's not ideal - have this class depended on a AuraCharacter class. The idea is to be able to get the socket
	 *  location from the weapon of whomever owns this. Therefore, we'd need an interface function for this.
	 *	There's an interface, ICombatInterface that is used by both AuraCharacter and enemies. The AuraCharacterBase
	 *	inherits from that interface, so the idea is to add a function that returns a socket location, then derived
	 *	classes can override and have their own implementation.
	 */

	FTransform SpawnTransform;
	GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	
}
