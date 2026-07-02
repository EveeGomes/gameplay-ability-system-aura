// Copyright Eveline Gomes.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	/**
	 * This asset manager can be set to be the project's asset manager by setting it in a config file.
	 * Once that is done we can use this method to cast the engine's Asset manager to this type and return it.
	 */

	// First assert GEngine is valid. It's better to do it through an assert and get a crash intentionally, since having an invalid GEngine is a huge problem.
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	// Dereference the pointer since this method expects to return a reference to UAuraAssetManager.
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
}
