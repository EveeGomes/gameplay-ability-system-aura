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
	 */

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		/**
		 * We are on the server, so listen for target data.
		 * When the Target Data reaches the server, there's a delegate broadcast (as we saw).
		 *  We can get that delegate through the ASC.
		 * So as soon as Activate() is called on the server, there will be this bind between this delegate and the
		 *  callback. But what if we're too late and the target data has already been sent? And the delegate has already
		 *  been broadcast? Well in that case we should call the callback anyways!
		 *  So, there's a way to check that by calling the TargetDataDelegate if it already received target
		 *  data. We use CallReplicatedTargetDataDelegatesIfSet(...) which sends us a bool that tells us if TargetDataSet
		 *  has already been sent; and if not, we'll tell the server we're still waiting for it by calling
		 *  SetWaitingOnRemotePlayerData().
		 */
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); /*the activation prediction key since it says "Ability-Original-"*/  
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
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

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	/**
	 * Even though the common sense is to think of REPLICATION as data going from server to client, in GAS there's
	 *  a lot of situations in which data will travel from client to server via RPCs.
	 * This function will be called on the server when replicated data has been received. Then, we need to broadcast it
	 *  so our ability task, in the gameplay ability, has its valid data execution pin executed.
	 *  Beyond broadcasting, we need to make sure that the ASC knows the data has been received. That is because when
	 *  replicated data is received by the server, it gets stored in a specific data structure and we can tell the ASC
	 *  we received the data, so the ASC doesn't need to store it, it doesn't need to cache it anymore.
	 *  In the definition of ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(...) we can see that the ASC is able
	 *  to find something called "ability replicated data cache" inside a map that associates the ability spec handle
	 *  and the predition key with a target data! Therefore for each ability spec handle and prediction key, there is a
	 *  target data in that cached data data structure (which is processed in that method!). In short that method tells
	 *  the ASC that data has been received, don't worry about it anymore.
	 */

	// Tells the ASC target data has been received, so no need to keep it stored.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// Then, broadcast the data (checking it before)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
