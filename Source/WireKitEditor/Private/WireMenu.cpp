// Fill out your copyright notice in the Description page of Project Settings.

#include "WireMenu.h"

#include "ToolMenus.h"
#include "WireEditorCommands.h"

#define LOCTEXT_NAMESPACE "WireMenu"

void FWireMenu::RegisterLevelEditorMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	if (!Menu)
	{
		return;
	}

	FToolMenuSection& Section = Menu->FindOrAddSection(
		"WireKit",
		LOCTEXT("WireKitHeading", "WireKit"),
		FToolMenuInsert(NAME_None, EToolMenuInsertType::First));

	Section.AddMenuEntry(
		FWireEditorCommands::Get().OpenViewKitDetails,
		TAttribute<FText>(),
		TAttribute<FText>(),
		FSlateIcon(FName("InsightsStyle"), "Icons.CalleesView"));
}

#undef LOCTEXT_NAMESPACE