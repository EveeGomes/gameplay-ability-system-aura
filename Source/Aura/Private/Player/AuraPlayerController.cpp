// Copyright Eveline Gomes.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"


void AAuraPlayerController::BeginPlay()
{
   Super::BeginPlay();

   /**
   * Add the Mapping Context.
   * The IMC, AuraContext, will be set in BP. So we need to check in case this pointer isn't set for some reason, which makes it invalid!
   *  That could cause some issues like the input not working, which is enough for stopping execution. Well, that's a great reason for setting
   *  the IMC on the player controller! Also, the game shouldn't continue if input isn't working, and for that we can use an assert!
   * So we'll use the check macro and send the IMC ptr. That will halt execution in case the condition fails: if the ptr hasn't been set yet,
   *  it'll be evaluated as false! This way we assert that aura context is set and if trying to run the game without setting it, we'll get a crash.
   * 
   */
   check(AuraContext);

   /** 
   * For adding an IMC, we need to get access to the Enhanced Input Local Player subsystem. We can get a pointer of this type, and set it 
   *  through a static function that belongs to ULocalPlayer class.
   * Subsystems are singletons, meaning it exists only ONE for the duration of the program!
   * Instead of using an if to check if Subsystem is valid, we'll use an assertion again like we did with AuraContext. The difference between
   *  using if and an assertion is that we'll get a crash if Subsystem is null.
   * https://dev.epicgames.com/documentation/en-us/unreal-engine/asserts-in-unreal-engine?application_version=5.3
   * 
   * Then, we proceed to add the IMC. Since we only have one, we'll have priority as 0.
   */

   UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
   check(Subsystem);
   Subsystem->AddMappingContext(AuraContext, 0);
   
}

AAuraPlayerController::AAuraPlayerController()
{
   /** Changes will be sent to all clients in the same server. It'll be addressed better later.*/
   bReplicates = true;
}
