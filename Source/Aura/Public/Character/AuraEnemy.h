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

	/** Begin AuraCharacterBase */
	void InitAbilityActorInfo() override;
	/** End AuraCharacterBase */

private:

	/** 
	* For enemy character, we don't need to replicate this Level varibale. That is because we're only concerned with checking the level on the server 
	*  for AI controlled enemies. And that's because important things that will require that level in calculations will only be done on the server.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
