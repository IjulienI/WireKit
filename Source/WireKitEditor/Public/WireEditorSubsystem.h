// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "WireEditorSubsystem.generated.h"

class UWireComponent;
/**
 * 
 */
UCLASS()
class WIREKITEDITOR_API UWireEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	//---------------------------------------
	// UEditorSubsystem Overrides
	//---------------------------------------
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//---------------------------------------
	// Getters
	//---------------------------------------
	TWeakObjectPtr<UWireComponent> GetCurrentSelection() const { return CurrentSelection; }
	
private:
	//---------------------------------------
	// Delegate functions
	//---------------------------------------
	void OnNewActorsPlaced(UObject* ObjToUse, const TArray<AActor*>& NewActors);
	void OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh);

	//---------------------------------------
	// Delegate handles
	//---------------------------------------
	FDelegateHandle OnNewActorsPlacedHandle;
	FDelegateHandle OnActorSelectionChangedHandle;

	//---------------------------------------
	// Internal vars
	//---------------------------------------
	UPROPERTY()
	TWeakObjectPtr<UWireComponent> CurrentSelection;
};
