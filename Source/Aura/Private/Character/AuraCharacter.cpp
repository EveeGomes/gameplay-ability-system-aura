// Copyright Eveline Gomes.


#include "Character/AuraCharacter.h"

/** Components */
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
   /** 
   * Setup parameters so this character behaves as we want/need it to. We'll use typical parameters for a Top Down game.
   * 
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
