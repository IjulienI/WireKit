// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/WireKitDetails.h"

#include "EngineUtils.h"
#include "WireComponent.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/Input/SSuggestionTextBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"
#include "WireEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "SWireKitDetails"

const FEditableTextBoxStyle& TextBoxStyle = FAppStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox");

void SWireKitDetails::Construct(const FArguments& InArgs)
{
	this->ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)

			//---------------------------------------
			// Name Slot
			//---------------------------------------
			+SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(4.0f)
			[
				SNew(SHorizontalBox)
				
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("NameContent", "Name"))
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4.0f)
				[
					SAssignNew(NameEditableTextBox, SEditableTextBox) 
					.HintText(LOCTEXT("NameContent", "Actor Name..."))
					.MinDesiredWidth(200.0f)
					.SelectAllTextWhenFocused(true)
					.OnTextChanged_Lambda([this](const FText& New)
					{
						const bool bValid = !New.ToString().Contains(TEXT(" "));
						NameEditableTextBox->SetError(bValid ? FText::GetEmpty() : LOCTEXT("Space", "No space in Name"));
					})
					.OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
					{
						if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
						{
							LocalObjectName = *Text.ToString();
						}
					})
				]
			]

			+SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(4.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(4.0f)
				[
					SNew(SVerticalBox)

					//---------------------------------------
					// Tab bar
					//---------------------------------------
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(4.0f)
					[
						SNew(SSegmentedControl<int32>)
						.Value_Lambda([this]() { return ActiveTab; })
						.OnValueChanged_Lambda([this](int32 NewValue) { ActiveTab = NewValue; })
						+ SSegmentedControl<int32>::Slot(0).Text(LOCTEXT("OutputsTab", "Outputs"))
						+ SSegmentedControl<int32>::Slot(1).Text(LOCTEXT("InputsTab", "Inputs"))
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(4.0f)
					[
						SNew(SSeparator)
						.Orientation(Orient_Horizontal)
					]
					
					//---------------------------------------
					// Active menu
					//---------------------------------------
					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.Padding(4.0f)
					[
						SNew(SWidgetSwitcher)
						.WidgetIndex_Lambda([this]() { return ActiveTab; })

						//---------------------------------------
						// Outputs
						//---------------------------------------
						+ SWidgetSwitcher::Slot()
						[
							SNew(SVerticalBox)

							//---------------------------------------
							// Columns
							//---------------------------------------
							+SVerticalBox::Slot()
							.FillHeight(1.0f)
							.Padding(4.0f)
							[
								SAssignNew(ConnectionList, SListView<TSharedPtr<FWireConnection>>)
								.ListItemsSource(&Connections)
								.OnGenerateRow_Lambda([](const TSharedPtr<FWireConnection>& Connection, const TSharedRef<STableViewBase>& Owner)
								{
									return SNew(SWireKitDetailsMutliColumn, Owner).Connection(Connection);
								})
								.OnSelectionChanged_Lambda([this](const TSharedPtr<FWireConnection>& Connection, const ESelectInfo::Type& SelectInfo)
								{
									//if (SelectInfo == ESelectInfo::Type::)
									OnConnectionListSelectionChanged(Connection);
								})
								.HeaderRow
								(
									SNew(SHeaderRow)
									
									+SHeaderRow::Column("OutputName")
									.DefaultLabel(LOCTEXT("OutputNameColumn", "Output"))
									.FillWidth(2.0f)
									
									+SHeaderRow::Column("TargetEntity")
									.DefaultLabel(LOCTEXT("TargetEntityColumn", "Target"))
									.FillWidth(2.0f)
									
									+SHeaderRow::Column("TargetInput")
									.DefaultLabel(LOCTEXT("TargetInputColumn", "Input"))
									.FillWidth(2.0f)
									
									+SHeaderRow::Column("Parameter")
									.DefaultLabel(LOCTEXT("ParameterColumn", "Parameter"))
									.FillWidth(2.0f)
									
									+SHeaderRow::Column("Delay")
									.DefaultLabel(LOCTEXT("DelayColumn", "Delay"))
									.FillWidth(2.0f)
									
									+SHeaderRow::Column("bOnlyOnce")
									.DefaultLabel(LOCTEXT("bOnlyOnceColumn", "Once ?"))
									.FillWidth(2.0f)
								)
							]

							+SVerticalBox::Slot()
							.AutoHeight()
							.Padding(2)
							[
								SNew(SHorizontalBox)

								+SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(4,0,4,0)
								[
									SNew(SButton)
									.OnClicked_Lambda([this]()
									{
										AddConnectionRow();
										return FReply::Handled();
									})
									[
										SNew(SImage).Image(FAppStyle::GetBrush("Icons.Plus"))
									]
								]

								+SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(4,0,4,0)
								[
									SNew(SButton)
									.OnClicked_Lambda([this]()
									{
										CopyConnectionRow();
										return FReply::Handled();
									})
									[
										SNew(SImage).Image(FAppStyle::GetBrush("GenericCommands.Copy"))
									]
								]

								+SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(4,0,4,0)
								[
									SNew(SButton)
									.OnClicked_Lambda([this]()
									{
										PasteConnectionRow();
										return FReply::Handled();
									})
									[
										SNew(SImage).Image(FAppStyle::GetBrush("GenericCommands.Paste"))
									]
								]

								+SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(4,0,4,0)
								[
									SNew(SButton)
									.OnClicked_Lambda([this]()
									{
										DuplicateConnectionRow();
										return FReply::Handled();
									})
									[
										SNew(SImage).Image(FAppStyle::GetBrush("GenericCommands.Duplicate"))
									]
								]

								+SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(4,0,4,0)
								[
									SNew(SButton)
									.OnClicked_Lambda([this]()
									{
										RemoveConnectionRow();
										return FReply::Handled();
									})
									[
										SNew(SImage).Image(FAppStyle::GetBrush("Icons.Delete"))
									]
								]
							]

							+SVerticalBox::Slot()
							.AutoHeight()
							.Padding(4.0f)
							[
								SNew(SSeparator)
								.Orientation(Orient_Horizontal)
							]

							//---------------------------------------
							// Variable modification
							//---------------------------------------
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)
								.IsEnabled_Lambda([this]()
								{
									return ConnectionList->GetSelectedItems().Num() == 1;
								})
								+SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.Padding(4.0f)
								[
									SNew(SVerticalBox)

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("OutputContent", "My output"))
										]

										+SHorizontalBox::Slot()
										.FillWidth(1.0f)
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(OutputSuggestionTextBox, SSuggestionTextBox)
											.HintText(LOCTEXT("Outputs", "Output..."))
											.TextStyle(&TextBoxStyle)
											.ForegroundColor(TextBoxStyle.ForegroundColor)
											.MinDesiredWidth(100.0f)
											.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
											{
												OutputShowingSuggestions(OutSuggestions);
											})
											.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type CommitType)
											{
												if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
												{
													OnOutputContentCommitted(Text);
												}
											})
										]
									]

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("TargetContent", "Target name"))
										]

										+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(TargetSuggestionTextBox, SSuggestionTextBox)
											.HintText(LOCTEXT("Target", "Target..."))
											.TextStyle(&TextBoxStyle)
											.ForegroundColor(TextBoxStyle.ForegroundColor)
											.MinDesiredWidth(100.0f)
											.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
											{
												TargetShowingSuggestions(OutSuggestions);
											})
											.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type CommitType)
											{
												if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
												{
													OnTargetContentCommitted(Text);
												}
											})
										]
									]

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("InputContent", "Via this input"))
										]

										+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(InputSuggestionTextBox, SSuggestionTextBox)
											.HintText(LOCTEXT("Input", "Input..."))
											.TextStyle(&TextBoxStyle)
											.ForegroundColor(TextBoxStyle.ForegroundColor)
											.MinDesiredWidth(100.0f)
											.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
											{
												// TODO : All detected outputs
											})
											.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type CommitType)
											{
												if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
												{
													OnInputContentCommitted(Text);
												}
											})
										]
									]
								]

								+SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.Padding(4.0f)
								[
									SNew(SVerticalBox)

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("ParameterContent", "parameter"))
										]

										+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(ParameterSuggestionTextBox, SSuggestionTextBox)
											.HintText(LOCTEXT("Override", "<none>"))
											.TextStyle(&TextBoxStyle)
											.ForegroundColor(TextBoxStyle.ForegroundColor)
											.MinDesiredWidth(100.0f)
											.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
											{
												// TODO : All detected outputs
											})
											.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type CommitType)
											{
												if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
												{
													OnParameterContentCommitted(Text);
												}
											})
										]
									]

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("DelayContent", "delay (s)"))
										]

										+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(DelaySuggestionTextBox, SSuggestionTextBox)
											.HintText(LOCTEXT("Delay", "0"))
											.TextStyle(&TextBoxStyle)
											.ForegroundColor(TextBoxStyle.ForegroundColor)
											.MinDesiredWidth(100.0f)
											.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
											{
												// TODO : All detected outputs
											})
											.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type CommitType)
											{
												if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
												{
													OnDelayContentCommitted(Text);
												}
											})
										]
									]

									+SVerticalBox::Slot()
									.AutoHeight()
									.Padding(4.0f)
									[
										SNew(SHorizontalBox)

										+SHorizontalBox::Slot()
										.FillWidth(0.8f)
										.Padding(0.0f)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("DoOnceContent", "Fire once only"))
										]

										+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.Padding(0.0f)
										[
											SAssignNew(DoOnceSuggestionCheckBox ,SCheckBox)
											.Padding(4.0f)
											.OnCheckStateChanged_Lambda([this](const ECheckBoxState& CheckBoxState)
											{
												OnDoOnceContentCommitted(CheckBoxState);
											})
										]
									]
								]
							]
						]

						+ SWidgetSwitcher::Slot()
						[
							SNew(STextBlock).Text(LOCTEXT("InputsContent", "Contenu Inputs"))
						]
					]
				]
			]

			//---------------------------------------
			// Footer buttons
			//---------------------------------------
			+SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(4.0f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("CancelButton", "Cancel"))
					.OnClicked_Lambda([this]()
					{
						CancelChanges();
						return FReply::Handled();
					})
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4.0f)
				[
					SNew(SButton)
					.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("PrimaryButton"))
					.Text(LOCTEXT("ApplyButton", "Apply"))
					.OnClicked_Lambda([this]()
					{
						ApplyChanges();
						return FReply::Handled();
					})
				]
			]
		]
	];
	
	OnWireSelectionChangedHandle = GEditor->GetEditorSubsystem<UWireEditorSubsystem>()->OnWireSelectionChanged.AddRaw(this, &SWireKitDetails::OnSelectionChanged);
	OnSelectionChanged();
}

