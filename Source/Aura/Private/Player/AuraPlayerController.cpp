// Copyright Eveline Gomes.


#include "Player/AuraPlayerController.h"

/** Input */
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

/** Interfaces */
#include "Interaction/EnemyInterface.h"


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
   /** 
   * Since our Move Input Action is an axis 2D type, we can retrieve the X and Y axes data from it.
   * We use the Get<>() function which is a template function that allows us to get the value in the form we need it in.
   * Once we have a FVector2D extracted from the InputActionValue, we can access the X and Y axes to add movement input.
   * ie: add movement to our controlled pawn in the forward direction and right direction, based on those axes values.
   * 
   * We need to find out which direction is forward for us, and we need to do that based on our controller, because the controller
   *  is pointed basically in the direction from the camera to the character (parellel to the ground).
   * So, we can access the controller rotation to zero out the pitch(y) and roll(x), and have the yaw(z) value for our controller rotation.
   * If we convert that to a vector, then we can consider that as forward as far as WASD keys are concerned!
   * We do all that to have a rotation we can use to get a forward vector from. And we do that by using an FRotationMatrix that we initialize
   *  with the FRotator we created and then call a GetUnitAxis passing the X axis that gives us the forward axis!
   * We can do the same but passing the Y axis to get the right axis, which gives us the right direction!
   * 
   * These values are normalized, as GetUnitAxis returns a unit vector which has a length of one.
   * 
   * FROM CHATGPT:
   * Why Use This?
   * You're using this to move or determine directions in the world based on where the player is looking. By isolating the yaw component, 
   *  you ensure that "forward" and "right" are always relative to the player's view, without the complications of looking up or down (pitch) 
   *  or tilting (roll). 
   *  This helps make movement controls feel intuitive and consistent with the camera orientation.
   * 
   * Once we have those directions, we can add movement input to the controlled pawn.
   * To get the controlled pawn by this player controller class, we'll wrap the code in a if statement because Move is pottentially called 
   *  every frame and it might be too early before the controlled pawn is valid.
   * The AddMovementInput() needs a world direction and a scale value to change the world direction accordingly. We already receive that
   *  scale value from the InputActionValue, eg if we press W it'll be positive 1 but if we press S it'll be negative 1, and these keys
   *  correspond to data in the Y axis. So, all we have to do is pass the InputAxisVector.Y for scaling the forward direction. 
   *  Similarly, for adding right and left movement, we'll use the RightDirection and scale it by the InputAxisVector.X since the A and D
   *  keys are associated with the right and left.
   */

   // Retrieve data from InputActionValue
   const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

   // Get a forward direction vector, and right direction vector
   const FRotator Rotation = GetControlRotation();
   const FRotator YawRotation{ 0., Rotation.Yaw, 0. };
   const FVector ForwardDirection = FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X);
   const FVector RightDirection = FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y);

   // Add movement input to the controlled pawn
   if (APawn* ControlledPawn = GetPawn<APawn>())
   {
      ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
      ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
   }
}

