// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "WireWorldSubsystem.generated.h"

class UWireComponent;

struct FWirePendingEvent
{
    double FireTime = 0.0;
    FName TargetEntity;
    FName TargetInput;
    FString Parameter;
    FWireContext Context;
};

UCLASS()
class WIREKITRUNTIME_API UWireWorldSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()
    
public:
    //---------------------------------------
    // FTickableGameObject Overrides
    //---------------------------------------
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override  { RETURN_QUICK_DECLARE_CYCLE_STAT(UWireWorldSubsystem, STATGROUP_Tickables); }
    virtual bool IsTickable() const override { return PendingEvents.Num() > 0; }
    
    //---------------------------------------
    // Registering
    //---------------------------------------
    UFUNCTION(BlueprintCallable, Category = "WireKit")
    void RegisterObject(UWireComponent* WireComponent);
    
    UFUNCTION(BlueprintCallable, Category = "WireKit")
    void UnregisterObject(UWireComponent* WireComponent);

    //---------------------------------------
    // Public API
    //---------------------------------------
    void QueueEvent(const FWireConnection& Connection, AActor* Caller, AActor* Activator);
    void CancelPending(AActor* Caller);
    
private:
    //---------------------------------------
    // Internal func
    //---------------------------------------
    void ResolveAndDispatch(const FWirePendingEvent& Event);
    void DispatchInput(AActor* Target, const FWirePendingEvent& Event);
    
    //---------------------------------------
    // Internal vars
    //---------------------------------------
    TMultiMap<FName, TWeakObjectPtr<UWireComponent>> Wires;
    TArray<FWirePendingEvent> PendingEvents;
};
