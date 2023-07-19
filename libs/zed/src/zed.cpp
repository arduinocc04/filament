#include "zed/zed.h"
int zed::VZCam::init() {

#ifdef _SL_JETSON_
    const bool isJetson = true;
#else
    const bool isJetson = false;
#endif

    sl::InitParameters initParams;
    initParams.camera_resolution = sl::RESOLUTION::AUTO;
    initParams.depth_mode = sl::DEPTH_MODE::ULTRA;
    initParams.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;

    auto returnedState = zedCam.open(initParams);
    if(returnedState != sl::ERROR_CODE::SUCCESS) {
        printf("Open Camera\nExit program.: %d\n", (int)returnedState);
        zedCam.close();
        return -1;
    }

    zedCam.enablePositionalTracking();

    sl::BodyTrackingParameters bodyTrackingParams;
    bodyTrackingParams.enable_tracking = true; // track people across images flow
    bodyTrackingParams.enable_body_fitting = true; // smooth skeletons moves
    bodyTrackingParams.body_format = sl::BODY_FORMAT::BODY_38;
    bodyTrackingParams.detection_model = isJetson ? sl::BODY_TRACKING_MODEL::HUMAN_BODY_FAST : sl::BODY_TRACKING_MODEL::HUMAN_BODY_ACCURATE;
    
    returnedState = zedCam.enableBodyTracking(bodyTrackingParams);
    if (returnedState != sl::ERROR_CODE::SUCCESS) {
        printf("enable Object Detection\nExit program.: %d\n", (int)returnedState);
        zedCam.close();
        return -1;
    }

    bodyTrackerParams.detection_confidence_threshold = 40;

    auto camConfig = zedCam.getCameraInformation().camera_configuration;
    return 0;
}

sl::Bodies zed::VZCam::retrieveBodies() {
    auto err = zedCam.grab();
    if(err == sl::ERROR_CODE::SUCCESS) {
        zedCam.retrieveBodies(bodies, bodyTrackerParams);        
    }
    return bodies;
}