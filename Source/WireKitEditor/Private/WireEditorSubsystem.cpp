// Fill out your copyright notice in the Description page of Project Settings.


#include "WireEditorSubsystem.h"

#include "EngineUtils.h"
#include "LevelEditor.h"
#include "WireComponent.h"


void UWireEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//---------------------------------------
	// Delegate bind
	//---------------------------------------
	OnNewActorsPlacedHandle = FEditorDelegates::OnNewActorsPlaced.AddUObject(this, &UWireEditorSubsystem::OnNewActorsPlaced);
	OnMapOpenedHandle = FEditorDelegates::OnMapOpened.AddUObject(this, &UWireEditorSubsystem::OnMapOpened);
	OnPostUndoRedoHandle = FEditorDelegates::PostUndoRedo.AddUObject(this, &UWireEditorSubsystem::OnPostUndoRedo);
	
	OnLevelActorDeletedHandle = GEngine->OnLevelActorDeleted().AddUObject(this, &UWireEditorSubsystem::OnLevelActorDeleted);
	
	auto& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	OnActorSelectionChangedHandle = LevelEditor.OnActorSelectionChanged().AddUObject(this, &UWireEditorSubsystem::OnActorSelectionChanged);
}

void UWireEditorSubsystem::Deinitialize()
{
	//---------------------------------------
	// Delegate unbind
	//---------------------------------------
	GEngine->OnLevelActorDeleted().Remove(OnLevelActorDeletedHandle);
	
	FEditorDelegates::OnNewActorsPlaced.Remove(OnNewActorsPlacedHandle);
	FEditorDelegates::OnMapOpened.Remove(OnMapOpenedHandle);
	FEditorDelegates::PostUndoRedo.Remove(OnPostUndoRedoHandle);
	
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
		UWireComponent* WireComponent = NewObject<UWireComponent>(Actor, TEXT("Wire"), RF_Transactional);
		Actor->AddInstanceComponent(WireComponent);
		WireComponent->RegisterComponent();
		
		Wires.Add(WireComponent);
	}
}

void UWireEditorSubsystem::OnLevelActorDeleted(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	UWireComponent* WireComponent = Actor->FindComponentByClass<UWireComponent>();
	if (!WireComponent)
	{
		return;
	}

	Wires.RemoveAll([WireComponent](const TWeakObjectPtr<UWireComponent>& Component)
	{
		return Component.Get() == WireComponent;
	});

	if (CurrentSelection.Get() == WireComponent)
	{
		CurrentSelection = nullptr;
		OnWireSelectionChanged.Broadcast();
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

void UWireEditorSubsystem::OnMapOpened(const FString& InMapName, bool bIsTemplate)
{
	RefreshWires();
}

void UWireEditorSubsystem::OnPostUndoRedo()
{
	bWiresListDirty = true;
	
	GEditor->GetTimerManager()->SetTimerForNextTick([this]()
	{
		if (bWiresListDirty)
		{
			RefreshWires();
			bWiresListDirty = false;
			
			if (!CurrentSelection.IsValid())
			{
				CurrentSelection = nullptr;
				OnWireSelectionChanged.Broadcast();
			}
		}
	});
}

void UWireEditorSubsystem::RefreshWires()
{
	const UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return;
	}
	
	Wires.Empty();
	
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (const AActor* Actor = *It)
		{
			if (auto* WireComponent = Actor->GetComponentByClass<UWireComponent>())
			{
				Wires.Add(WireComponent);
			}
		}
	}
}
