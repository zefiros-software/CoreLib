
#pragma once
#include "api/controller.h"

#include "plugin/plugin.h"

class Test3
    : public PluginBase
{
public:

    void OnInit() override;

    U32 GetNumber();
};


DEFINE_PLUGIN( Test3 );