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

protected:
	virtual void BeginPlay() override;

public:
	AAuraEnemy();
	/** <IEnemyInterface> */
	virtual void HighlightActor() override;
	virtual void UnHighlihtActor() override;
	/** </IEnemyInterface> */

	virtual void Tick(float DeltaTime) override;

};
