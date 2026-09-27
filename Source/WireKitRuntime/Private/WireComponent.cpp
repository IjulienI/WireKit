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
    UWireComponent* ActivatorComponent  = Activator->FindComponentByClass<UWireComponent>();
    if (!WireSubsystem.IsValid() || !ActivatorComponent)
    {
        return;
    }
    
    for (FWireConnection& Connection : Connections)
    {
        if (Connection.OutputName != OutputName) continue;
        //if (Connection.TimesToFire >= 0 && Connection.FireCount >= Connection.TimesToFire) continue;
        
        ++Connection.FireCount;
        WireSubsystem->QueueEvent(Connection, this, ActivatorComponent);
    }
}

void UWireComponent::Destroy()
{
    GetOwner()->Destroy();
}

void UWireComponent::DestroyHierarchy()
{
    // TODO : Find all liked actors
    GetOwner()->Destroy();
}

void UWireComponent::Respawn()
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !Owner)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewActor = World->SpawnActor(Owner->GetClass(), &Owner->GetActorTransform(), SpawnParams);
    if (!NewActor)
    {
        return;
    }

    if (UWireComponent* NewWireComponent = NewActor->FindComponentByClass<UWireComponent>())
    {
        if (WireSubsystem.IsValid())
        {
            WireSubsystem->UnregisterObject(NewWireComponent);
        }

        NewWireComponent->SetObjectName(GetObjectName());
        NewWireComponent->SetConnection(GetConnections());

        if (NewWireComponent->WireSubsystem.IsValid())
        {
            NewWireComponent->WireSubsystem->RegisterObject(NewWireComponent);
        }
    }

    Owner->Destroy();
}

void UWireComponent::EnableTick()
{
    GetOwner()->SetActorTickEnabled(true);
}

void UWireComponent::DisableTick()
{
    GetOwner()->SetActorTickEnabled(false);
}

void UWireComponent::Show()
{
    GetOwner()->SetActorHiddenInGame(false);
}

void UWireComponent::Hide()
{
    GetOwner()->SetActorHiddenInGame(true);
}

void UWireComponent::EnableCollision()
{
    GetOwner()->SetActorEnableCollision(true);
}

void UWireComponent::DisableCollision()
{
    GetOwner()->SetActorEnableCollision(false);
}

#if WITH_EDITOR
TArray<UFunction*> UWireComponent::GetAllInputs() const
{
    TArray<UFunction*> OutFunctions;

    //---------------------------------------
    // Owner functions
    //---------------------------------------
    for (TFieldIterator<UFunction> FuncIt(GetOwner()->GetClass()); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        if (Function->GetMetaData("Category") == "WireKit")
        {
            OutFunctions.Add(Function);
        }
    }

    //---------------------------------------
    // Self functions
    //---------------------------------------
    for (TFieldIterator<UFunction> FuncIt(GetClass()); FuncIt; ++FuncIt)
    {
        UFunction* Function = *FuncIt;
        if (Function->GetMetaData("Category") == "WireKit")
        {
            OutFunctions.Add(Function);
        }
    }
    return OutFunctions;
}

TArray<FMulticastDelegateProperty*> UWireComponent::GetAllOutputs() const
{
    TArray<FMulticastDelegateProperty*> OutDelegates;

    //---------------------------------------
    // Owner delegates
    //---------------------------------------
    for (TFieldIterator<FMulticastDelegateProperty> PropIt(GetOwner()->GetClass()); PropIt; ++PropIt)
    {
        FMulticastDelegateProperty* Property = *PropIt;
        if (Property->GetMetaData("Category") == "WireKit")
        {
            OutDelegates.Add(Property);
        }
    }

    //---------------------------------------
    // Self delegates
    //---------------------------------------
    for (TFieldIterator<FMulticastDelegateProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        FMulticastDelegateProperty* Property = *PropIt;
        if (Property->GetMetaData("Category") == "WireKit")
        {
            OutDelegates.Add(Property);
        }
    }
    return OutDelegates;
}
#endif
