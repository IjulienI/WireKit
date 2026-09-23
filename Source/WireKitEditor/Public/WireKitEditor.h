#pragma once

#include "CoreMinimal.h"
#include "WireMenu.h"
#include "Modules/ModuleManager.h"

class FWireKitEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    void BindGlobalWireKitEditorCommands();
};
