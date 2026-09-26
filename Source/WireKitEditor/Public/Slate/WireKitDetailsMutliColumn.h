// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.h"

/**
 * 
 */
class WIREKITEDITOR_API SWireKitDetailsMutliColumn : public SMultiColumnTableRow<TSharedPtr<FWireConnection>>
{
public:
	SLATE_BEGIN_ARGS(SWireKitDetailsMutliColumn) {}
		SLATE_ARGUMENT(TSharedPtr<FWireConnection>, Connection)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& Owner);
	
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
	
private:
	TSharedPtr<FWireConnection> Connection;
};
