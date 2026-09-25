// Fill out your copyright notice in the Description page of Project Settings.


#include "WireKitDetailsMutliColumn.h"

#define LOCTEXT_NAMESPACE "SWireKitDetailsMutliColumn"

void SWireKitDetailsMutliColumn::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& Owner)
{
	Connection = InArgs._Connection;
	FSuperRowType::Construct(FSuperRowType::FArguments().Padding(2.0f), Owner);
}

TSharedRef<SWidget> SWireKitDetailsMutliColumn::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("OutputName"))
	{
		return SNew(STextBlock).Text(FText::FromName(Connection->OutputName));
	}
	if (InColumnName == TEXT("TargetEntity"))
	{
		return SNew(STextBlock).Text(FText::FromName(Connection->TargetEntity));
	}
	if (InColumnName == TEXT("TargetInput"))
	{
		return SNew(STextBlock).Text(FText::FromName(Connection->TargetInput));
	}
	if (InColumnName == TEXT("Parameter"))
	{
		return SNew(STextBlock).Text(FText::FromString(Connection->Parameter));
	}
	if (InColumnName == TEXT("Delay"))
	{
		return SNew(STextBlock).Text(FText::Format(LOCTEXT("DelayColumnFormat", "{0} s"), Connection->Delay));
	}
	if (InColumnName == TEXT("bOnlyOnce"))
	{
		return SNew(SCheckBox).IsChecked(Connection->bOnlyOnce ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}
	return SNullWidget::NullWidget;
}

#undef LOCTEXT_NAMESPACE