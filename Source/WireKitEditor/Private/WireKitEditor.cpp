#include "WireKitEditor.h"

#include "LevelEditor.h"
#include "WireEditorCommands.h"

#define LOCTEXT_NAMESPACE "FWireKitEditorModule"

void FWireKitEditorModule::StartupModule()
{
	FWireEditorCommands::Register();
	BindGlobalWireKitEditorCommands();
	
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateStatic(&FWireMenu::RegisterLevelEditorMenus));
}

void FWireKitEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

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

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FWireKitEditorModule, WireKitEditor)