// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

	/** 
	* The place to add the IMC we've created in the editor, to this player controller, is in BeginPlay().
	*/	

public:
	AAuraPlayerController();

	/** 
	* Check which actor implements the EnemyInterface. Since it's not an expensive operation and we need some responsiveness when hovering the 
	*  mouse cursor over many different actors, we'll implement this in the Tick function. Therefore, we have to override the PlayerTick method.
	*/
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	/** 
	* Pointers to use in CursorTrace().
	* The right way to hold a member variable to an interface is by using a template wrapper designed to hold interfaces:
	*  TScriptInterface<> ; We should use that instead of raw pointers.
	*/
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	/* Input Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/* Input Actions Callback Functions */
	// Since MoveAction is an IA that provides data, this function must have an input parameter of type FInputActionValue (forward declared struct)
	void Move(const FInputActionValue& InputActionValue);

	// Trace under the cursor
	void CursorTrace();

	/* Callbacks for InputActions from InputConfig */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	/* Set the InputConfig DA from BP */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/* It casts only once by checking if it's null (which will be the first time), and then casting to AuraASC. */
	UAuraAbilitySystemComponent* GetASC();

	/* Click to move related */
	/**
	 * This mechanic was inspired by the Top-Down Template. However, things will be a bit different in this project because
	 *  the template one implemented the short mouse press movement with a function that isn't replicated, so in the client
	 *  side it doesn't work.
	 *  For this project we'll implement the Click To Move using AddMovementInput() method and a spline for paths where an
	 *  obstacle needs to be avoided.
	 *
	 * FollowTime:
	 *  used to keep track if how much time the mouse button has been pressed before is released. That's so we know if it
	 *  was a short press. That variable holds the value that represents the amount of time we've been following the mouse
	 *  cursor (if we're holding the LMB down, we're following the cursor - the location under the mouse cursor!). So, as
	 *  we're following, we'll increment this variable!
	 *
	 * ShortPressThreshold
	 *  know the threshold of a short press - how long the LMB (mouse cursor) has been holding down before releasing it.
	 *  It should be initialized to a non-zero value since it's in seconds. 
	 *
	 * bAutoRunning
	 *  will be set to true if a short press happened, which is when we need to generate path points and a spline for a
	 *  smooth movement. As soon as it's true, AddMovementInput() will be called every single frame.
	 *
	 * AutoRunAcceptanceRadius 
	 *  When auto running, each and every frame we'll be getting closer and closer to our destination, and at a certain
	 *  point we need to stop the movement. So this variable represents a parameter, that we can change as we need/want,
	 *  of how close to the destination we should be to stop moving.
	 *
	 * Spline
	 *  it's a component that allows us to create a smooth curve out of some FVector world locations. It'll be constructed
	 *  on this class constructor, but its points will be set as we generate paths.
	 *
	 * bTargeting
	 *  this boolean will be used to keep track of whether we're targeting something.
	 */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
