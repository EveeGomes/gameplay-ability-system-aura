// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/** Struct with 4 key variables */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// Default constructor
	FWidgetControllerParams() {} // use = default; instead of empty body {}?
	// Define the constructor using an initializer list
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) 
		: fPlayerController(PC), fPlayerState(PS), fAbilitySystemComponent(ASC), fAttributeSet(AS) {}

	// When creating structs we should initialize all member variables otherwise we tend to get compiler warnings.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> fPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> fPlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> fAbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> fAttributeSet = nullptr;
};

/**
 * This is our class responsible for getting data from the system, from the MODEL in our architecture, and then broadcast to our user widgets.
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
	/** 
	* Here we'll have a set of variables from which it's going to get any data.
	* As for this game project, we're interested in 4 classes to retrieve data from, so we'll have to have access to them:
	*  - the ASC;
	*  - the attribute set;
	*  - the player state;
	*  - the player controller
	* We'll make these as protected variables, so children can have access to it.
	* 
	* As this widget controller has these 4 key variables, we need to set them when creating a widget controller. An easier way to do it is
	*  to create a struct with these 4 variables in it, making initialization of a widget controller easier.
	* After creating the struct, we'll have a function that initializes the member variables with values from that struct when a widget controller
	*  is created.
	* 
	* This class will work as a Base class of "widget controllers". This way we can have derived widget controllers that would work with each
	*  specific widget, for example our Overlay widget would have its own widget controller.
	*/
public:
	// Initializes struct values
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	/** 
	* The widget controller doesn't know anything about the widget. All it knows is that it has access to all 4 key variables, which is how it's
	*  going to access data to broadcast them to the widget! By broadcasting, we're talking about Delegates for they're great ways to have one-way
	*  dependencies (widget controller doesn't know about the widget, but the widget knows about the widget controller).
	* So, the widget controller should have a way of broadcasting initial values, like initial health and mana values.
	* We'll have a virtual function for broadcasting initial values so that any widget controller child of this class that inherits this function 
	*  can broadcast its own unique initial values.
	*/
	virtual void BroadcastInitialValues();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
