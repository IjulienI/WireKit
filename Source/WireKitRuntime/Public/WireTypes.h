// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.generated.h"

USTRUCT(BlueprintType)
struct FWireConnection
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
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (ClampMin = "-1"))
    int32 TimesToFire = -1;

    UPROPERTY(Transient)
    int32 FireCount = 0;
};

USTRUCT(BlueprintType)
struct FWireContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AActor> Caller;
    UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AActor> Activator;
};
