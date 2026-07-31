// Copyright Eveline Gomes.


#include "Player/AuraPlayerController.h"

/* Input */
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"

/* Interfaces */
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/EnemyInterface.h"

/* Other Components */
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
   /* Changes will be sent to all clients in the same server. It'll be addressed better later.*/
   bReplicates = true;

   Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
   * As for class 33, we'll change from using an assert to using if statement. That is because for multiplayer games, the subsystem might be null
   *  and we don't want the game to crash but to continue. And only when isn't null is that we can add the mapping context. Also check comment on
   *  AuraCharacter -> InitAbilityActorInfo().
   * 
   * Then, we proceed to add the IMC. Since we only have one, we'll have priority as 0.
   */

   UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
   if (Subsystem)
   {
      Subsystem->AddMappingContext(AuraContext, 0);
   }

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

   UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
   AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
   AuraInputComponent->BindAbilityActions(InputConfig, this,
      &ThisClass::AbilityInputTagPressed,
      &ThisClass::AbilityInputTagReleased,
      &ThisClass::AbilityInputTagHeld);
}

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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
   /**
    * The variable we use to check whether we're hovering over an enemy, ThisActor, is always set to a value that can be
    *  either null or an enemy we've hovered. That variable we can use to distinguish whether we're clicking the LMB to
    *  move or to activate (or try to) an ability.
    *  So bTargeting will be set to true if ThisActor has a valid value, and false if it's null.
    *
    * bAutoRunning is set to false at the beginning because this function is called as soon as a key/button is pressed, so
    *  we don't know yet if it's a short press or not until we RELEASE the key/button!
    *
    * So, all the logic above should happen only if InputTag is the LMB!
    */

   if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
   {
      bTargeting = ThisActor ? true : false;
      bAutoRunning = false;
   }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
   /**
    * We'll do similarly to TagHeld since we need to check if we're using the LMB or not. That check is done so we can
    *  activate the abilities, otherwise proceed with the LMB logic.
    *
    * Here in TagReleased we want to check the FollowTime so we know if it was a short press or not. So if it's less or
    *  equal to the ShortPressThreshold we've set, we want to find a path, a navigation path = a set of points to follow.
    *  For that we'll use the function UNavigationSystemV1::FindPathToLocationSynchronously()! That function returns a
    *  pointer to a UNavigationPath, which has a member .PathPoints that consists of a TArray of FVectors!! That's what
    *  we need to generate our spline points!!!
    *  In Addition to those spline points, we'll draw some spheres so we can visualize them!
    *  We need to add NavigationSystem to Aura.Build.cs in PrivateDependencyModuleNames (private because we're using it
    *  here in a .cpp file, a private file).
    *
    * So for creating the spline points:
    *  - first we need to clear any points that existed before!
    *  - then we'll loop through the points from the NavPath and add them to the spline!
    *  - to help visualize them for testing we'll draw some spheres!
    * Another thing to do is set bAutoRunning to true, since this is the bevahior of "auto running".
    * We also need to reset FollowTime to zero and bTargeting to false at the end of the whole else statement!
    *
    * In the editor we must add a Nav Mesh Bounds Volume for this whole logic to work!!!
    */
   
   if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
   {
      if (GetASC())
      {
         GetASC()->AbilityInputTagReleased(InputTag);
      }

      return;
   }
   
   if (bTargeting)
   {
      if (GetASC())
      {
         GetASC()->AbilityInputTagHeld(InputTag);
      }
   }
   else
   {
      APawn* ControlledPawn = GetPawn();
      if (FollowTime <= ShortPressThreshold && ControlledPawn)
      {
         if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
         {
            Spline->ClearSplinePoints();
            for (const FVector& PointLoc : NavPath->PathPoints)
            {
               Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
               DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
            }
            bAutoRunning = true;
         }
      }
      FollowTime = 0.f;
      bTargeting = false;
   }
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
   /**
    * Similar to AbilityInputTagPressed() we should check for the InputTag_LMB. However, here we should proceed with the
    *  old implementation whenever IT'S NOT InputTag_LMB, which is to try and activate an ability associated with InputTag.
    *  At the end we should return (whether ASC is valid or not!).
    *
    * Now, when it's InputTag_LMB, it means we're holding the LMB down, and in that case we need to know if we're targeting
    *  before proceeding. If so, it means we want to attack, ie activate the ability! Otherwise we're concern with the
    *  move behavior!
    *  - so we gotta increment the FollowTime with the delta seconds;
    *  - get the world location, which is the destination we want to move towards! (ps: it's ok for a line trace to use
    *    .Location or .HitPoint but if it was a sphere that would be different because that could mean the surface or the
    *    center of the sphere!).
    *  - then, we think about calling AddMovementInput. So, first we find the vector from the controlled pawn to the
    *    CachedDestination and normalize it to get the direction.
    *    Finally we can use that direction when calling AddMovementInput.
    */
   
   if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
   {
      if (GetASC())
      {
         GetASC()->AbilityInputTagHeld(InputTag);
      }

      return;
   }
   
   if (bTargeting)
   {
      if (GetASC())
      {
         GetASC()->AbilityInputTagHeld(InputTag);
      }
   }
   else
   {
      FollowTime += GetWorld()->GetDeltaSeconds();

      FHitResult Hit;
      if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit))
      {
         CachedDestination = Hit.Location;
      }

      if (APawn* ControlledPawn = GetPawn())
      {
         const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
         ControlledPawn->AddMovementInput(WorldDirection);
      }
   }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
   if (AuraAbilitySystemComponent == nullptr)
   {
      AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
         UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>())
      );
   }

   return AuraAbilitySystemComponent;
}
