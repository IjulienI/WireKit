// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireWorldSubsystem.h"

#include "WireComponent.h"
#include "WireLog.h"

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

void UWireWorldSubsystem::Tick(float DeltaTime)
{
    const double Now = GetWorld()->GetTimeSeconds();

    for (int32 i = PendingEvents.Num() - 1; i >= 0; --i)
    {
        if (PendingEvents[i].FireTime <= Now)
        {
            const FWirePendingEvent Event = PendingEvents[i];
            PendingEvents.RemoveAtSwap(i, 1, EAllowShrinking::No);
        }
    }
}

void UWireWorldSubsystem::QueueEvent(const FWireConnection& Connection, AActor* Caller, AActor* Activator)
{
    FWirePendingEvent Event;
    Event.FireTime = GetWorld()->GetTimeSeconds() + FMath::Max(0.f, Connection.Delay);
    Event.TargetEntity = Connection.TargetEntity;
    Event.TargetInput = Connection.TargetInput;
    Event.Parameter = Connection.Parameter;
    Event.Context.Caller = Caller;
    Event.Context.Activator = Activator;
    
    if (Connection.Delay <= 0.f)
    {
        ResolveAndDispatch(Event);
    }
    else
    {
        PendingEvents.Add(MoveTemp(Event));
    }
}

void UWireWorldSubsystem::CancelPending(AActor* Caller)
{
    for (int32 i = PendingEvents.Num() - 1; i >= 0; --i)
    {
        if (PendingEvents[i].Context.Caller.Get() == Caller)
        {
            PendingEvents.RemoveAtSwap(i, 1, EAllowShrinking::No);
        }
    }
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

void UWireWorldSubsystem::ResolveAndDispatch(const FWirePendingEvent& Event)
{
    // !self !caller !activator handle
    const FString Target = Event.TargetEntity.ToString();
    if (Target == "!self" || Target == "!caller")
    {
        if (auto* Caller = Event.Context.Caller.Get())
        {
            DispatchInput(Caller, Event);
            return;
        }
    }
    if (Target == "!activator")
    {
        if (auto* Activator = Event.Context.Activator.Get())
        {
            DispatchInput(Activator, Event);
            return;
        }
    }
    
    for (TPair<FName, TWeakObjectPtr<UWireComponent>>& Wire : Wires)
    {
        const auto* Component = Wire.Value.Get();
        if (!Component) continue;
        
        if (Wire.Key == Target)
        {
            if (Event.FireTime > 0.f)
            {
                PendingEvents.Add(Event);
                continue;
            }
            DispatchInput(Component->GetOwner(), Event);
        }
    }
    UE_LOG(LogWireKitRuntime, Warning, TEXT("UWireWorldSubsystem::ResolveAndDispatch - No target found"));
}

void UWireWorldSubsystem::DispatchInput(AActor* Target, const FWirePendingEvent& Event)
{
    // TODO : Find a way to use unreal reflection to find Event/Delegate input
}


