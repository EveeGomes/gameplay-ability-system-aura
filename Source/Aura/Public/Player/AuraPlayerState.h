// Copyright Eveline Gomes.

#pragma once

/** Interface */
#include "AbilitySystemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


/** Forward Declaration */
class UAbilitySystemComponent;
class UAttributeSet;

/** 
* This is the most important class for shared information about a specific player. It's meant to hold current info about the player, and each player
*  has their own PS. https://cedric-neukirchen.net/docs/multiplayer-compendium/common-classes/playerstate/
* https://gascompanion.github.io/asc-on-player-state/
* This is where the ASC and AttributeSet(AS) are going to live for the player controlled character (Aura). For the enemies, both ASC and AS are going
*  to live on the enemy class itself! (Check class 17 for the reasons to put ASC and AS in this class for Aura).
*/
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	
	// Mark variables to be replicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// GETTERS:
	/** Begin IAbilitySystemInterface Interface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End IAbilitySystemInterface Interface */

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/** 
	* Declare those pointers here since our player controlled character won't have them.
	*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	/** 
	* Level is going to be replicated. It needs to have its own Rep Notify so we can show it in the HUD, and broadcast it whenever it's replicated!
	* So, in order to replicate anything, we shall override GetLifetimeReplicatedProps() method!
	* Then, we add the Level Rep Notify function, which must be a UFUNCTION to be considered a rep notify.
	* Finally, Level can be marked as replicated by adding to its UPROPERTY the specifier ReplicatedUsing.
	* 
	* This variable reside here, but this should be the case only for the player controlled character. For enemies, it should be on the Enemy Character
	*  class: AuraEnemy.
	*/
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
