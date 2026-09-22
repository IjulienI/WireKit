// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.generated.h"

USTRUCT(BlueprintType)
struct FWireOutput
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "My output name"))
    FName OutputName = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "Targets entities named"))
    FName TargetEntity = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "Via this input"))
    FName TargetInput = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "With a parameter override of"))
    FString Parameter = FString();
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "After a delay in seconds of", ClampMin = 0, Units = "Seconds"))
    float Delay = 0.0f;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ToolTip = "Fire once only"))
    bool bOnlyOnce = false;
};

USTRUCT(BlueprintType)
struct FWireInput
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    FName Source = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    FName Output = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    FName MyInput = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    FString Parameter = FString();
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    float Delay = 0.0f;
    
    UPROPERTY(EditAnywhere, Category = "WireKit")
    bool bOnlyOnce = false;
};