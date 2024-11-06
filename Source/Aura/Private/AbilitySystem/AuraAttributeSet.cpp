// Copyright Eveline Gomes.


#include "AbilitySystem/AuraAttributeSet.h"

/** Register variables for replication */
#include "Net/UnrealNetwork.h"

/** PostGameplayEffectExecute */
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"

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
   /** 
   * NewValue comes from a calculation from GE 
   */

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

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
   // Source = causer of the effect (source is something else that applied the effect to us)
   // Target = target of the effect (owner of this AS - us, in this context)

   /** Get Source's info */
   // Get our effect context
   Props.EffectContextHandle = Data.EffectSpec.GetContext();
   // Get the ASC from the source of this GE
   Props.SourceASC = Props.EffectContextHandle.GetInstigatorAbilitySystemComponent();

   // Since we're doing a lot of accessing pointers, we need to add some checks because not all sources might have ASC or AvatarActor.
   // TSharedPtr is a struct that has its own utilities, so we can use . operator to call those utilities. Then, once checked if that
   //  wrapper is valid, we can also use the -> operator to check if the pointer itself is valid.
   if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
   {
      // With the SourceASC, we can get other things such as the source actor that owns the ASC
      // AvatarActor is a pointer wrapper, so we have to call .Get() on that
      Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();

      // Get (and store) the source's player controller (if the source has one) -> changed to AController (instead of APlayerController) because
      //  Pawn->GetController() returns a AController and for now we don't need a PlayerController so we can avoid another cast. That could change
      //  later if needed.
      Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
      // Now, in case the AbilityActorInfo has a nullptr for the PlayerController, we can fallback on getting the player controller from
      //  the actor itself by casting it to a pawn. The source might not be a pawn, so we're only setting if we can.
      if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
      {
         // attempt to get the player controller from the pawn directly!
         // Cast the avatar actor to a pawn and try to get the player controller from that pawn
         if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
         {
            Props.SourceController = Pawn->GetController();
         }
      }
      if (Props.SourceController)
      {
         // Get the source character by casting the pawn possessed by the controller
         Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
      }
   }

   /** Get Target's info */
   // Get the target's avatar actor, but do the necessary checks before since we access a bunch of pointers before finally getting the actor.
   // For the checks we use the pointer wrapper utilities functions .IsValid()
   if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
   {
      Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
      // We can also get the target's controller
      Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
      // We can also get the character
      Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
      // We could access the ASC directly, but we can also access through the interface, or even with the UAbilitySystemBlueprintLibrary
      Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
   }
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
   Super::PostGameplayEffectExecute(Data);

   FEffectProperties Props;
   SetEffectProperties(Data, Props);

   /** 
   * Check if the attribute that's being changed is the one that's being affected (in our example: health)
   */
   if (Data.EvaluatedData.Attribute == GetHealthAttribute())
   {
      GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Health: %f"), GetHealth()));
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


