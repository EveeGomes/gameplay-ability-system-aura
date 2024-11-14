// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** 
	* As we don't want it to be required to implement this function, and in case it's not implemented it should return zero. For that we'll
	*  generate a definition in this interface .cpp file that returns 0. Therefore, as we add this interface to the BaseCharacter class, it'll
	*  have an implementation and the children classes won't be required to add an implementation as well.
	*/
	virtual int32 GetLevel();
};
