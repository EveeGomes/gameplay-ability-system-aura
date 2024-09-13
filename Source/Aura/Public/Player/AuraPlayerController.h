// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Input Actions Callback Functions */
	// Since MoveAction is an IA that provides data, this function must have an input parameter of type FInputActionValue (forward declared struct)
	void Move(const FInputActionValue& InputActionValue);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	AAuraPlayerController();

};
