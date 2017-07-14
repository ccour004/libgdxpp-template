#include <LibGDXApplication.h>
#include <memory>
#include "MyAppListener.h"

int main(int argc, char* argv[])
{    
    /*std::shared_ptr<DesktopConfiguration> desktop = std::make_shared<DesktopConfiguration>();
    std::shared_ptr<MobileConfiguration> mobile = std::make_shared<MobileConfiguration>();
    desktop->fullscreen = true;
    desktop->setBetterAntiAlias();
    mobile->setBetterAntiAlias();*/
    
    std::unique_ptr<LibGDX_Application> app = std::make_unique<LibGDX_Application>(
        /*desktop,mobile,*/std::make_shared<MyAppListener>());
    return 0;
}