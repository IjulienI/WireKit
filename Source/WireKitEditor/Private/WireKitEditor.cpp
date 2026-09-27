#include "WireKitEditor.h"

#include "LevelEditor.h"
#include "UnrealEdGlobals.h"
#include "WireComponent.h"
#include "WireComponentVisualizer.h"
#include "WireEditorCommands.h"
#include "Editor/UnrealEdEngine.h"
#include "Slate/WireKitDetails.h"

#define LOCTEXT_NAMESPACE "FWireKitEditorModule"

const FName FWireKitEditorModule::WireKitDetailsTabId(TEXT("WireKitDetails"));

void FWireKitEditorModule::StartupModule()
{
	FWireEditorCommands::Register();
	BindGlobalWireKitEditorCommands();
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
			WireKitDetailsTabId,
			FOnSpawnTab::CreateRaw(this, &FWireKitEditorModule::SpawnWireKitDetailsTab))
		.SetDisplayName(LOCTEXT("TabDisplayName", "WireKit Details"))
		.SetTooltipText(LOCTEXT("TabTooltip", "Open WireKit details"))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FName("InsightsStyle"), "Icons.CalleesView"));
	
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateStatic(&FWireMenu::RegisterLevelEditorMenus));
	
	if (GUnrealEd)
	{
		TSharedPtr<FWireComponentVisualizer> WireVisualizer = MakeShareable(new FWireComponentVisualizer);
		GUnrealEd->RegisterComponentVisualizer(UWireComponent::StaticClass()->GetFName(), WireVisualizer);
	}
}

void FWireKitEditorModule::ShutdownModule()
{
	if (FSlateApplication::IsInitialized())
	{
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(WireKitDetailsTabId);
	}
	
	if (GUnrealEd) 
	{
		GUnrealEd->UnregisterComponentVisualizer(UWireComponent::StaticClass()->GetFName());
	}
	
	FWireEditorCommands::Unregister();
}

void FWireKitEditorModule::BindGlobalWireKitEditorCommands()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedRef<FUICommandList> CommandBindings = LevelEditorModule.GetGlobalLevelEditorActions();

	const FWireEditorCommands& Commands = FWireEditorCommands::Get();
	FUICommandList& ActionList = *CommandBindings;
	
	ActionList.MapAction(
		Commands.OpenViewKitDetails,
		FExecuteAction::CreateStatic(&FWireEditorActionCallbacks::OpenViewKitDetails));
}

TSharedRef<SDockTab> FWireKitEditorModule::SpawnWireKitDetailsTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(LOCTEXT("TabTitle", "WireKit Details"))
		[
			SNew(SWireKitDetails)
		];
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWireKitEditorModule, WireKitEditor)