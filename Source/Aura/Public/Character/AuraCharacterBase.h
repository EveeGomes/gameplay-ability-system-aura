// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

private:

protected:
	virtual void BeginPlay() override;

	/** 
	* Add a Skeletal Mesh component to this base class so both children can inheret and both can have a weapon.
	* This weapon will be attached to the socket on the skeleton, therefore we need to make sure the skeleton meshes 
	*  have the socket we specified.
	*
	* Using TObjecPtr wrapper for members, adds features such as access tracking and optional lazy load behavior.
	*  - Access tracking allows us to know when an object is being used, ie we can track how often a pointer is
	*  accessed or dereferenced.
	*  - Lazy load behavior "means that an asset can not be loaded until it's actually needed or used in the program".
	* In packaged build it behaves the same as a raw pointer though, but add those features in editor!
	* 
	* Nice to check: https://forums.unrealengine.com/t/why-should-i-replace-raw-pointers-with-tobjectptr/232781
	* 
	* As all pointers, we'll make it a UPROPERTY()
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

public:	
	AAuraCharacterBase();

};
