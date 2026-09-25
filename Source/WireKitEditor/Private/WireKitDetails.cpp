// Fill out your copyright notice in the Description page of Project Settings.


#include "WireKitDetails.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/Input/SSuggestionTextBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SWireKitDetails"

void SWireKitDetails::Construct(const FArguments& InArgs)
{
	this->ChildSlot
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
					if (CommitType == ETextCommit::OnEnter)
					{
						// TODO : Push the name
					}
				})
			]
		]

		+SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(4.0f)
		[
			SNew(SBorder)
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
										SNew(SSuggestionTextBox)
										.HintText(LOCTEXT("Outputs", "Output..."))
										.MinDesiredWidth(100.0f)
										.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
										{
											// TODO : All detected outputs
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
										SNew(SSuggestionTextBox)
										.HintText(LOCTEXT("Target", "Target..."))
										.MinDesiredWidth(100.0f)
										.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
										{
											// TODO : All detected outputs
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
										SNew(SSuggestionTextBox)
										.HintText(LOCTEXT("Input", "Input..."))
										.MinDesiredWidth(100.0f)
										.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
										{
											// TODO : All detected outputs
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
										SNew(SSuggestionTextBox)
										.HintText(LOCTEXT("Override", "<none>"))
										.MinDesiredWidth(100.0f)
										.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
										{
											// TODO : All detected outputs
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
										SNew(SSuggestionTextBox)
										.HintText(LOCTEXT("Delay", "0"))
										.MinDesiredWidth(100.0f)
										.OnShowingSuggestions_Lambda([this](const FString& Text, TArray<FString>& OutSuggestions)
										{
											// TODO : All detected outputs
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
										SNew(SCheckBox)
										.Padding(4.0f)
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
					return FReply::Handled();
				})
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
