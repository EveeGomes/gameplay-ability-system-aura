// Copyright Eveline Gomes.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * This is our Base class for widgets.
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/** 
	* Have a member variable for its widget controller, ie we want our widgets to have the concept of a controller. That way, each widget
	*  will have its controller set, and when that controller broadcasts data, our widgets will receive that data and respond to it. That will
	*  form a dependency that goes one way from our AuraUserWidget to AuraWidgetController.
	* The widget controller won't know what widgets are associated with, but the widgets will know what their controllers are.
	* 
	* We'll create a member variable, a pointer of type UObject that is very generic, allowing the widget controller to be any UObject making things
	*  a little bit more versatile. It must be accessed from BP as all our user widgets classes will be user widgets BPs. We'll make it BlueprintReadOnly
	*  so it won't be set it directly, but only accessed.
	* Most of the user widgets are responsible for the visuals for the widgets (what widgets look like on screen when they receive data). So, whenever we
	*  set our widget controller for a given user widget, we're going to initialize the visuals, ie we're going to do something visually in response.
	*  To set a widget controller we'll use a function that'll work like a BeginPlay. That'll also be a Blueprint Implementable Event, so it will be
	*  called whenever we set the WidgetController for a given UserWidget. IE we need to connect/associate the WidgetController with calling that function.
	*  For it, we'll have a function to set the WidgetController that's gonna be called from BP.
	*/

public:
	// Set the WidgetController and call WidgetControllerSet()
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	/** BeginPlay like function for setting/initializing the widget controller to a user controller */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
