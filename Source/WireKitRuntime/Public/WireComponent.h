// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.h"
#include "Components/ActorComponent.h"
#include "WireComponent.generated.h"

/**
 * Connects this actor's events to actions on other actors, configured directly in the Details panel.
 * Pick an output, a target and an input, with an optional parameter, delay and fire limit. No code required.
 */
UCLASS(ClassGroup=(WireKit), meta=(BlueprintSpawnableComponent, DisplayName = "WireComponent"))
class WIREKITRUNTIME_API UWireComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWireComponent();
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
private:
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (AllowPrivateAccess = true))
    TArray<FWireOutput> Outputs;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (AllowPrivateAccess = true, EditCondition = false))
    TArray<FWireInput> Inputs;
};
