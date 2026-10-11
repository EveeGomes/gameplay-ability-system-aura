// Copyright Eveline Gomes.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

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
	 *
	 * The following implementation has a limitation of not sending data up to the server. IE HitResult is empty and
	 *  when ValidData broadcasts there's no location to send!
	 */
	
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		if (!HitResult.bBlockingHit) return;

		ValidData.Broadcast(HitResult.Location);
	}

}
