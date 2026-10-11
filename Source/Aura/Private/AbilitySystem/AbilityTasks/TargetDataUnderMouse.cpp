// Copyright Eveline Gomes.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "Character/AuraCharacter.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	/**
	 * No need to call Super because it only does a UE_VLOG.
	 *
	 * All we need here is to get data and then broadcast ValidData.
	 * We need the mouse hit location and broadcast that.
	 */

	const AAuraCharacter* AuraCharacter = Cast<AAuraCharacter>(GetAvatarActor());
	if (AuraCharacter)
	{
		const APlayerController* PlayerController = Cast<APlayerController>(AuraCharacter->GetController());
		if (PlayerController)
		{
			FHitResult HitResult;
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
			if (!HitResult.bBlockingHit) return;

			ValidData.Broadcast(HitResult.Location);
		}
	}
}
