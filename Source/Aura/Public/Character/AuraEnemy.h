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
	/** <IEnemyInterface> */
	virtual void HighlightActor() override;
	virtual void UnHighlihtActor() override;
	/** </IEnemyInterface> */

	
	UPROPERTY(BlueprintReadOnly) // Also, we can do this in BP for debugging Highlight and Unhighlight methods 
	bool Highlighted = false;

	virtual void Tick(float DeltaTime) override;
	AAuraEnemy();
};
