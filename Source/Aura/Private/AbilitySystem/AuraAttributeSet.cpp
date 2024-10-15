// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAttributeSet.h"

/** Register variables for replication */
#include "Net/UnrealNetwork.h"

/** PostGameplayEffectExecute */
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
   // Initialize Attributes
   InitHealth(50.f);
   InitMaxHealth(100.f);
   InitMana(50.f);
   InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   /** 
   * Call the following macro to register variables to be replicated.
   * COND_None means it'll be replicated without any conditions.
   * REPNOTIFY_Always means that if the value is set on the server and replicated, then on the client it'll be updated and set. The other option,
   *  the default one, _OnChanged, if the variable is set to a value on the server and that value hasn't changed, then there won't be any replication;
   *  but for GAS we want to replicate anyways, even if there's no change (that's because if we're setting it we want to respond to the act of setting it, 
   *  whether if we set it to a new value or its own same value).
   */
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
   Super::PreAttributeChange(Attribute, NewValue);

   // Check if Attribute matches any of our attributes
   if (Attribute == GetHealthAttribute())
   {
      // Avoid Health to ever go above MaxHealth value or below 0
      NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
   }
   if (Attribute == GetManaAttribute())
   {
      // Avoid Mana to ever go above MaxMana or below 0
      NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
   }
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
   Super::PostGameplayEffectExecute(Data);

   // Source = causer of the effect (source is something else that applied the effect to us)
   // Target = target of the effect (owner of this AS - us, in this context)

   // Get our effect context
   const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
   // Get the ASC from the source of this GE
   const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetInstigatorAbilitySystemComponent();
   
   // Since we're doing a lot of accessing pointers, we need to add some checks because not all sources might have ASC or AvatarActor.
   // TSharedPtr is a struct that has its own utilities, so we can use . operator to call those utilities. Then, once checked if that
   //  wrapper is valid, we can also use the -> operator to check if the poniter itself is valid.
   if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
   {
      // With the SourceASC, we can get other things such as the source actor that owns the ASC
      // AvatarActor is a pointer wrapper, so we have to call .Get() on that
      AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();

      // Get (and store) the source's player controller (if the source has one) -> changed to AController (instead of APlayerController) because
      //  Pawn->GetController() returns a AController and for now we don't need a PlayerController so we can avoid another cast. That could change
      //  later if needed.
      const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
      // Now, in case the AbilityActorInfo has a nullptr for the PlayerController, we can fallback on getting the player controller from
      //  the actor itself by casting it to a pawn. The source might not be a pawn, so we're only setting if we can.
      if (SourceController == nullptr && SourceAvatarActor != nullptr)
      {
         // attempt to get the player controller from the pawn directly!
         // Cast the avatar actor to a pawn and try to get the player controller from that pawn
         if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
         {
            SourceController = Pawn->GetController();
         }
      }
      if (SourceController)
      {
         // Get the source character by casting the pawn possessed by the controller
         ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
      }
   }
   

   
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
   // Inform the ability system that Health has just been replicated
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
