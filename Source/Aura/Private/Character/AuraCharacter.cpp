// Copyright Eveline Gomes.


#include "Character/AuraCharacter.h"

/** Components */
#include "GameFramework/CharacterMovementComponent.h"

/** Setting up ASC */
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

/**  */
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
   /** 
   * Setup parameters so this character behaves as we want/need it to. We'll use typical parameters for a Top Down game.
   */

   GetCharacterMovement()->bOrientRotationToMovement = true;
   // Control the rotation rate. Our character will orient towards movement with 400. speed in the Yaw rotational direction
   GetCharacterMovement()->RotationRate = FRotator{ 0., 400., 0. };
   // CharacterMovement can constraint our movement to a plane
   GetCharacterMovement()->bConstrainToPlane = true;
   // Make sure the character is snapped to a plane when the game starts
   GetCharacterMovement()->bSnapToPlaneAtStart = true;
   
   // Our character itself shouldn't use the controller rotation
   bUseControllerRotationPitch = false;
   bUseControllerRotationRoll = false;
   bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
   Super::PossessedBy(NewController);

   InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
   Super::OnRep_PlayerState();

   InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
   AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
   check(AuraPlayerState);
   // Tell ASC who are the Owner Actor, and Avatar Actor
   AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
   // Call AbilityActorInfoSet() from AuraAbilitySystemComponent class, so it knows the ASC has been set!
   Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

   // Set ASC and AttributeSet pointers
   AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
   AttributeSet = AuraPlayerState->GetAttributeSet();

   /** 
   * Here we know that the player controller has been set already. We also have here the player state, ASC and AS been set. Therefore this is
   *  a great place to call InitOverlay() so it construct the widget and set the widget controller. For that we'll need access to AuraHUD! HUD is
   *  always accessable through the player controller, so we can use AuraPlayerController through a ptr.
   * A common question: should we check if AuraPlayerController ptr is null? Or do we know it'll never be null at this point? Should we use an if
   *  statement or an assertion?
   *  So only on the server, all the player controllers are valid, ie the server has all the players's player controllers. Each player however, has
   *  their own player controller, so on a client machine controlling a specific character, that player controller is valid, but the other characters
   *  on that client machine that aren't locally controlled, they don't have valid player controllers. So, in multiplayer game, that player controller
   *  pointer, can and will be null, and for that we need to check and only continue if it's not null. This is a place where we use an if statement
   *  because we want to continue instead of getting a crash.
   * Another place which something similar happens, is in AuraPlayerController BeginPlay(). So for multiplayer games, unless we're in our locally
   *  controlled machine that has a valid local player, we should check the subsystem instead of using an assert and proceed with adding the mapping
   *  context.
   * As we get the HUD (in this case our AuraHUD), we also need to check since it'll only be valid in the player's local machine.
   */
   if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
   {
      // Get and check AuraHUD
      if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
      {
         // Call InitOverlay() and pass the args as we now have them all initialized!
         AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
      }
   }
}