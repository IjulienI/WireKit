// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WireWorldSubsystem.generated.h"

struct FWireConnection;
class UWireComponent;

UCLASS()
class WIREKITRUNTIME_API UWireWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
    
public:
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

    //---------------------------------------
    // Debug
    //---------------------------------------
    UFUNCTION(BlueprintCallable, Category = "WireKit", meta = (DevelopmentOnly))
    TArray<UWireComponent*> GetWiresByName(const FName& WireName) const;
    
    UFUNCTION(BlueprintCallable, Category = "WireKit", meta = (DevelopmentOnly))
    TArray<FName> GetWireNames() const;
    
private:
    //---------------------------------------
    // Internal vars
    //---------------------------------------
    TMultiMap<FName, TWeakObjectPtr<UWireComponent>> Wires;
};
