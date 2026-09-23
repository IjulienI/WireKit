// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WIREKITEDITOR_API FWireMenu
{
public:
	static void RegisterLevelEditorMenus();
	
private:
	TSharedPtr<FUICommandInfo> OpenViewKitDetails;
};
