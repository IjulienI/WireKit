#include "WireKitEditor.h"

#include "LevelEditor.h"
#include "WireEditorCommands.h"
#include "WireKitDetails.h"

#define LOCTEXT_NAMESPACE "FWireKitEditorModule"

const FName FWireKitEditorModule::WireKitDetailsTabId(TEXT("WireKitDetails"));

void FWireKitEditorModule::StartupModule()
{
	FWireEditorCommands::Register();
	BindGlobalWireKitEditorCommands();
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
			WireKitDetailsTabId,
			FOnSpawnTab::CreateRaw(this, &FWireKitEditorModule::SpawnWireKitDetailsTab))
		.SetDisplayName(LOCTEXT("TabTitle", "WireKit Details"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateStatic(&FWireMenu::RegisterLevelEditorMenus));
}

void FWireKitEditorModule::ShutdownModule()
{
	if (FSlateApplication::IsInitialized())
	{
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(WireKitDetailsTabId);
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
		[
			SNew(SWireKitDetails)
		];
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWireKitEditorModule, WireKitEditor)