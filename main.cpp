#include "stdafx.h"

enum PropertyType : unsigned char
{
    PT_Bool = 0U,
    PT_Float,
    PT_String,
    PT_Int32,
    PT_UInt64,
    PT_Matrix34
};
const char* g_PropertyTypeName[]
{
    "bool", "float", "string", "int32", "uint64", "matrix34"
};

struct StoredProperty
{
    vr::ETrackedDeviceProperty m_vrProperty = vr::Prop_Invalid;
    std::string m_propertyName;
    PropertyType m_propertyType;
    bool m_stored = false;

    union
    {
        bool m_bool = false;
        float m_float;
        int32_t m_int32;
        uint64_t m_uint64;
        vr::HmdMatrix34_t m_matrix34;
    };
    std::string m_string = "";

    StoredProperty(vr::ETrackedDeviceProperty f_prop, const char *f_name, PropertyType f_type)
    {
        m_vrProperty = f_prop;
        m_propertyName.assign(f_name);
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

        auto l_shortDelay = std::chrono::seconds(1U);
        std::cout << "Activate your controllers in period of 3 seconds ";
        for(size_t i = 0U; i < 3U; i++)
        {
            std::cout << '.';
            std::this_thread::sleep_for(l_shortDelay);
        }
        std::cout << std::endl;

        std::vector<StoredProperty> l_storedProperties
        {
            StoredProperty(vr::Prop_TrackingSystemName_String, "TrackingSystemName", PT_String),
            StoredProperty(vr::Prop_ModelNumber_String, "ModelNumber", PT_String),
            StoredProperty(vr::Prop_SerialNumber_String, "SerialNumber", PT_String),
            StoredProperty(vr::Prop_RenderModelName_String, "RenderModelName", PT_String),
            StoredProperty(vr::Prop_WillDriftInYaw_Bool, "WillDriftInYaw", PT_Bool),
            StoredProperty(vr::Prop_ManufacturerName_String, "ManufacturerName", PT_String),
            StoredProperty(vr::Prop_TrackingFirmwareVersion_String, "TrackingFirmwareVersion", PT_String),
            StoredProperty(vr::Prop_HardwareRevision_String, "HardwareRevision", PT_String),
            StoredProperty(vr::Prop_AllWirelessDongleDescriptions_String, "AllWirelessDongleDescriptions", PT_String),
            StoredProperty(vr::Prop_ConnectedWirelessDongle_String, "ConnectedWirelessDongle", PT_String),
            StoredProperty(vr::Prop_DeviceIsWireless_Bool, "DeviceIsWireless", PT_Bool),
            StoredProperty(vr::Prop_DeviceIsCharging_Bool, "DeviceIsCharging", PT_Bool),
            StoredProperty(vr::Prop_DeviceBatteryPercentage_Float, "DeviceBatteryPercentage", PT_Float),
            StoredProperty(vr::Prop_StatusDisplayTransform_Matrix34, "StatusDisplayTransform", PT_Matrix34),
            StoredProperty(vr::Prop_Firmware_UpdateAvailable_Bool, "Firmware_UpdateAvailable", PT_Bool),
            StoredProperty(vr::Prop_Firmware_ManualUpdate_Bool, "Firmware_ManualUpdate", PT_Bool),
            StoredProperty(vr::Prop_Firmware_ManualUpdateURL_String, "Firmware_ManualUpdateURL", PT_String),
            StoredProperty(vr::Prop_HardwareRevision_Uint64, "HardwareRevision", PT_UInt64),
            StoredProperty(vr::Prop_FirmwareVersion_Uint64, "FirmwareVersion", PT_UInt64),
            StoredProperty(vr::Prop_FPGAVersion_Uint64, "FPGAVersion", PT_UInt64),
            StoredProperty(vr::Prop_VRCVersion_Uint64, "VRCVersion", PT_UInt64),
            StoredProperty(vr::Prop_RadioVersion_Uint64, "RadioVersion", PT_UInt64),
            StoredProperty(vr::Prop_DongleVersion_Uint64, "DongleVersion", PT_UInt64),
            StoredProperty(vr::Prop_BlockServerShutdown_Bool, "BlockServerShutdown", PT_Bool),
            StoredProperty(vr::Prop_CanUnifyCoordinateSystemWithHmd_Bool, "CanUnifyCoordinateSystemWithHmd", PT_Bool),
            StoredProperty(vr::Prop_ContainsProximitySensor_Bool, "ContainsProximitySensor", PT_Bool),
            StoredProperty(vr::Prop_DeviceProvidesBatteryStatus_Bool, "DeviceProvidesBatteryStatus", PT_Bool),
            StoredProperty(vr::Prop_DeviceCanPowerOff_Bool, "DeviceCanPowerOff", PT_Bool),
            StoredProperty(vr::Prop_Firmware_ProgrammingTarget_String, "Firmware_ProgrammingTarget", PT_String),
            StoredProperty(vr::Prop_DeviceClass_Int32, "DeviceClass", PT_Int32),
            StoredProperty(vr::Prop_HasCamera_Bool, "HasCamera", PT_Bool),
            StoredProperty(vr::Prop_DriverVersion_String, "DriverVersion", PT_String),
            StoredProperty(vr::Prop_Firmware_ForceUpdateRequired_Bool, "Firmware_ForceUpdateRequired", PT_Bool),
            StoredProperty(vr::Prop_ParentDriver_Uint64, "ParentDriver", PT_UInt64),
            StoredProperty(vr::Prop_ResourceRoot_String, "ResourceRoot", PT_String),
            StoredProperty(vr::Prop_RegisteredDeviceType_String, "RegisteredDeviceType", PT_String),
            StoredProperty(vr::Prop_InputProfilePath_String, "InputProfilePath", PT_String),
            StoredProperty(vr::Prop_NeverTracked_Bool, "NeverTracked", PT_Bool),
            StoredProperty(vr::Prop_NumCameras_Int32, "NumCameras", PT_Int32),
            StoredProperty(vr::Prop_CameraFrameLayout_Int32, "CameraFrameLayout", PT_Int32),
            StoredProperty(vr::Prop_CameraStreamFormat_Int32, "CameraStreamFormat", PT_Int32),
            StoredProperty(vr::Prop_AdditionalDeviceSettingsPath_String, "AdditionalDeviceSettingsPath", PT_String),
            StoredProperty(vr::Prop_Identifiable_Bool, "Identifiable", PT_Bool),
            StoredProperty(vr::Prop_BootloaderVersion_Uint64, "BootloaderVersion", PT_UInt64),
            StoredProperty(vr::Prop_AdditionalSystemReportData_String, "AdditionalSystemReportData", PT_String),
            StoredProperty(vr::Prop_CompositeFirmwareVersion_String, "CompositeFirmwareVersion", PT_String),
            StoredProperty(vr::Prop_Firmware_RemindUpdate_Bool, "Firmware_RemindUpdate", PT_Bool),
            StoredProperty(vr::Prop_AttachedDeviceId_String, "AttachedDeviceId", PT_String),
            StoredProperty(vr::Prop_SupportedButtons_Uint64, "SupportedButtons", PT_UInt64),
            StoredProperty(vr::Prop_Axis0Type_Int32, "Axis0Type", PT_Int32),
            StoredProperty(vr::Prop_Axis1Type_Int32, "Axis1Type", PT_Int32),
            StoredProperty(vr::Prop_Axis2Type_Int32, "Axis2Type", PT_Int32),
            StoredProperty(vr::Prop_Axis3Type_Int32, "Axis3Type", PT_Int32),
            StoredProperty(vr::Prop_Axis4Type_Int32, "Axis4Type", PT_Int32),
            StoredProperty(vr::Prop_ControllerRoleHint_Int32, "ControllerRoleHint", PT_Int32),
            StoredProperty(vr::Prop_IconPathName_String, "IconPathName", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceOff_String, "NamedIconPathDeviceOff", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceSearching_String, "NamedIconPathDeviceSearching", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceSearchingAlert_String, "NamedIconPathDeviceSearchingAlert", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceReady_String, "NamedIconPathDeviceReady", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceReadyAlert_String, "NamedIconPathDeviceReadyAlert", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceNotReady_String, "NamedIconPathDeviceNotReady", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceStandby_String, "NamedIconPathDeviceStandby", PT_String),
            StoredProperty(vr::Prop_NamedIconPathDeviceAlertLow_String, "NamedIconPathDeviceAlertLow", PT_String),
            StoredProperty(vr::Prop_UserConfigPath_String, "UserConfigPath", PT_String),
            StoredProperty(vr::Prop_InstallPath_String, "InstallPath", PT_String),
            StoredProperty(vr::Prop_HasDisplayComponent_Bool, "HasDisplayComponent", PT_Bool),
            StoredProperty(vr::Prop_HasControllerComponent_Bool, "HasControllerComponent", PT_Bool),
            StoredProperty(vr::Prop_HasCameraComponent_Bool, "HasCameraComponent", PT_Bool),
            StoredProperty(vr::Prop_HasDriverDirectModeComponent_Bool, "HasDriverDirectModeComponent", PT_Bool),
            StoredProperty(vr::Prop_HasVirtualDisplayComponent_Bool, "HasVirtualDisplayComponent", PT_Bool),
            StoredProperty(vr::Prop_HasSpatialAnchorsSupport_Bool, "HasSpatialAnchorsSupport", PT_Bool),
            StoredProperty(vr::Prop_ControllerType_String, "ControllerType", PT_String),
            StoredProperty(vr::Prop_ControllerHandSelectionPriority_Int32, "ControllerHandSelectionPriority", PT_Int32)
        };

        pugi::xml_document *l_xmlFile = new pugi::xml_document();
        pugi::xml_node l_rootNode = l_xmlFile->append_child("root");

        vr::TrackedDeviceIndex_t l_controllers[vr::k_unMaxTrackedDeviceCount];
        uint32_t l_controllersCount = l_vrSystem->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_Controller, l_controllers, vr::k_unMaxTrackedDeviceCount);

