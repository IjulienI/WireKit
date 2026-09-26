// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WireKitDetailsMutliColumn.h"

class UWireComponent;
/**
 * 
 */
class WIREKITEDITOR_API SWireKitDetails : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWireKitDetails) {}
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	~SWireKitDetails();
	
private:
	int32 ActiveTab = 0;
	
	TWeakObjectPtr<UWireComponent> CurrentWireComponent;
	FName LocalObjectName = NAME_None;
	TSharedPtr<SEditableTextBox> NameEditableTextBox;
	TArray<TSharedPtr<FWireConnection>> Clipboard;
	
	TSharedPtr<SListView<TSharedPtr<FWireConnection>>> ConnectionList;
	TArray<TSharedPtr<FWireConnection>> Connections;
	
	TSharedPtr<SSuggestionTextBox> OutputSuggestionTextBox;
	TSharedPtr<SSuggestionTextBox> TargetSuggestionTextBox;
	TSharedPtr<SSuggestionTextBox> InputSuggestionTextBox;
	TSharedPtr<SSuggestionTextBox> ParameterSuggestionTextBox;
	TSharedPtr<SSuggestionTextBox> DelaySuggestionTextBox;
	TSharedPtr<SCheckBox> DoOnceSuggestionCheckBox;
	
	FDelegateHandle OnWireSelectionChangedHandle;
	
	 bool GetSelectedConnection(TSharedPtr<FWireConnection>& OutConnection) const;
	
	void OnSelectionChanged();
	void OnConnectionListSelectionChanged(const TSharedPtr<FWireConnection>& Connection);
	void AddConnectionRow();
	void CopyConnectionRow();
	void PasteConnectionRow();
	void DuplicateConnectionRow();
	void RemoveConnectionRow();
	void OnOutputContentCommitted(const FText& InText);
	void OnTargetContentCommitted(const FText& InText);
	void OnInputContentCommitted(const FText& InText);
	void OnParameterContentCommitted(const FText& InText);
	void OnDelayContentCommitted(const FText& InText);
	void OnDoOnceContentCommitted(const ECheckBoxState& InValue);
	void RefreshFromSelection(TWeakObjectPtr<UWireComponent> InWireComponent);
	void OutputShowingSuggestions(TArray<FString>& OutSuggestions);
	void TargetShowingSuggestions(TArray<FString>& OutSuggestions);
	
	void RefreshConnectionList(bool bResetSelection = false);
	
	void CancelChanges();
	void ApplyChanges();
};
