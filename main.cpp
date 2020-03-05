#include "stdafx.h"

enum PropertyType : unsigned char
{
    PT_Bool = 0U,
    PT_Float,
    PT_String,
    PT_Int32,
    PT_UInt64,
    PT_Matrix34,
    PT_Vector3
};
const char* g_PropertyTypeName[]
{
    "bool", "float", "string", "int32", "uint64", "matrix34", "vector3"
};
const char* g_DeviceTypeName[]
{
    "invalid", "hmd", "controller", "generic_tracker", "tracking_reference", "display_redirect"
};

struct StoredProperty
{
    vr::ETrackedDeviceProperty m_vrProperty = vr::Prop_Invalid;
    const char *m_propertyName;
    PropertyType m_propertyType;
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

    StoredProperty(vr::ETrackedDeviceProperty f_prop, const char *f_name, PropertyType f_type)
    {
        m_vrProperty = f_prop;
        m_propertyName = f_name;
        m_propertyType = f_type;
    }
};

int main(int argc, const char* argv[])
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
            l_storedProperties.emplace_back(vr::Prop_TrackingSystemName_String, "TrackingSystemName", PT_String);
            l_storedProperties.emplace_back(vr::Prop_ModelNumber_String, "ModelNumber", PT_String);
            l_storedProperties.emplace_back(vr::Prop_SerialNumber_String, "SerialNumber", PT_String);
            l_storedProperties.emplace_back(vr::Prop_RenderModelName_String, "RenderModelName", PT_String);
            l_storedProperties.emplace_back(vr::Prop_WillDriftInYaw_Bool, "WillDriftInYaw", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_ManufacturerName_String, "ManufacturerName", PT_String);
            l_storedProperties.emplace_back(vr::Prop_TrackingFirmwareVersion_String, "TrackingFirmwareVersion", PT_String);
            l_storedProperties.emplace_back(vr::Prop_HardwareRevision_String, "HardwareRevision", PT_String);
            l_storedProperties.emplace_back(vr::Prop_AllWirelessDongleDescriptions_String, "AllWirelessDongleDescriptions", PT_String);
            l_storedProperties.emplace_back(vr::Prop_ConnectedWirelessDongle_String, "ConnectedWirelessDongle", PT_String);
            l_storedProperties.emplace_back(vr::Prop_DeviceIsWireless_Bool, "DeviceIsWireless", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_DeviceIsCharging_Bool, "DeviceIsCharging", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_DeviceBatteryPercentage_Float, "DeviceBatteryPercentage", PT_Float);
            l_storedProperties.emplace_back(vr::Prop_StatusDisplayTransform_Matrix34, "StatusDisplayTransform", PT_Matrix34);
            l_storedProperties.emplace_back(vr::Prop_Firmware_UpdateAvailable_Bool, "Firmware_UpdateAvailable", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ManualUpdate_Bool, "Firmware_ManualUpdate", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ManualUpdateURL_String, "Firmware_ManualUpdateURL", PT_String);
            l_storedProperties.emplace_back(vr::Prop_HardwareRevision_Uint64, "HardwareRevision", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_FirmwareVersion_Uint64, "FirmwareVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_FPGAVersion_Uint64, "FPGAVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_VRCVersion_Uint64, "VRCVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_RadioVersion_Uint64, "RadioVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_DongleVersion_Uint64, "DongleVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_BlockServerShutdown_Bool, "BlockServerShutdown", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_CanUnifyCoordinateSystemWithHmd_Bool, "CanUnifyCoordinateSystemWithHmd", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_ContainsProximitySensor_Bool, "ContainsProximitySensor", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_DeviceProvidesBatteryStatus_Bool, "DeviceProvidesBatteryStatus", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_DeviceCanPowerOff_Bool, "DeviceCanPowerOff", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ProgrammingTarget_String, "Firmware_ProgrammingTarget", PT_String);
            l_storedProperties.emplace_back(vr::Prop_DeviceClass_Int32, "DeviceClass", PT_Int32);
            l_storedProperties.emplace_back(vr::Prop_HasCamera_Bool, "HasCamera", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_DriverVersion_String, "DriverVersion", PT_String);
            l_storedProperties.emplace_back(vr::Prop_Firmware_ForceUpdateRequired_Bool, "Firmware_ForceUpdateRequired", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_ViveSystemButtonFixRequired_Bool, "ViveSystemButtonFixRequired", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_ParentDriver_Uint64, "ParentDriver", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_ResourceRoot_String, "ResourceRoot", PT_String);
            l_storedProperties.emplace_back(vr::Prop_RegisteredDeviceType_String, "RegisteredDeviceType", PT_String);
            l_storedProperties.emplace_back(vr::Prop_InputProfilePath_String, "InputProfilePath", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NeverTracked_Bool, "NeverTracked", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_NumCameras_Int32, "NumCameras", PT_Int32);
            l_storedProperties.emplace_back(vr::Prop_CameraFrameLayout_Int32, "CameraFrameLayout", PT_Int32);
            l_storedProperties.emplace_back(vr::Prop_CameraStreamFormat_Int32, "CameraStreamFormat", PT_Int32);
            l_storedProperties.emplace_back(vr::Prop_AdditionalDeviceSettingsPath_String, "AdditionalDeviceSettingsPath", PT_String);
            l_storedProperties.emplace_back(vr::Prop_Identifiable_Bool, "Identifiable", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_BootloaderVersion_Uint64, "BootloaderVersion", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_AdditionalSystemReportData_String, "AdditionalSystemReportData", PT_String);
            l_storedProperties.emplace_back(vr::Prop_CompositeFirmwareVersion_String, "CompositeFirmwareVersion", PT_String);
            l_storedProperties.emplace_back(vr::Prop_Firmware_RemindUpdate_Bool, "Firmware_RemindUpdate", PT_Bool);

            switch(l_deviceClass)
            {
                case vr::TrackedDeviceClass_HMD:
                {
                    l_storedProperties.emplace_back(vr::Prop_ReportsTimeSinceVSync_Bool, "ReportsTimeSinceVSync", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_SecondsFromVsyncToPhotons_Float, "SecondsFromVsyncToPhotons", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFrequency_Float, "DisplayFrequency", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_UserIpdMeters_Float, "UserIpdMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_CurrentUniverseId_Uint64, "CurrentUniverseId", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_PreviousUniverseId_Uint64, "PreviousUniverseId", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFirmwareVersion_Uint64, "DisplayFirmwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_IsOnDesktop_Bool, "IsOnDesktop", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCType_Int32, "DisplayMCType", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCOffset_Float, "DisplayMCOffset", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCScale_Float, "DisplayMCScale", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_EdidVendorID_Int32, "EdidVendorID", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageLeft_String, "DisplayMCImageLeft", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageRight_String, "DisplayMCImageRight", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCBlackClamp_Float, "DisplayGCBlackClamp", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_EdidProductID_Int32, "EdidProductID", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_CameraToHeadTransform_Matrix34, "CameraToHeadTransform", PT_Matrix34);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCType_Int32, "DisplayGCType", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCOffset_Float, "DisplayGCOffset", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCScale_Float, "DisplayGCScale", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCPrescale_Float, "DisplayGCPrescale", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCImage_String, "DisplayGCImage", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplayGCImage_String, "DisplayGCImage", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterLeftU_Float, "LensCenterLeftU", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterLeftV_Float, "LensCenterLeftV", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterRightU_Float, "LensCenterRightU", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_LensCenterRightV_Float, "LensCenterRightV", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_UserHeadToEyeDepthMeters_Float, "UserHeadToEyeDepthMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_CameraFirmwareVersion_Uint64, "CameraFirmwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_CameraFirmwareDescription_String, "CameraFirmwareDescription", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_DisplayFPGAVersion_Uint64, "DisplayFPGAVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_DisplayBootloaderVersion_Uint64, "DisplayBootloaderVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_DisplayHardwareVersion_Uint64, "DisplayHardwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_AudioFirmwareVersion_Uint64, "AudioFirmwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_CameraCompatibilityMode_Int32, "CameraCompatibilityMode", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_ScreenshotHorizontalFieldOfViewDegrees_Float, "ScreenshotHorizontalFieldOfViewDegrees", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DisplaySuppressed_Bool, "DisplaySuppressed", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DisplayAllowNightMode_Bool, "DisplayAllowNightMode", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageWidth_Int32, "DisplayMCImageWidth", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageHeight_Int32, "DisplayMCImageHeight", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DisplayMCImageNumChannels_Int32, "DisplayMCImageNumChannels", PT_Int32);
                    //l_storedProperties.emplace_back(vr::Prop_DisplayMCImageData_Binary, "DisplayMCImageData_Binary", ?);
                    l_storedProperties.emplace_back(vr::Prop_SecondsFromPhotonsToVblank_Float, "SecondsFromPhotonsToVblank", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DriverDirectModeSendsVsyncEvents_Bool, "DriverDirectModeSendsVsyncEvents", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DisplayDebugMode_Bool, "DisplayDebugMode", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_GraphicsAdapterLuid_Uint64, "GraphicsAdapterLuid", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_DriverProvidedChaperonePath_String, "DriverProvidedChaperonePath", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedTrackingReferenceCount_Int32, "ExpectedTrackingReferenceCount", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedControllerCount_Int32, "ExpectedControllerCount", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathControllerLeftDeviceOff_String, "NamedIconPathControllerLeftDeviceOff", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathControllerRightDeviceOff_String, "NamedIconPathControllerRightDeviceOff", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_NamedIconPathTrackingReferenceDeviceOff_String, "NamedIconPathTrackingReferenceDeviceOff", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_DoNotApplyPrediction_Bool, "DoNotApplyPrediction", PT_Bool);
                    //l_storedProperties.emplace_back(vr::Prop_CameraToHeadTransforms_Matrix34_Array, "CameraToHeadTransforms_Matrix34_Array", ?);
                    l_storedProperties.emplace_back(vr::Prop_DistortionMeshResolution_Int32, "DistortionMeshResolution", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverIsDrawingControllers_Bool, "DriverIsDrawingControllers", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestsApplicationPause_Bool, "DriverRequestsApplicationPause", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestsReducedRendering_Bool, "DriverRequestsReducedRendering", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_MinimumIpdStepMeters_Float, "MinimumIpdStepMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_AudioBridgeFirmwareVersion_Uint64, "AudioBridgeFirmwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_ImageBridgeFirmwareVersion_Uint64, "ImageBridgeFirmwareVersion", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_ImuToHeadTransform_Matrix34, "ImuToHeadTransform", PT_Matrix34);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryGyroBias_Vector3, "ImuFactoryGyroBias", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryGyroScale_Vector3, "ImuFactoryGyroScale", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryAccelerometerBias_Vector3, "ImuFactoryAccelerometerBias", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_ImuFactoryAccelerometerScale_Vector3, "ImuFactoryAccelerometerScale", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_ConfigurationIncludesLighthouse20Features_Bool, "ConfigurationIncludesLighthouse20Features", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_AdditionalRadioFeatures_Uint64, "AdditionalRadioFeatures", PT_UInt64);
                    //l_storedProperties.emplace_back(vr::Prop_CameraWhiteBalance_Vector4_Array, "CameraWhiteBalance", ?);
                    //l_storedProperties.emplace_back(vr::Prop_CameraDistortionFunction_Int32_Array, "CameraDistortionFunction", ?);
                    //l_storedProperties.emplace_back(vr::Prop_CameraDistortionCoefficients_Float_Array, "CameraDistortionCoefficients", ?);
                    l_storedProperties.emplace_back(vr::Prop_ExpectedControllerType_String, "ExpectedControllerType", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_HmdTrackingStyle_Int32, "HmdTrackingStyle", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverProvidedChaperoneVisibility_Bool, "DriverProvidedChaperoneVisibility", PT_Bool);
                    //l_storedProperties.emplace_back(vr::Prop_DisplayAvailableFrameRates_Float_Array, "DisplayAvailableFrameRates_Float_Array", ?);
                    l_storedProperties.emplace_back(vr::Prop_DisplaySupportsMultipleFramerates_Bool, "DisplaySupportsMultipleFramerates", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_DisplayColorMultLeft_Vector3, "DisplayColorMultLeft", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_DisplayColorMultRight_Vector3, "DisplayColorMultRight", PT_Vector3);
                    l_storedProperties.emplace_back(vr::Prop_DashboardLayoutPathName_String, "DashboardLayoutPathName", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_DashboardScale_Float, "DashboardScale", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_IpdUIRangeMinMeters_Float, "IpdUIRangeMinMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_IpdUIRangeMaxMeters_Float, "IpdUIRangeMaxMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraCorrectionMode_Int32, "DriverRequestedMuraCorrectionMode", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerLeft_Int32, "DriverRequestedMuraFeather_InnerLeft", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerRight_Int32, "DriverRequestedMuraFeather_InnerRight", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerTop_Int32, "DriverRequestedMuraFeather_InnerTop", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_InnerBottom_Int32, "DriverRequestedMuraFeather_InnerBottom", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterLeft_Int32, "DriverRequestedMuraFeather_OuterLeft", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterRight_Int32, "DriverRequestedMuraFeather_OuterRight", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterTop_Int32, "DriverRequestedMuraFeather_OuterTop", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_DriverRequestedMuraFeather_OuterBottom_Int32, "DriverRequestedMuraFeather_OuterBottom", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultPlaybackDeviceId_String, "Audio_DefaultPlaybackDeviceId", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultRecordingDeviceId_String, "Audio_DefaultRecordingDeviceId", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_Audio_DefaultPlaybackDeviceVolume_Float, "Audio_DefaultPlaybackDeviceVolume", PT_Float);
                } break;
                case vr::TrackedDeviceClass_Controller: case vr::TrackedDeviceClass_GenericTracker:
                {
                    l_storedProperties.emplace_back(vr::Prop_AttachedDeviceId_String, "AttachedDeviceId", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_SupportedButtons_Uint64, "SupportedButtons", PT_UInt64);
                    l_storedProperties.emplace_back(vr::Prop_Axis0Type_Int32, "Axis0Type", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_Axis1Type_Int32, "Axis1Type", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_Axis2Type_Int32, "Axis2Type", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_Axis3Type_Int32, "Axis3Type", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_Axis4Type_Int32, "Axis4Type", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_ControllerRoleHint_Int32, "ControllerRoleHint", PT_Int32);
                    l_storedProperties.emplace_back(vr::Prop_ControllerType_String, "ControllerType", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_ControllerHandSelectionPriority_Int32, "ControllerHandSelectionPriority", PT_Int32);
                } break;
                case vr::TrackedDeviceClass_TrackingReference:
                {
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewLeftDegrees_Float, "FieldOfViewLeftDegrees", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewRightDegrees_Float, "FieldOfViewRightDegrees", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewTopDegrees_Float, "FieldOfViewTopDegrees", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_FieldOfViewBottomDegrees_Float, "FieldOfViewBottomDegrees", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_TrackingRangeMinimumMeters_Float, "TrackingRangeMinimumMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_TrackingRangeMaximumMeters_Float, "TrackingRangeMaximumMeters", PT_Float);
                    l_storedProperties.emplace_back(vr::Prop_ModeLabel_String, "ModeLabel", PT_String);
                    l_storedProperties.emplace_back(vr::Prop_CanWirelessIdentify_Bool, "CanWirelessIdentify", PT_Bool);
                    l_storedProperties.emplace_back(vr::Prop_Nonce_Int32, "Nonce", PT_Int32);
                } break;
            }

            l_storedProperties.emplace_back(vr::Prop_IconPathName_String, "IconPathName", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceOff_String, "NamedIconPathDeviceOff", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceSearching_String, "NamedIconPathDeviceSearching", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceSearchingAlert_String, "NamedIconPathDeviceSearchingAlert", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceReady_String, "NamedIconPathDeviceReady", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceReadyAlert_String, "NamedIconPathDeviceReadyAlert", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceNotReady_String, "NamedIconPathDeviceNotReady", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceStandby_String, "NamedIconPathDeviceStandby", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceAlertLow_String, "NamedIconPathDeviceAlertLow", PT_String);
            l_storedProperties.emplace_back(vr::Prop_NamedIconPathDeviceStandbyAlert_String, "NamedIconPathDeviceStandbyAlert", PT_String);
            l_storedProperties.emplace_back(vr::Prop_OverrideContainer_Uint64, "OverrideContainer", PT_UInt64);
            l_storedProperties.emplace_back(vr::Prop_UserConfigPath_String, "UserConfigPath", PT_String);
            l_storedProperties.emplace_back(vr::Prop_InstallPath_String, "InstallPath", PT_String);
            l_storedProperties.emplace_back(vr::Prop_HasDisplayComponent_Bool, "HasDisplayComponent", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasControllerComponent_Bool, "HasControllerComponent", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasCameraComponent_Bool, "HasCameraComponent", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasDriverDirectModeComponent_Bool, "HasDriverDirectModeComponent", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasVirtualDisplayComponent_Bool, "HasVirtualDisplayComponent", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasSpatialAnchorsSupport_Bool, "HasSpatialAnchorsSupport", PT_Bool);
            l_storedProperties.emplace_back(vr::Prop_HasSpatialAnchorsSupport_Bool, "HasSpatialAnchorsSupport", PT_Bool);

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
                    switch(l_storedProperty.m_propertyType)
                    {
                        case PT_Bool:
                            l_storedProperty.m_bool = l_vrSystem->GetBoolTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case PT_Float:
                            l_storedProperty.m_float = l_vrSystem->GetFloatTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case PT_String:
                        {
                            char l_propertyString[vr::k_unMaxPropertyStringSize];
                            const uint32_t l_length = l_vrSystem->GetStringTrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, l_propertyString, vr::k_unMaxPropertyStringSize, &l_propertyError);
                            if(l_length > 0U) l_storedProperty.m_string.assign(l_propertyString, l_length);
                        } break;
                        case PT_Int32:
                            l_storedProperty.m_int32 = l_vrSystem->GetInt32TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case PT_UInt64:
                            l_storedProperty.m_uint64 = l_vrSystem->GetUint64TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case PT_Matrix34:
                            l_storedProperty.m_matrix34 = l_vrSystem->GetMatrix34TrackedDeviceProperty(l_devices[i], l_storedProperty.m_vrProperty, &l_propertyError);
                            break;
                        case PT_Vector3:
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
                        l_typeAttribute.set_value(g_PropertyTypeName[l_storedProperty.m_propertyType]);

                        pugi::xml_attribute l_valueAttribute = l_propertyNode.append_attribute("value");
                        switch(l_storedProperty.m_propertyType)
                        {
                            case PT_Bool:
                                l_valueAttribute.set_value(l_storedProperty.m_bool);
                                break;
                            case PT_Float:
                                l_valueAttribute.set_value(l_storedProperty.m_float);
                                break;
                            case PT_Int32:
                                l_valueAttribute.set_value(l_storedProperty.m_int32);
                                break;
                            case PT_UInt64:
                                l_valueAttribute.set_value(l_storedProperty.m_uint64);
                                break;
                            case PT_String:
                                l_valueAttribute.set_value(l_storedProperty.m_string.c_str());
                                break;
                            case PT_Matrix34:
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
                            case PT_Vector3:
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
