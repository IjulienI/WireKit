// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WireKitDetailsMutliColumn.h"

/**
 * 
 */
class WIREKITEDITOR_API SWireKitDetails : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWireKitDetails) {}
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
private:
	int32 ActiveTab = 0;
	
	TSharedPtr<SEditableTextBox> NameEditableTextBox;
	
	TSharedPtr<SListView<TSharedPtr<FWireConnection>>> ConnectionList;
	TArray<TSharedPtr<FWireConnection>> Connections;
};