SWireKitDetails::~SWireKitDetails()
{
	GEditor->GetEditorSubsystem<UWireEditorSubsystem>()->OnWireSelectionChanged.Remove(OnWireSelectionChangedHandle);
}

bool SWireKitDetails::GetSelectedConnection(TSharedPtr<FWireConnection>& OutConnection) const
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return false;
	}
	OutConnection =  SelectedConnections[0];
	return true;
}

void SWireKitDetails::OnSelectionChanged()
{
	CurrentWireComponent = GEditor->GetEditorSubsystem<UWireEditorSubsystem>()->GetCurrentSelection();
	
	RefreshFromSelection(CurrentWireComponent);
}

void SWireKitDetails::OnConnectionListSelectionChanged(const TSharedPtr<FWireConnection>& Connection)
{
	if (Connection.IsValid())
	{
		OutputSuggestionTextBox.Get()->SetText(FText::FromString(Connection->OutputName.ToString()));
		TargetSuggestionTextBox.Get()->SetText(FText::FromString(Connection->TargetEntity.ToString()));
		InputSuggestionTextBox.Get()->SetText(FText::FromString(Connection->TargetInput.ToString()));
		ParameterSuggestionTextBox.Get()->SetText(FText::FromString(Connection->Parameter));
		DelaySuggestionTextBox.Get()->SetText(FText::Format(LOCTEXT("DelayColumnFormat", "{0} s"), Connection->Delay));
		DoOnceSuggestionCheckBox.Get()->SetIsChecked(Connection->bOnlyOnce ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}
	else
	{
		OutputSuggestionTextBox.Get()->SetText(FText::FromString(TEXT("Output...")));
		TargetSuggestionTextBox.Get()->SetText(FText::FromString(TEXT("Target...")));
		InputSuggestionTextBox.Get()->SetText(FText::FromString(TEXT("Input...")));
		ParameterSuggestionTextBox.Get()->SetText(FText::FromString(TEXT("<none>")));
		DelaySuggestionTextBox.Get()->SetText(FText::FromString(TEXT("0 s")));
		DoOnceSuggestionCheckBox.Get()->SetIsChecked(ECheckBoxState::Unchecked);
	}
}

void SWireKitDetails::AddConnectionRow()
{
	Connections.Add(MakeShared<FWireConnection>());
	RefreshConnectionList();
}

void SWireKitDetails::CopyConnectionRow()
{
	Clipboard.Empty();
	for (const TSharedPtr<FWireConnection>& Item : ConnectionList->GetSelectedItems())
	{
		if (Item.IsValid())
		{
			Clipboard.Add(MakeShared<FWireConnection>(*Item));
		}
	}
}

void SWireKitDetails::PasteConnectionRow()
{
	if (Clipboard.IsEmpty())
	{
		return;
	}
	
	for (TSharedPtr<FWireConnection> WireConnection : Clipboard)
	{
		if (WireConnection.IsValid())
		{
			auto NewConnection = *WireConnection;
			Connections.Add(MakeShared<FWireConnection>(NewConnection));
		}
	}
	RefreshConnectionList();
}

void SWireKitDetails::DuplicateConnectionRow()
{
	for (TSharedPtr<FWireConnection> WireConnection : ConnectionList->GetSelectedItems())
	{
		if (WireConnection.IsValid())
		{
			auto NewConnection = *WireConnection;
			Connections.Add(MakeShared<FWireConnection>(NewConnection));
		}
	}
	RefreshConnectionList();
}

void SWireKitDetails::RemoveConnectionRow()
{
	for (TSharedPtr<FWireConnection>& WireConnection : ConnectionList->GetSelectedItems())
	{
		if (WireConnection.IsValid())
		{
			Connections.Remove(WireConnection);
		}
	}
	RefreshConnectionList(true);
}

void SWireKitDetails::OnOutputContentCommitted(const FText& InText)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->OutputName = *InText.ToString();
}

