// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "WireEditorSubsystem.generated.h"

class UWireComponent;

DECLARE_MULTICAST_DELEGATE(FOnWireSelectionChanged);

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
	TMultiMap<FName, TWeakObjectPtr<UWireComponent>>& GetWires() { return Wires; }

	//---------------------------------------
	// Public delegate
	//---------------------------------------
	FOnWireSelectionChanged OnWireSelectionChanged;
	
private:
	//---------------------------------------
	// Delegate functions
	//---------------------------------------
	void OnNewActorsPlaced(UObject* ObjToUse, const TArray<AActor*>& NewActors);
	void OnLevelActorDeleted(AActor* Actor);
	void OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh);
	void OnMapOpened(const FString& InMapName, bool bIsTemplate);
	void OnPostUndoRedo();
	void OnObjectsReplaced(const TMap<UObject*, UObject*>& OldToNewInstanceMap);

	//---------------------------------------
	// Delegate handles
	//---------------------------------------
	FDelegateHandle OnNewActorsPlacedHandle;
	FDelegateHandle OnLevelActorDeletedHandle;
	FDelegateHandle OnPostUndoRedoHandle;
	FDelegateHandle OnMapOpenedHandle;
	FDelegateHandle OnActorSelectionChangedHandle;
	FDelegateHandle OnObjectsReplacedHandle;

	//---------------------------------------
	// Internal funcs
	//---------------------------------------
	void RefreshWires();
	
	//---------------------------------------
	// Internal vars
	//---------------------------------------
	UPROPERTY()
	TWeakObjectPtr<UWireComponent> CurrentSelection;
	TMultiMap<FName, TWeakObjectPtr<UWireComponent>> Wires;
	bool bWiresListDirty = false;
};
