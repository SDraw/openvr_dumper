#include "stdafx.h"

const std::map<vr::PropertyTypeTag_t, const char*> g_PropertyTypeTagName
{
    { vr::k_unInvalidPropertyTag, "invalid" },
    { vr::k_unFloatPropertyTag, "float" },
    { vr::k_unInt32PropertyTag, "int32" },
    { vr::k_unUint64PropertyTag, "uint64" },
    { vr::k_unBoolPropertyTag, "bool" },
    { vr::k_unStringPropertyTag, "string" },
    { vr::k_unErrorPropertyTag, "error" },
    { vr::k_unDoublePropertyTag, "double" },
    { vr::k_unHmdMatrix34PropertyTag, "mat3x4" },
    { vr::k_unHmdMatrix44PropertyTag, "mat4x4" },
    { vr::k_unHmdVector3PropertyTag, "vec3" },
    { vr::k_unHmdVector4PropertyTag, "vec4" },
    { vr::k_unHmdVector2PropertyTag, "vec2" },
    { vr::k_unHmdQuadPropertyTag, "quad" },
    { vr::k_unHiddenAreaPropertyTag, "hidden_area" },
    { vr::k_unPathHandleInfoTag, "path_handle_info" },
    { vr::k_unActionPropertyTag, "action" },
    { vr::k_unInputValuePropertyTag, "input_value" },
    { vr::k_unWildcardPropertyTag, "wildcard" },
    { vr::k_unHapticVibrationPropertyTag, "haptic_vibration" },
    { vr::k_unSkeletonPropertyTag, "skeleton" },
    { vr::k_unSpatialAnchorPosePropertyTag, "spatial_anchor_pose" },
    { vr::k_unJsonPropertyTag, "json" },
    { vr::k_unActiveActionSetPropertyTag, "active_action_set" },
    { vr::k_unOpenVRInternalReserved_Start, "openvr_internal_reserved_start" },
    { vr::k_unOpenVRInternalReserved_End, "openvr_internal_reserved_end" }
};
const char* g_DeviceTypeName[vr::TrackedDeviceClass_Max]
{
    "invalid", "hmd", "controller", "generic_tracker", "tracking_reference", "display_redirect"
};

struct StoredProperty
{
    vr::ETrackedDeviceProperty m_vrProperty = vr::Prop_Invalid;
    const char *m_propertyName;
    vr::PropertyTypeTag_t m_propertyTypeTag = vr::k_unInvalidPropertyTag;
    bool m_stored = false;

    union
    {
        bool m_bool = false;
        float m_float;
        int32_t m_int32;
        uint64_t m_uint64;
        vr::HmdMatrix34_t m_matrix34;
        vr::HmdVector3_t m_vector3;
    };
    std::string m_string;

    StoredProperty(vr::ETrackedDeviceProperty f_prop, const char *f_name, vr::PropertyTypeTag_t f_tag)
    {
        m_vrProperty = f_prop;
        m_propertyName = f_name;
        m_propertyTypeTag = f_tag;
    }
};

