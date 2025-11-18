// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Singleton used for loading primary assets.
 *
 * This is also where AuraGameplayTags singleton can initialize tags by calling its method, FAuraGameplayTags::InitializeNativeGameplayTags().
 *
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	/** Casts the engine's asset manager to this type and returns it. */
	static UAuraAssetManager& Get();
};
