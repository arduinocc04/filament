#include <cstdio>
#include <sl/Camera.hpp>

namespace zed{
    /*class VZCam{ // Viuron-Zed Camera
    private:
        sl::Camera zedCam;
        sl::Bodies bodies;
        sl::BodyTrackingRuntimeParameters bodyTrackerParams;
    public:
    };*/
void init();
sl::Bodies retrieveBodies();
}