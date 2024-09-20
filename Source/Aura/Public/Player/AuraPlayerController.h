// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

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

	/** Input Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Input Actions Callback Functions */
	// Since MoveAction is an IA that provides data, this function must have an input parameter of type FInputActionValue (forward declared struct)
	void Move(const FInputActionValue& InputActionValue);

	// Trace under the cursor
	void CursorTrace();

};
