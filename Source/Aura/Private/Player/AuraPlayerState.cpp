// Copyright Eveline Gomes.


#include "Player/AuraPlayerState.h"

/** GetLifetimeReplicatedProps - macro to mark variables as replicated */
#include "Net/UnrealNetwork.h"

/** GAS classes */
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
   /** 
   * Construct GAS related pointers:
   */
   // Construct AuraAbilitySystemComponent
   AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
   // Make sure it is replicated
   AbilitySystemComponent->SetIsReplicated(true);
   /** 
   * Gameplay Effects are used to change attributes values, i.e we use them to cause damage, or heal the player. So if those changes happen
   *  on the server, the client should know about it, the client would like to show in the HUD etc.
   * 
   * The replication mode determines how these gameplay effects will be replicated. There are 3 modes: full, mixed and minimal.
   * Full -> replicate gameplay effects to all clients (use case: single player).
   * Mixed -> replicates only to the owning client, but gameplay cues and tags are replicated to all clients (multiplayer, player-controlled).
   * Minimal -> won't replicate effects, but gameplay cues and tags are replicated to all clients (multiplayer, AI-controlled).
   */
   // Set replication mode for this component
   AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

   // Construct AttributeSet
   AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

   /** 
   * Use NetUpdateFrequency to set how often the server tries to update clients. That's useful to sync up clients with the server version 
   *  as changes occur on ther server for the player state. So any variables that should be replicated, they'll update and the server will
   *  try to meet this net update frequency if it can.
   * Normally this net update frequency is low, but as we need to have our ability system component and atribute set in the player state,
   *  we should make this update faster, set it to a higher value! That's why we're using 100 here.
   */
   NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
   return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{

}