int main()
{
    vr::EVRInitError l_vrInitError;
    vr::IVRSystem *l_vrSystem = vr::VR_Init(&l_vrInitError, vr::VRApplication_Background);
    if(l_vrInitError == vr::VRInitError_None)
    {
        std::cout << "OpenVR initialized" << std::endl;

        const auto l_shortDelay = std::chrono::seconds(1U);
        std::cout << "Activate your devices in period of 3 seconds ";
        for(size_t i = 0U; i < 3U; i++)
        {
            std::cout << '.';
            std::this_thread::sleep_for(l_shortDelay);
        }
        std::cout << std::endl;

        pugi::xml_document *l_xmlFile = new pugi::xml_document();
        pugi::xml_node l_rootNode = l_xmlFile->append_child("root");

        for(int l_deviceClass = vr::TrackedDeviceClass_Invalid; l_deviceClass < vr::TrackedDeviceClass_Max; l_deviceClass++)
        {
            std::vector<StoredProperty> l_storedProperties;

            // General properties
            l_storedProperties.emplace_back(vr::Prop_TrackingSystemName_String, "TrackingSystemName", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ModelNumber_String, "ModelNumber", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_SerialNumber_String, "SerialNumber", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_RenderModelName_String, "RenderModelName", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_WillDriftInYaw_Bool, "WillDriftInYaw", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ManufacturerName_String, "ManufacturerName", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_TrackingFirmwareVersion_String, "TrackingFirmwareVersion", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HardwareRevision_String, "HardwareRevision", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_AllWirelessDongleDescriptions_String, "AllWirelessDongleDescriptions", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ConnectedWirelessDongle_String, "ConnectedWirelessDongle", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceIsWireless_Bool, "DeviceIsWireless", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceIsCharging_Bool, "DeviceIsCharging", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceBatteryPercentage_Float, "DeviceBatteryPercentage", vr::k_unFloatPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_StatusDisplayTransform_Matrix34, "StatusDisplayTransform", vr::k_unHmdMatrix34PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_UpdateAvailable_Bool, "Firmware_UpdateAvailable", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ManualUpdate_Bool, "Firmware_ManualUpdate", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ManualUpdateURL_String, "Firmware_ManualUpdateURL", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HardwareRevision_Uint64, "HardwareRevision", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_FirmwareVersion_Uint64, "FirmwareVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_FPGAVersion_Uint64, "FPGAVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_VRCVersion_Uint64, "VRCVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_RadioVersion_Uint64, "RadioVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DongleVersion_Uint64, "DongleVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_BlockServerShutdown_Bool, "BlockServerShutdown", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_CanUnifyCoordinateSystemWithHmd_Bool, "CanUnifyCoordinateSystemWithHmd", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ContainsProximitySensor_Bool, "ContainsProximitySensor", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceProvidesBatteryStatus_Bool, "DeviceProvidesBatteryStatus", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceCanPowerOff_Bool, "DeviceCanPowerOff", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ProgrammingTarget_String, "Firmware_ProgrammingTarget", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DeviceClass_Int32, "DeviceClass", vr::k_unInt32PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasCamera_Bool, "HasCamera", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_DriverVersion_String, "DriverVersion", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ForceUpdateRequired_Bool, "Firmware_ForceUpdateRequired", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ViveSystemButtonFixRequired_Bool, "ViveSystemButtonFixRequired", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ParentDriver_Uint64, "ParentDriver", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_ResourceRoot_String, "ResourceRoot", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_RegisteredDeviceType_String, "RegisteredDeviceType", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_InputProfilePath_String, "InputProfilePath", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NeverTracked_Bool, "NeverTracked", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NumCameras_Int32, "NumCameras", vr::k_unInt32PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_CameraFrameLayout_Int32, "CameraFrameLayout", vr::k_unInt32PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_CameraStreamFormat_Int32, "CameraStreamFormat", vr::k_unInt32PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_AdditionalDeviceSettingsPath_String, "AdditionalDeviceSettingsPath", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Identifiable_Bool, "Identifiable", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_BootloaderVersion_Uint64, "BootloaderVersion", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_AdditionalSystemReportData_String, "AdditionalSystemReportData", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_CompositeFirmwareVersion_String, "CompositeFirmwareVersion", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_Firmware_RemindUpdate_Bool, "Firmware_RemindUpdate", vr::k_unBoolPropertyTag);

            switch(l_deviceClass)
            {
                case vr::TrackedDeviceClass_HMD:
                {
                    l_storedProperties.emplace_back(vr::Prop_ReportsTimeSinceVSync_Bool, "ReportsTimeSinceVSync", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_SecondsFromVsyncToPhotons_Float, "SecondsFromVsyncToPhotons", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFrequency_Float, "DisplayFrequency", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_UserIpdMeters_Float, "UserIpdMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CurrentUniverseId_Uint64, "CurrentUniverseId", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_PreviousUniverseId_Uint64, "PreviousUniverseId", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFirmwareVersion_Uint64, "DisplayFirmwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_IsOnDesktop_Bool, "IsOnDesktop", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCType_Int32, "DisplayMCType", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCOffset_Float, "DisplayMCOffset", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCScale_Float, "DisplayMCScale", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_EdidVendorID_Int32, "EdidVendorID", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageLeft_String, "DisplayMCImageLeft", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageRight_String, "DisplayMCImageRight", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCBlackClamp_Float, "DisplayGCBlackClamp", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_EdidProductID_Int32, "EdidProductID", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CameraToHeadTransform_Matrix34, "CameraToHeadTransform", vr::k_unHmdMatrix34PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCType_Int32, "DisplayGCType", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCOffset_Float, "DisplayGCOffset", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCScale_Float, "DisplayGCScale", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCPrescale_Float, "DisplayGCPrescale", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCImage_String, "DisplayGCImage", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCImage_String, "DisplayGCImage", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterLeftU_Float, "LensCenterLeftU", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterLeftV_Float, "LensCenterLeftV", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterRightU_Float, "LensCenterRightU", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterRightV_Float, "LensCenterRightV", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_UserHeadToEyeDepthMeters_Float, "UserHeadToEyeDepthMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CameraFirmwareVersion_Uint64, "CameraFirmwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CameraFirmwareDescription_String, "CameraFirmwareDescription", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFPGAVersion_Uint64, "DisplayFPGAVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayBootloaderVersion_Uint64, "DisplayBootloaderVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayHardwareVersion_Uint64, "DisplayHardwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_AudioFirmwareVersion_Uint64, "AudioFirmwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CameraCompatibilityMode_Int32, "CameraCompatibilityMode", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ScreenshotHorizontalFieldOfViewDegrees_Float, "ScreenshotHorizontalFieldOfViewDegrees", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplaySuppressed_Bool, "DisplaySuppressed", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayAllowNightMode_Bool, "DisplayAllowNightMode", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageWidth_Int32, "DisplayMCImageWidth", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageHeight_Int32, "DisplayMCImageHeight", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageNumChannels_Int32, "DisplayMCImageNumChannels", vr::k_unInt32PropertyTag);
                    //l_storedProperties.emplace_back(vr::Prop_DisplayMCImageData_Binary, "DisplayMCImageData_Binary", ?);
                    l_storedProperties.emplace_back(vr::Prop_SecondsFromPhotonsToVblank_Float, "SecondsFromPhotonsToVblank", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverDirectModeSendsVsyncEvents_Bool, "DriverDirectModeSendsVsyncEvents", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayDebugMode_Bool, "DisplayDebugMode", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_GraphicsAdapterLuid_Uint64, "GraphicsAdapterLuid", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverProvidedChaperonePath_String, "DriverProvidedChaperonePath", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedTrackingReferenceCount_Int32, "ExpectedTrackingReferenceCount", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedControllerCount_Int32, "ExpectedControllerCount", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathControllerLeftDeviceOff_String, "NamedIconPathControllerLeftDeviceOff", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathControllerRightDeviceOff_String, "NamedIconPathControllerRightDeviceOff", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathTrackingReferenceDeviceOff_String, "NamedIconPathTrackingReferenceDeviceOff", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DoNotApplyPrediction_Bool, "DoNotApplyPrediction", vr::k_unBoolPropertyTag);
                    //l_storedProperties.emplace_back(vr::Prop_CameraToHeadTransforms_Matrix34_Array, "CameraToHeadTransforms_Matrix34_Array", ?);
                    l_storedProperties.emplace_back(vr::Prop_DistortionMeshResolution_Int32, "DistortionMeshResolution", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverIsDrawingControllers_Bool, "DriverIsDrawingControllers", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestsApplicationPause_Bool, "DriverRequestsApplicationPause", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestsReducedRendering_Bool, "DriverRequestsReducedRendering", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_MinimumIpdStepMeters_Float, "MinimumIpdStepMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_AudioBridgeFirmwareVersion_Uint64, "AudioBridgeFirmwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImageBridgeFirmwareVersion_Uint64, "ImageBridgeFirmwareVersion", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImuToHeadTransform_Matrix34, "ImuToHeadTransform", vr::k_unHmdMatrix34PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryGyroBias_Vector3, "ImuFactoryGyroBias", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryGyroScale_Vector3, "ImuFactoryGyroScale", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryAccelerometerBias_Vector3, "ImuFactoryAccelerometerBias", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryAccelerometerScale_Vector3, "ImuFactoryAccelerometerScale", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ConfigurationIncludesLighthouse20Features_Bool, "ConfigurationIncludesLighthouse20Features", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_AdditionalRadioFeatures_Uint64, "AdditionalRadioFeatures", vr::k_unUint64PropertyTag);
                    //l_storedProperties.emplace_back(vr::Prop_CameraWhiteBalance_Vector4_Array, "CameraWhiteBalance", ?);
                    //l_storedProperties.emplace_back(vr::Prop_CameraDistortionFunction_Int32_Array, "CameraDistortionFunction", ?);
                    //l_storedProperties.emplace_back(vr::Prop_CameraDistortionCoefficients_Float_Array, "CameraDistortionCoefficients", ?);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedControllerType_String, "ExpectedControllerType", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_HmdTrackingStyle_Int32, "HmdTrackingStyle", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverProvidedChaperoneVisibility_Bool, "DriverProvidedChaperoneVisibility", vr::k_unBoolPropertyTag);
                    //l_storedProperties.emplace_back(vr::Prop_DisplayAvailableFrameRates_Float_Array, "DisplayAvailableFrameRates_Float_Array", ?);
                    l_storedProperties.emplace_back(vr::Prop_DisplaySupportsMultipleFramerates_Bool, "DisplaySupportsMultipleFramerates", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayColorMultLeft_Vector3, "DisplayColorMultLeft", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DisplayColorMultRight_Vector3, "DisplayColorMultRight", vr::k_unHmdVector3PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DashboardScale_Float, "DashboardScale", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_IpdUIRangeMinMeters_Float, "IpdUIRangeMinMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_IpdUIRangeMaxMeters_Float, "IpdUIRangeMaxMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraCorrectionMode_Int32, "DriverRequestedMuraCorrectionMode", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerLeft_Int32, "DriverRequestedMuraFeather_InnerLeft", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerRight_Int32, "DriverRequestedMuraFeather_InnerRight", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerTop_Int32, "DriverRequestedMuraFeather_InnerTop", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerBottom_Int32, "DriverRequestedMuraFeather_InnerBottom", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterLeft_Int32, "DriverRequestedMuraFeather_OuterLeft", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterRight_Int32, "DriverRequestedMuraFeather_OuterRight", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterTop_Int32, "DriverRequestedMuraFeather_OuterTop", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterBottom_Int32, "DriverRequestedMuraFeather_OuterBottom", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultPlaybackDeviceId_String, "Audio_DefaultPlaybackDeviceId", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultRecordingDeviceId_String, "Audio_DefaultRecordingDeviceId", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultPlaybackDeviceVolume_Float, "Audio_DefaultPlaybackDeviceVolume", vr::k_unFloatPropertyTag);
                } break;
                case vr::TrackedDeviceClass_Controller: case vr::TrackedDeviceClass_GenericTracker:
                {
                    l_storedProperties.emplace_back(vr::Prop_AttachedDeviceId_String, "AttachedDeviceId", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_SupportedButtons_Uint64, "SupportedButtons", vr::k_unUint64PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Axis0Type_Int32, "Axis0Type", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Axis1Type_Int32, "Axis1Type", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Axis2Type_Int32, "Axis2Type", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Axis3Type_Int32, "Axis3Type", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Axis4Type_Int32, "Axis4Type", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ControllerRoleHint_Int32, "ControllerRoleHint", vr::k_unInt32PropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ControllerType_String, "ControllerType", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ControllerHandSelectionPriority_Int32, "ControllerHandSelectionPriority", vr::k_unInt32PropertyTag);
                } break;
                case vr::TrackedDeviceClass_TrackingReference:
                {
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewLeftDegrees_Float, "FieldOfViewLeftDegrees", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewRightDegrees_Float, "FieldOfViewRightDegrees", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewTopDegrees_Float, "FieldOfViewTopDegrees", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewBottomDegrees_Float, "FieldOfViewBottomDegrees", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_TrackingRangeMinimumMeters_Float, "TrackingRangeMinimumMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_TrackingRangeMaximumMeters_Float, "TrackingRangeMaximumMeters", vr::k_unFloatPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_ModeLabel_String, "ModeLabel", vr::k_unStringPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_CanWirelessIdentify_Bool, "CanWirelessIdentify", vr::k_unBoolPropertyTag);
                    l_storedProperties.emplace_back(vr::Prop_Nonce_Int32, "Nonce", vr::k_unInt32PropertyTag);
                } break;
            }

            l_storedProperties.emplace_back(vr::Prop_IconPathName_String, "IconPathName", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceOff_String, "NamedIconPathDeviceOff", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceSearching_String, "NamedIconPathDeviceSearching", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceSearchingAlert_String, "NamedIconPathDeviceSearchingAlert", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceReady_String, "NamedIconPathDeviceReady", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceReadyAlert_String, "NamedIconPathDeviceReadyAlert", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceNotReady_String, "NamedIconPathDeviceNotReady", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceStandby_String, "NamedIconPathDeviceStandby", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceAlertLow_String, "NamedIconPathDeviceAlertLow", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceStandbyAlert_String, "NamedIconPathDeviceStandbyAlert", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_OverrideContainer_Uint64, "OverrideContainer", vr::k_unUint64PropertyTag);
            l_storedProperties.emplace_back(vr::Prop_UserConfigPath_String, "UserConfigPath", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_InstallPath_String, "InstallPath", vr::k_unStringPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasDisplayComponent_Bool, "HasDisplayComponent", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasControllerComponent_Bool, "HasControllerComponent", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasCameraComponent_Bool, "HasCameraComponent", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasDriverDirectModeComponent_Bool, "HasDriverDirectModeComponent", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasVirtualDisplayComponent_Bool, "HasVirtualDisplayComponent", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasSpatialAnchorsSupport_Bool, "HasSpatialAnchorsSupport", vr::k_unBoolPropertyTag);
            l_storedProperties.emplace_back(vr::Prop_HasSpatialAnchorsSupport_Bool, "HasSpatialAnchorsSupport", vr::k_unBoolPropertyTag);

            vr::TrackedDeviceIndex_t l_devices[vr::k_unMaxTrackedDeviceCount];
            uint32_t l_devicesCount = l_vrSystem->GetSortedTrackedDeviceIndicesOfClass(static_cast<vr::ETrackedDeviceClass>(l_deviceClass), l_devices, vr::k_unMaxTrackedDeviceCount);

            for(vr::TrackedDeviceIndex_t i = 0U; i < l_devicesCount; i++)
            {
                pugi::xml_node l_deviceNode = l_rootNode.append_child("device");

                pugi::xml_attribute l_deviceId = l_deviceNode.append_attribute("id");
                l_deviceId.set_value(l_devices[i]);

                pugi::xml_attribute l_deviceType = l_deviceNode.append_attribute("type");
                l_deviceType.set_value(g_DeviceTypeName[static_cast<size_t>(l_deviceClass)]);

                for(auto &l_storedProperty : l_storedProperties)
                {
                    // Parse
                    vr::ETrackedPropertyError l_propertyError;
                    switch(l_storedProperty.m_propertyTypeTag)
                    {
                        case vr::k_unBoolPropertyTag:
                            l_storedProperty.m_bool = l_vrSystem->GetBoolTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case vr::k_unFloatPropertyTag:
                            l_storedProperty.m_float = l_vrSystem->GetFloatTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case vr::k_unStringPropertyTag:
                        {
                            char l_propertyString[vr::k_unMaxPropertyStringSize];
                            const uint32_t l_length = l_vrSystem->GetStringTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, l_propertyString, vr::k_unMaxPropertyStringSize, &l_propertyError);
                            if(l_length > 0U) l_storedProperty.m_string.assign(l_propertyString, l_length);
                        } break;
                        case vr::k_unInt32PropertyTag:
                            l_storedProperty.m_int32 = l_vrSystem->GetInt32TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case vr::k_unUint64PropertyTag:
                            l_storedProperty.m_uint64 = l_vrSystem->GetUint64TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case vr::k_unHmdMatrix34PropertyTag:
                            l_storedProperty.m_matrix34 = l_vrSystem->GetMatrix34TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case vr::k_unHmdVector3PropertyTag:
                            l_vrSystem->GetArrayTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, vr::k_unHmdVector3PropertyTag, &l_storedProperty.m_vector3, sizeof(vr::HmdVector3_t), &l_propertyError);
                            break;
                    }

                    l_storedProperty.m_stored = (l_propertyError == vr::TrackedProp_Success);

                    // Save to attributes
                    if(l_storedProperty.m_stored)
                    {
                        pugi::xml_node l_propertyNode = l_deviceNode.append_child("property");

                        pugi::xml_attribute l_propertyName = l_propertyNode.append_attribute("name");
                        l_propertyName.set_value(l_storedProperty.m_propertyName);

                        pugi::xml_attribute l_typeAttribute = l_propertyNode.append_attribute("type");
                        l_typeAttribute.set_value(g_PropertyTypeTagName.find(l_storedProperty.m_propertyTypeTag)->second);

                        pugi::xml_attribute l_valueAttribute = l_propertyNode.append_attribute("value");
                        switch(l_storedProperty.m_propertyTypeTag)
                        {
                            case vr::k_unBoolPropertyTag:
                                l_valueAttribute.set_value(l_storedProperty.m_bool);
                                break;
                            case vr::k_unFloatPropertyTag:
                                l_valueAttribute.set_value(l_storedProperty.m_float);
                                break;
                            case vr::k_unInt32PropertyTag:
                                l_valueAttribute.set_value(l_storedProperty.m_int32);
                                break;
                            case vr::k_unUint64PropertyTag:
                                l_valueAttribute.set_value(l_storedProperty.m_uint64);
                                break;
                            case vr::k_unStringPropertyTag:
                                l_valueAttribute.set_value(l_storedProperty.m_string.c_str());
                                break;
                            case vr::k_unHmdMatrix34PropertyTag:
                            {
                                std::string l_matrixString;
                                for(size_t j = 0U; j < 3U; j++)
                                {
                                    for(size_t k = 0U; k < 4U; k++)
                                    {
                                        l_matrixString.append(std::to_string(l_storedProperty.m_matrix34.m[j][k]));
                                        l_matrixString.push_back(' ');
                                    }
                                }
                                l_valueAttribute.set_value(l_matrixString.c_str());
                            } break;
                            case vr::k_unHmdVector3PropertyTag:
                            {
                                std::string l_vectorString;
                                for(size_t j = 0U; j < 3U; j++)
                                {
                                    l_vectorString.append(std::to_string(l_storedProperty.m_vector3.v[j]));
                                    l_vectorString.push_back(' ');
                                }
                                l_valueAttribute.set_value(l_vectorString.c_str());
                            } break;
                        }
                    }
                }

                std::cout << "Device(" << l_devices[i] << ") properties are parsed" << std::endl;
            }
        }

        if(l_xmlFile->save_file("devices_data.xml")) std::cout << "Saved to 'devices_data.xml'" << std::endl;
        else std::cout << "Can't save data to 'devices_data.xml'. Check file access." << std::endl;

        delete l_xmlFile;
    }
    else std::cout << "Can't initialize OpenVR" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2U));
    return 0;
}
