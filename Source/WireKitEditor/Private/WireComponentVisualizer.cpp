// Copyright (c) 2026 Julien soum. Licensed under the MIT License.


#include "WireComponentVisualizer.h"

#include "WireComponent.h"
#include "WireEditorSubsystem.h"

void FWireComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                 FPrimitiveDrawInterface* PDI)
{
    const UWireComponent* WireComponent = Cast<const UWireComponent>(Component);
    if (!WireComponent)
    {
        return;
    }
    
    TArray<FVector> TargetLocations;
    const auto& Wires = GEditor->GetEditorSubsystem<UWireEditorSubsystem>()->GetWires();
    for (const FWireConnection& Connection : WireComponent->GetConnections())
    {
        TArray<TWeakObjectPtr<UWireComponent>> TargetComponents;
        Wires.MultiFind(Connection.TargetEntity, TargetComponents);
        for (const TWeakObjectPtr<UWireComponent>& TargetComponent : TargetComponents)
        {
            TargetLocations.Add(TargetComponent->GetOwner()->GetActorLocation());
        }
    }
    
    const FVector OwnerLocation = WireComponent->GetOwner()->GetActorLocation();
    for (const FVector& TargetLocation : TargetLocations)
    {
        PDI->DrawLine(OwnerLocation, TargetLocation, FColor::Red, 0, 1.0f);
    }
}
