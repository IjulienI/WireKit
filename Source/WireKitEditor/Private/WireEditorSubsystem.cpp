// Fill out your copyright notice in the Description page of Project Settings.


#include "WireEditorSubsystem.h"

#include "LevelEditor.h"
#include "WireComponent.h"


void UWireEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//---------------------------------------
	// Delegate bind
	//---------------------------------------
	OnNewActorsPlacedHandle = FEditorDelegates::OnNewActorsPlaced.AddUObject(this, &UWireEditorSubsystem::OnNewActorsPlaced);
	
	auto& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	OnActorSelectionChangedHandle = LevelEditor.OnActorSelectionChanged().AddUObject(this, &UWireEditorSubsystem::OnActorSelectionChanged);
}

void UWireEditorSubsystem::Deinitialize()
{
	//---------------------------------------
	// Delegate unbind
	//---------------------------------------
	FEditorDelegates::OnNewActorsPlaced.Remove(OnNewActorsPlacedHandle);
	
	auto& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditor.OnActorSelectionChanged().Remove(OnActorSelectionChangedHandle);
	
	Super::Deinitialize();
}

void UWireEditorSubsystem::OnNewActorsPlaced(UObject* ObjToUse, const TArray<AActor*>& NewActors)
{
	for (AActor* Actor : NewActors)
	{
		if (!Actor || Actor->FindComponentByClass<UWireComponent>())
		{
			continue;
		}

		Actor->Modify();
		UWireComponent* Comp = NewObject<UWireComponent>(Actor, TEXT("Wire"), RF_Transactional);
		Actor->AddInstanceComponent(Comp);
		Comp->RegisterComponent();
	}
}

void UWireEditorSubsystem::OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh)
{
	if (!NewSelection.IsEmpty())
	{
		const auto* Actor = Cast<AActor>(NewSelection[0]);
		if (!Actor)
		{
			CurrentSelection = nullptr;
			OnWireSelectionChanged.Broadcast();
			return;
		}
		if (auto* WireComponent = Actor->GetComponentByClass<UWireComponent>())
		{
			CurrentSelection = WireComponent;
			OnWireSelectionChanged.Broadcast();
		}
		else
		{
			CurrentSelection = nullptr;
			OnWireSelectionChanged.Broadcast();
		}
	}
}
