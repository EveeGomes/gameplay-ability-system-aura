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
   /** 
   * MaxHealth and MaxMana are depended on other attributes, and since we need those values to set Health and Mana we won't initialize these values
   *  here like we used to using the function from their ATTRIBUTE_ACCESSORS macros (InitHealth(10.f); InitMana(10.f);). We actually have to initialize
   *  them only after MaxHealth and MaxMana have been set! Therefore timing is important here.
   * The preferred way of setting attribute values is by GEs (the instructor's preferred way hehe). So we'll have a GE that initializes Health and Mana
   *  to the values of MaxHealth and MaxMana. This can be an instant GE, instead of infinite, since it just initializes those values.
   * We need a GE to initialize our vital attributes, and that should be a variable which we set where we have other variables for the GEs that 
   *  initialize our attributes: in AuraCharacterBase. As soon as we have the variable, we'll apply that attribute by calling the method ApplyEffectToSelf
   *  in InitializeDefaultAttributes() after the call using primary and secondary GEs. Then, we call that method passing this new variable.
   */
   // Initialize Attributes
   //InitHealth(10.f);
   //InitMana(10.f);
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

   // Primary Attributes
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

   // Secondary Attributes
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

   // Vital Attributes
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
   DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
   // IMPORTANT: remove clamping from this function. Remove this function then?
   // https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39784058#questions/20594972


   /** 
   * NewValue comes from our modifier on the GE that's being applied.
   * Then, we're clamping it before the attribute changes. However, that clamp isn't going to permanentely change that modifier! It just changes
   *  the value that is being returned from querying that modifier. And in case there's another GE with its own modifier queried, then it's going
   *  to recalculate the value from current value! I.E. it's kind of like the value was never actually clamped... We clamped before the attribute
   *  changes, but that modifier recalculates. This just changes the value returned from querying the modifier.
   * So, to solve this we should clamp again, but in this case we should do it in PostGameplayEffectExecute() as this is called after the GE has
   *  already been applied. There will only be a single replication resulting from the attributes values.
   */

   Super::PreAttributeChange(Attribute, NewValue);

   //// Check if Attribute matches any of our attributes
   //if (Attribute == GetHealthAttribute())
   //{
   //   // Avoid Health to ever go above MaxHealth value or below 0
   //   NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
   //}
   //if (Attribute == GetManaAttribute())
   //{
   //   // Avoid Mana to ever go above MaxMana or below 0
   //   NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
   //}
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
   * Check if the attribute that's being changed is the one that's being affected (in our example: health). If so, clamp it again as changes here
   *  happens to the BaseValue rather than to CurrentValue as in PreAttributeChange() (where we were only changing the NewValue).
   */
   // Clamp Health
   if (Data.EvaluatedData.Attribute == GetHealthAttribute())
   {
      SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
   }
   // Clamp Mana
   if (Data.EvaluatedData.Attribute == GetManaAttribute())
   {
      SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
   }
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
   
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
   // Inform the ability system that Health has just been replicated
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
   GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}


