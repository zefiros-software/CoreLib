
#pragma once
#include "api/controller.h"

#include "plugin/plugin.h"

class Test2
    : public PluginBase
{
public:

    void OnInit() override;

    U32 GetNumber();
};

DEFINE_PLUGIN( Test2 );