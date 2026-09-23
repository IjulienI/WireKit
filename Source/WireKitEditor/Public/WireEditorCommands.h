// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WIREKITEDITOR_API FWireEditorCommands : public TCommands<FWireEditorCommands>
{
	
public:
	FWireEditorCommands();
	
	virtual void RegisterCommands() override;
	
	
	FORCENOINLINE static const FWireEditorCommands& Get();
	
	TSharedPtr<FUICommandInfo> OpenViewKitDetails;
};

class WIREKITEDITOR_API FWireEditorActionCallbacks
{
public:
	static void OpenViewKitDetails();
};
