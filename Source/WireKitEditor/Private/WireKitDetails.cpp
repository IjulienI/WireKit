// Fill out your copyright notice in the Description page of Project Settings.


#include "WireKitDetails.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SWireKitDetails"

void SWireKitDetails::Construct(const FArguments& InArgs)
{
	this->ChildSlot
	[
		SNew(SVerticalBox)

		// Tab bar
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

		// Active contenu
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(4.0f)
		[
			SNew(SWidgetSwitcher)
			.WidgetIndex_Lambda([this]() { return ActiveTab; })

			+ SWidgetSwitcher::Slot()
			[
				SNew(STextBlock).Text(LOCTEXT("OutputsContent", "Contenu Outputs"))
			]

			+ SWidgetSwitcher::Slot()
			[
				SNew(STextBlock).Text(LOCTEXT("InputsContent", "Contenu Inputs"))
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
