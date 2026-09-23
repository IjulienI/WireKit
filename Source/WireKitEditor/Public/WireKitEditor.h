#pragma once

#include "CoreMinimal.h"
#include "WireMenu.h"
#include "Modules/ModuleManager.h"

class FWireKitEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    static const FName WireKitDetailsTabId;
private:
    void BindGlobalWireKitEditorCommands();
    TSharedRef<SDockTab> SpawnWireKitDetailsTab(const FSpawnTabArgs& Args);
};
