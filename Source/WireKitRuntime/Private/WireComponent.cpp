// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireComponent.h"

#include "WireWorldSubsystem.h"


UWireComponent::UWireComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWireComponent::BeginPlay()
{
    Super::BeginPlay();

    //---------------------------------------
    // Register
    //---------------------------------------
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    if (auto* Subsystem = World->GetSubsystem<UWireWorldSubsystem>())
    {
        WireSubsystem = Subsystem;
        WireSubsystem->RegisterObject(this);
    }
    else
    {
        ensureAlwaysMsgf(false, TEXT("UWireComponent::BeginPlay(): Subsystem is nullptr"));
    }
}

void UWireComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //---------------------------------------
    // UnRegister
    //---------------------------------------
    if (WireSubsystem.IsValid())
    {
        WireSubsystem->UnregisterObject(this);
    }
    WireSubsystem.Reset();
    
    Super::EndPlay(EndPlayReason);
}

void UWireComponent::FireOutput(FName OutputName, AActor* Activator)
{
    if (!WireSubsystem.IsValid())
    {
        return;
    }
    
    for (FWireConnection& Connection : Connections)
    {
        if (Connection.OutputName != OutputName) continue;
        //if (Connection.TimesToFire >= 0 && Connection.FireCount >= Connection.TimesToFire) continue;
        
        ++Connection.FireCount;
        WireSubsystem->QueueEvent(Connection, GetOwner(), Activator);
    }
}

#if WITH_EDITOR
TArray<UFunction*> UWireComponent::GetAllWireFunctions() const
{
    TArray<UFunction*> OutFunctions;
    
    for (TFieldIterator<UFunction> FuncIt(GetOwner()->GetClass()); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        if (Function->GetMetaData("Category") == "WireKit")
        {
            OutFunctions.Add(Function);
        }
    }
    return OutFunctions;
}
#endif