void SWireKitDetails::OnTargetContentCommitted(const FText& InText)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->TargetEntity = *InText.ToString();
}

void SWireKitDetails::OnInputContentCommitted(const FText& InText)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->TargetInput = *InText.ToString();
}

void SWireKitDetails::OnParameterContentCommitted(const FText& InText)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->Parameter = *InText.ToString();
}

void SWireKitDetails::OnDelayContentCommitted(const FText& InText)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->Delay = FCString::Atof(*InText.ToString());
}

void SWireKitDetails::OnDoOnceContentCommitted(const ECheckBoxState& InValue)
{
	const auto SelectedConnections = ConnectionList->GetSelectedItems();
	if (SelectedConnections.Num() != 1 || !SelectedConnections[0].IsValid())
	{
		return;
	}
	SelectedConnections[0]->bOnlyOnce = InValue == ECheckBoxState::Checked;
}

void SWireKitDetails::RefreshFromSelection(TWeakObjectPtr<UWireComponent> InWireComponent)
{
	if (InWireComponent.IsValid())
	{
		Connections.Empty();
		auto WireComponent = InWireComponent.Get();
		for (FWireConnection Connection : WireComponent->GetConnections())
		{
			Connections.Add(MakeShared<FWireConnection>(Connection));
		}
		NameEditableTextBox->SetText(FText::FromName(WireComponent->GetObjectName()));
		ConnectionList->RequestListRefresh();
	}
}

