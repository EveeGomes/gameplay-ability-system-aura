// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributesMenuWidgetController.generated.h"

/**
 * Retrieve information from Attributes and send over to the Attributes Menu to be displayed. 
 */
UCLASS()
class AURA_API UAttributesMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
};
