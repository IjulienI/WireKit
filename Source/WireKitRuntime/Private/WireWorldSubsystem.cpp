// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireWorldSubsystem.h"

#include "WireComponent.h"
#include "WireRuntimeLog.h"

namespace WireKeywords
{
    inline const FName Self(TEXT("!Self"));
    inline const FName Caller(TEXT("!Caller"));
    inline const FName Activator(TEXT("!Activator"));
}

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
            ResolveAndDispatch(Event);
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
    Event.Context.Self = Caller;
    // TODO : Find a way to get the real Caller
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

void UWireWorldSubsystem::ResolveAndDispatch(const FWirePendingEvent& Event)
{
    // !self !caller !activator handle
    const FName Target = Event.TargetEntity;
    
    if (Target == WireKeywords::Self)
    {
        if (AActor* Self = Event.Context.Self.Get())
        {
            DispatchInput(Self, Event);
        }
        else
        {
            UE_LOG(LogWireKitRuntime, Verbose,
                TEXT("ResolveAndDispatch - '%s' is no longer valid, input '%s' skipped"),
                *Target.ToString(), *Event.TargetInput.ToString());
        }
        return;
    }
    if (Target == WireKeywords::Caller)
    {
        if (AActor* Caller = Event.Context.Caller.Get())
        {
            DispatchInput(Caller, Event);
        }
        else
        {
            UE_LOG(LogWireKitRuntime, Verbose,
                TEXT("ResolveAndDispatch - '%s' is no longer valid, input '%s' skipped"),
                *Target.ToString(), *Event.TargetInput.ToString());
        }
        return;
    }
    if (Target == WireKeywords::Activator)
    {
        if (AActor* Activator = Event.Context.Activator.Get())
        {
            DispatchInput(Activator, Event);
        }
        else
        {
            UE_LOG(LogWireKitRuntime, Verbose,
                TEXT("ResolveAndDispatch - '%s' is no longer valid, input '%s' skipped"),
                *Target.ToString(), *Event.TargetInput.ToString());
        }
        return;
    }
    
    TArray<TWeakObjectPtr<UWireComponent>> Found;
    Wires.MultiFind(Target, Found);

    bool bDispatched = false;
    for (const TWeakObjectPtr<UWireComponent>& Wire : Found)
    {
        const UWireComponent* Component = Wire.Get();
        if (!Component)
        {
            Wires.RemoveSingle(Target, Wire);
            continue;
        }

        if (AActor* Owner = Component->GetOwner())
        {
            DispatchInput(Owner, Event);
            bDispatched = true;
        }
    }

    if (!bDispatched)
    {
        UE_LOG(LogWireKitRuntime, Warning,
            TEXT("ResolveAndDispatch - No target named '%s' for input '%s' (caller: %s)"),
            *Target.ToString(), *Event.TargetInput.ToString(),
            *GetNameSafe(Event.Context.Caller.Get()));
    }
}

void UWireWorldSubsystem::DispatchInput(AActor* Target, const FWirePendingEvent& Event)
{
    if (!Target) return;
    
    UFunction* Function = Target->FindFunction(Event.TargetInput);

    if (!Function)
    {
        UE_LOG(LogWireKitRuntime, Warning,
            TEXT("DispatchInput - No function named '%s' in actor '%s' (caller: %s)"),
            *Event.TargetInput.ToString(),*GetNameSafe(Target),
            *GetNameSafe(Event.Context.Caller.Get()));
        return;
    }
    
    
    if (Function->ParmsSize == 0)
    {
        Target->ProcessEvent(Function, nullptr);
    }
    
    // TODO : Add function param handle
}


