// Copyright Eveline Gomes.

#pragma once

#include "Interaction/EnemyInterface.h"

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	/** Being IEnemyInterface Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlihtActor() override;
	/** End IEnemyInterface Interface */

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
