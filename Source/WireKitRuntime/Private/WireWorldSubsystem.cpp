// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireWorldSubsystem.h"

#include "WireComponent.h"

void UWireWorldSubsystem::RegisterObject(UWireComponent* WireComponent)
{
    if (WireComponent)
    {
        const FName ObjectName = WireComponent->GetObjectName();
        if (!ObjectName.IsNone())
        {
            Wires.Add(ObjectName, WireComponent);
        }
    }
}

void UWireWorldSubsystem::UnregisterObject(UWireComponent* WireComponent)
{
    if (WireComponent)
    {
        const FName ObjectName = WireComponent->GetObjectName();
        if (!ObjectName.IsNone())
        {
            Wires.RemoveSingle(ObjectName, WireComponent);
        }
    }
}

void UWireWorldSubsystem::QueueEvent(const FWireConnection& Connection, AActor* Caller, AActor* Activator)
{
    // TODO : Find UFunction from Connection
}

//---------------------------------------
// Debug
//---------------------------------------
TArray<UWireComponent*> UWireWorldSubsystem::GetWiresByName(const FName& WireName) const
{
    TArray<TWeakObjectPtr<UWireComponent>> Found;
    Wires.MultiFind(WireName, Found);

    TArray<UWireComponent*> Result;
    Result.Reserve(Found.Num());
    for (const TWeakObjectPtr<UWireComponent>& Wire : Found)
    {
        if (UWireComponent* Valid = Wire.Get())
        {
            Result.Add(Valid);
        }
    }
    return Result;
}

TArray<FName> UWireWorldSubsystem::GetWireNames() const
{
    TArray<FName> Result;
    Wires.GetKeys(Result);
    return Result;
}
