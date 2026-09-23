// Fill out your copyright notice in the Description page of Project Settings.


#include "WireEditorCommands.h"

#include "WireKitEditor.h"

#define LOCTEXT_NAMESPACE "WireEditorActions"

FWireEditorCommands::FWireEditorCommands() 
	: TCommands<FWireEditorCommands>
	(
		"WireEditor",
		NSLOCTEXT("Contexts", "WireEditor", "Wire Editor"),
		"LevelViewport",
		FAppStyle::GetAppStyleSetName()
	)
{
}

FORCENOINLINE const FWireEditorCommands& FWireEditorCommands::Get()
{
	return TCommands<FWireEditorCommands>::Get();
}

UE_DISABLE_OPTIMIZATION_SHIP

void FWireEditorCommands::RegisterCommands()
{
	UI_COMMAND( OpenViewKitDetails, "WireKit Details", "Details on how to use the Level Editor", EUserInterfaceActionType::Button, FInputChord( EKeys::F8 ) );
}

void FWireEditorActionCallbacks::OpenViewKitDetails()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FWireKitEditorModule::WireKitDetailsTabId);
}

UE_ENABLE_OPTIMIZATION_SHIP

#undef LOCTEXT_NAMESPACE