// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	/** Begin APawn Interface */
	// Init ability actor info for the Server
	virtual void PossessedBy(AController* NewController) override;
	// Init ability actor info for the Client
	virtual void OnRep_PlayerState() override;
	/** End APawn Interface */

	/** Begin Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** End Combat Interface */

private:
	/** Begin AuraCharacterBase */
	void InitAbilityActorInfo() override;
	/** End AuraCharacterBase */
};
