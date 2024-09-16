// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Highlight the enemy when we hover over it (pure virtual = 0, making this class an abstract one. Derived children must implement pure functions)
	virtual void HighlightActor() = 0;
	virtual void UnHighlihtActor() = 0;
};
