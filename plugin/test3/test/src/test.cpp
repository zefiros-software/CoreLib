
#include "api/controller.h"

#include "plugin/plugin.h"

class Test
    : public PluginBase
{
public:

    void OnInit() override
    {
        std::string name = Controller::Get< Test >()->GetName();
    }
};

SET_PLUGIN( Test );