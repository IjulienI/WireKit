// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireComponent.h"


UWireComponent::UWireComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UWireComponent::BeginPlay()
{
    Super::BeginPlay();
    
}

void UWireComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    
    Super::EndPlay(EndPlayReason);
}
