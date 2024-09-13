// Copyright Eveline Gomes.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


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
   */

   // Retrieve data from InputActionValue
   const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

   // Get a forward direction vector, and right direction vector
   const FRotator Rotation = GetControlRotation();
   const FRotator YawRotation{ 0., Rotation.Yaw, 0. };
   const FVector ForwardDirection = FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X);
   const FVector RightDirection = FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y);
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
   *  stores the address of an object of UEnhancedInputComponent type. For that we need to cast the InputComponent to UEnhancedInputComponent.
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
