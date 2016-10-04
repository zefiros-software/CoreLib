
#pragma once
#include "api/controller.h"

#include "plugin/plugin.h"

class Test4Plugin
    : public PluginBase
{
public:

    void OnInit() override;

    U32 GetNumber();
};

namespace Test4
{
    EXPOSE_CONTROLLER_API( Test4Plugin, GetNumber );
    EXPOSE_CONTROLLER_API( Test4Plugin, IsInitialised );
}

DEFINE_PLUGIN( Test4Plugin );