        std::cout << "Detected " << l_controllersCount << " controller(s)" << std::endl;

        for(vr::TrackedDeviceIndex_t i = 0U; i < l_controllersCount; i++)
        {
            pugi::xml_node l_controllerNode = l_rootNode.append_child("controller");

            for(auto &l_storedProperty : l_storedProperties)
            {
                // Parse
                vr::ETrackedPropertyError l_propertyError;
                switch(l_storedProperty.m_propertyType)
                {
                    case PT_Bool:
                        l_storedProperty.m_bool = l_vrSystem->GetBoolTrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, &l_propertyError);
                        break;
                    case PT_Float:
                        l_storedProperty.m_float = l_vrSystem->GetFloatTrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, &l_propertyError);
                        break;
                    case PT_String:
                    {
                        char l_propertyString[vr::k_unMaxPropertyStringSize];
                        uint32_t l_length = l_vrSystem->GetStringTrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, l_propertyString, vr::k_unMaxPropertyStringSize, &l_propertyError);
                        if(l_length > 0U) l_storedProperty.m_string.assign(l_propertyString, l_length);
                    } break;
                    case PT_Int32:
                        l_storedProperty.m_int32 = l_vrSystem->GetInt32TrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, &l_propertyError);
                        break;
                    case PT_UInt64:
                        l_storedProperty.m_uint64 = l_vrSystem->GetUint64TrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, &l_propertyError);
                        break;
                    case PT_Matrix34:
                        l_storedProperty.m_matrix34 = l_vrSystem->GetMatrix34TrackedDeviceProperty(l_controllers[i], l_storedProperty.m_vrProperty, &l_propertyError);
                        break;
                }

                l_storedProperty.m_stored = (l_propertyError == vr::TrackedProp_Success);

                // Save to attributes
                if(l_storedProperty.m_stored)
                {
                    pugi::xml_node l_propertyNode = l_controllerNode.append_child("property");

                    pugi::xml_attribute l_propertyName = l_propertyNode.append_attribute("name");
                    l_propertyName.set_value(l_storedProperty.m_propertyName.c_str());

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
                                    l_matrixString.append(std::to_string(l_storedProperty.m_matrix34.m[k][j]));
                                    l_matrixString.push_back(' ');
                                }
                            }
                            l_valueAttribute.set_value(l_matrixString.c_str());
                        } break;
                    }
                }
            }

            std::cout << "Controller's " << l_controllers[i] << " properties are parsed" << std::endl;
        }

        if(l_xmlFile->save_file("controllers_data.xml")) std::cout << "Saved to 'controllers_data.xml'" << std::endl;
        else std::cout << "Can't save data to 'controllers_data.xml'. Check file access." << std::endl;

        delete l_xmlFile;
        l_storedProperties.clear();
    }
    else std::cout << "Can't initialize OpenVR" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2U));
    return 0;
}