void SWireKitDetails::OutputShowingSuggestions(TArray<FString>& OutSuggestions)
{
	TArray<FString> Suggestions;
	if (!CurrentWireComponent.IsValid())
	{
		OutSuggestions = Suggestions;
		return;
	}
	
	for (UFunction* WireFunction : CurrentWireComponent->GetAllWireFunctions())
	{
		Suggestions.Add(WireFunction->GetName());
	}
	OutSuggestions = Suggestions;
}

void SWireKitDetails::TargetShowingSuggestions(TArray<FString>& OutSuggestions)
{
	TArray<FString> Suggestions;
	
	OutSuggestions.Add("!Self");
	OutSuggestions.Add("!Activator");
	OutSuggestions.Add("!Caller");
	
	auto* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return;
	}
	
	// TODO : Use a cache of actor names thanks to the delagates (OnNewActorsPlaced, ...)
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		const AActor* Actor = *It;
		if (Actor)
		{
			const UWireComponent* WireComponent = Actor->GetComponentByClass<UWireComponent>();
			if (WireComponent)
			{
				OutSuggestions.Add(WireComponent->GetObjectName().ToString());
			}
		}
	}
}

void SWireKitDetails::CancelChanges()
{
	OnSelectionChanged();
}

void SWireKitDetails::ApplyChanges()
{	
	if (CurrentWireComponent.IsValid())
	{
		const FScopedTransaction Transaction(LOCTEXT("ApplyWire", "Edit Wire Connections"));
		CurrentWireComponent->Modify();
		
		CurrentWireComponent->SetObjectName(LocalObjectName);
		
		TArray<FWireConnection> NewConnections;
		NewConnections.Reserve(Connections.Num());
		for (const TSharedPtr<FWireConnection>& Connection : Connections)
		{
			NewConnections.Add(*Connection);
		}
		CurrentWireComponent->SetConnection(NewConnections);
	}
}

void SWireKitDetails::RefreshConnectionList(bool bResetSelection)
{
	if (bResetSelection)
	{
		ConnectionList->ClearSelection();
	}
	
	ConnectionList->RequestListRefresh();
	
	if (!ConnectionList->GetSelectedItems().IsEmpty())
	{
		ConnectionList->SetSelection(Connections.Last());
	}
}

#undef LOCTEXT_NAMESPACE
