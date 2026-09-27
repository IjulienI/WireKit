// Copyright (c) 2026 Julien soum. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "WireTypes.h"
#include "Components/ActorComponent.h"
#include "WireComponent.generated.h"

class UWireWorldSubsystem;

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

    //---------------------------------------
    // Overrides
    //---------------------------------------
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //---------------------------------------
    // Getters
    //---------------------------------------
    FName GetObjectName() const { return Name; }
    const TArray<FWireConnection>& GetConnections() const { return Connections; }

    //---------------------------------------
    // Setter
    //---------------------------------------
    void SetObjectName(const FName& NewName) { Name = NewName; }
    void SetConnection(const TArray<FWireConnection>& NewConnections) { Connections = NewConnections; }

    //---------------------------------------
    // Functions
    //---------------------------------------
    UFUNCTION(BlueprintCallable, Category = "CoreWireKit")
    void FireOutput(FName OutputName, AActor* Activator);

    //---------------------------------------
    // Accessible functions
    //---------------------------------------
    UFUNCTION(Category = "WireKit")
    void Destroy();
    
    UFUNCTION(Category = "WireKit")
    void DestroyHierarchy();
    
    UFUNCTION(Category = "WireKit")
    void Respawn();
    
    UFUNCTION(Category = "WireKit")
    void EnableTick();
    
    UFUNCTION(Category = "WireKit")
    void DisableTick();
    
    UFUNCTION(Category = "WireKit")
    void Show();
    
    UFUNCTION(Category = "WireKit")
    void Hide();
    
    UFUNCTION(Category = "WireKit")
    void EnableCollision();
    
    UFUNCTION(Category = "WireKit")
    void DisableCollision();
    
#if WITH_EDITOR
    TArray<UFunction*> GetAllInputs() const;
    TArray<FMulticastDelegateProperty*> GetAllOutputs() const;
#endif
    
private:
    //---------------------------------------
    // Internal vars
    //---------------------------------------
    UPROPERTY(EditAnywhere, Category = "WireKit")
    FName Name = NAME_None;
    
    UPROPERTY(EditAnywhere, Category = "WireKit", meta = (AllowPrivateAccess = true))
    TArray<FWireConnection> Connections;

    //---------------------------------------
    // Cache
    //---------------------------------------
    TWeakObjectPtr<UWireWorldSubsystem> WireSubsystem;
};
