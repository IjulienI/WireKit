#include "WireKitEditor.h"

#include "LevelEditor.h"
#include "WireEditorCommands.h"
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
		.Label(LOCTEXT("TabTitle", "WireKit Details"))
		[
			SNew(SWireKitDetails)
		];
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWireKitEditorModule, WireKitEditor)