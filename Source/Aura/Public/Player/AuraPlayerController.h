// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;

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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	AAuraPlayerController();

};