void AAuraPlayerController::CursorTrace()
{
   /** 
   * Get the hit result under the cursor. This is something that the PlayerController class inheritly has the ability to do.
   * We'll call a method that do that and takes a trace channel, which we'll pass the visibility channel. The bTraceComplex will be
   *  set to false since we want to trace against simple collision only.
   * To avoid logic being nested like:
   *  if (CursorHit.bBlockingHit) { //... }
   *  we'll do: if (!CursorHit.bBlockingHit) return;
   *  and then we continue with the code below it.
   * 
   * Now, we'll use the hit result to see if what was hit implements the EnemyInterface, by casting it to a EnemyInterface.
   *  If the cast fails it'll return a nullptr, if succeeds it returns a valid enemy. And we'll use these information!
   * We'll create 2 pointers: one to hold the actor we hovered over previous frame, and a second to hold the actor hovered over in current frame.
   * Since we're using TScriptInterface, we don't need to perform the cast, we can simply pass the get actor to the pointer directly!
   * 
   * There are many scenarios we need to take care of when we do the Line Trace from cursor:
   * A. LastActor is null && ThisActor is null
   *     - Do nothing.
   * B. LastActor is null && ThisActor is valid
   *     - Highlight ThisActor.
   * C. LastActor is valid && ThisActor is null
   * (we've hovered over a valid actor last frame, but this frame we hover a non valid, so we should unhighlight LastActor!)
   *     - Unhighlight LastActor.
   * D. Both actors are valid, but LastActor != ThisActor
   * (we're hovering over different enemies: last frame: 1 enemy, this frame: a different enemy. Meaning, LastEnemy should be unhighlighted,
   *  while ThisActor should be highlighted)
   *     - Unhighlight LastActor, and Highlight ThisActor.
   * E. Both actors are valid, and are the same actor
   *     - Do nothing.
   */
   FHitResult CursorHit;
   GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
   if (!CursorHit.bBlockingHit) return;

   LastActor = ThisActor;
   ThisActor = CursorHit.GetActor();

   if (LastActor == nullptr) // explicitly stating if that pointer is null
   {
      if (ThisActor != nullptr)
      {
         // Case B
         ThisActor->HighlightActor();
      }
      else
      {
         // Case A - both are null, do nothing
         // unnecessary else, but here to make it easier to visualize and understand at first
      }
   }
   else // LastActor is valid
   {
      if (ThisActor == nullptr)
      {
         // Case C
         LastActor->UnHighlihtActor();
      }
      else // both actors are valid
      {
         if (LastActor != ThisActor)
         {
            // Case D
            LastActor->UnHighlihtActor();
            ThisActor->HighlightActor();
         }
         else
         {
            // Case E - do nothing
         }
      }
   }
}

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
   * With a IMC, the player controller can retrieve data. For handling input here, in this class, we use SetupInputComponent().
   * If we choose to do it in a character class instead, we'd use the SetupInputPlayerComponent().
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

   /** 
   * There are some settings that player controller has control over, that we can set here in BeginPlay(). Some examples are: showing the
   *  mouse cursor, set the input mode etc.
   */
   // In this Top Down we want to see our cursor!
   bShowMouseCursor = true;
   // Set it using an enum with many different cursors
   DefaultMouseCursor = EMouseCursor::Default;

   // Set input mode using the following struct which allows for using input from the keyboard, mouse, and use input to affect UI like widgets!
   FInputModeGameAndUI InputModeData;
   // Configure it:
   InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // won't lock the mouse to the viewport
   InputModeData.SetHideCursorDuringCapture(false); // won't hide the cursor as soons as it's captured into the viewport
   // Now, in order to use this InputModeData, use a player controller function to set it:
   SetInputMode(InputModeData);  
}

void AAuraPlayerController::SetupInputComponent()
{
   /** 
   * This is where we can retrieve data.
   * We need to bind functions to the IA we created.
   * First we access a member variable of player controller class: InputComponent which is a pointer of type UInputComponent, but
   *  stores the address of an object of UEnhancedInputComponent type (we know that by going to the Editor > Project Settings > Under Engine 
   *  select Input, then under Default Classes we see the Default Input Component Class is set to EnhancedInputcomponent. This is where we set
   *  things up. If we wanted our custom EnhancedInput class, we'd set that class to make sure our project uses that as the input component). 
   *  For that we need to cast the InputComponent to UEnhancedInputComponent.
   * Instead of doing a Cast and then checking the pointer or using an assert like we did in BeginPlay(), we'll use CastChecked<>() which does
   *  both things at one go! So, if the cast fails we'll get a crash.
   * 
   * With the EnhancedInputComponent we can now bind an InputAction (IA) to it, and then bind callback functions that will use the data coming
   *  from the IA (eg. when the player presses WASD MoveAction is filled with data according to how we've configured our IMC). Move() will accept
   *  the data coming as an InputActionValue type and use it as needed.
   */

   Super::SetupInputComponent();

   UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

   EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

AAuraPlayerController::AAuraPlayerController()
{
   /** Changes will be sent to all clients in the same server. It'll be addressed better later.*/
   bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
   /** 
   * Perform the trace and handle the highlighting of any actor we hit that implements EnemyInterface.
   * For that, we'll call CursorTrace().
   */

   Super::PlayerTick(DeltaTime);
   CursorTrace();
}
