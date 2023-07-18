#include "zed/zed.h"
sl::PositionalTrackingParameters positionalTrackingParams;
sl::InitParameters initParams;
sl::BodyTrackingParameters bodyTrackingParams;
sl::Camera zedCam;
sl::Bodies bodies;
sl::BodyTrackingRuntimeParameters bodyTrackerParams;
void zed::init() {

#ifdef _SL_JETSON_
    const bool isJetson = true;
#else
    const bool isJetson = false;
#endif

    
    initParams.camera_resolution = sl::RESOLUTION::AUTO;
    initParams.depth_mode = sl::DEPTH_MODE::ULTRA;
    initParams.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;

    auto returnedState = zedCam.open(initParams);
    if(returnedState != sl::ERROR_CODE::SUCCESS) {
        printf("Open Camera\nExit program.");
        zedCam.close();
        return;
    }

    

    returnedState = zedCam.enablePositionalTracking(positionalTrackingParams);
    if (returnedState != sl::ERROR_CODE::SUCCESS) {
        printf("enable Positional Tracking\nExit program.");
        zedCam.close();
        return;
    }

    
    bodyTrackingParams.enable_tracking = true; // track people across images flow
    bodyTrackingParams.enable_body_fitting = true; // smooth skeletons moves
    bodyTrackingParams.body_format = sl::BODY_FORMAT::BODY_34;
    bodyTrackingParams.detection_model = isJetson ? sl::BODY_TRACKING_MODEL::HUMAN_BODY_FAST : sl::BODY_TRACKING_MODEL::HUMAN_BODY_ACCURATE;

    returnedState = zedCam.enableBodyTracking(bodyTrackingParams);
    if (returnedState != sl::ERROR_CODE::SUCCESS) {
        printf("enable Object Detection\nExit program.");
        zedCam.close();
        return;
    }

    bodyTrackerParams.detection_confidence_threshold = 40;

    auto camConfig = zedCam.getCameraInformation().camera_configuration;
}

sl::Bodies zed::retrieveBodies() {
    auto err = zedCam.grab();
    if(err == sl::ERROR_CODE::SUCCESS) {
        zedCam.retrieveBodies(bodies, bodyTrackerParams);        
    }
    return bodies;
}