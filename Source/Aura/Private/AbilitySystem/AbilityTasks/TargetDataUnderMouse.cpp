// Copyright Eveline Gomes.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

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

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//TODO: We are on the server, so listen for target data.
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	/**
	 * If we're on the client and locally controlled, this function will be called and so we can broadcast our delegate
	 *  AND send data up to the server.
	 * If we're on the server and locally controlled, we simply broadcast our delegate.
	 * We check in Activate().
	 *
	 * This method needs to create some target data and send it. In L117, the instructor shows how to know which type
	 *  to create, by looking GamePlayAbilityTargetTypes.h file since it has many types according to what's needed.
	 */
	
	/**
	 * Before using the ScopedPredictionKey we need to create a ScopedPredictionWindow so we say everything inside this
	 *  function (scope) should be predicted! IE we're telling the server to allow us to do what we're doing here
	 *  locally.
	 */
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	if (PlayerController)
	{
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		if (!HitResult.bBlockingHit) return; // should this be here?
	}

	// Used to packet Data since ServerSetReplicatedTargetData needs this DataHandle variable!
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	DataHandle.Add(Data);

	// Send the data up to the server
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()/* associated with the activation of the ability */,
		DataHandle,
		FGameplayTag()/* just a temporary param */,
		AbilitySystemComponent->ScopedPredictionKey/* needed for right now, to this particular context.
		so we can create one using the ASC */
	);

	// Make sure we can broadcast delegates
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// Broadcast the data handle and from that get the location (and other things if needed)
		ValidData.Broadcast(DataHandle);
	}
}
