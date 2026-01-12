#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <usbiodef.h>
#include <iostream>
#include <string>

#pragma comment(lib, "setupapi.lib")

int main() {
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_USB_DEVICE,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if (hDevInfo == INVALID_HANDLE_VALUE) return 1;

    SP_DEVICE_INTERFACE_DATA interfaceData;
    interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    DWORD index = 0;
    while (SetupDiEnumDeviceInterfaces(hDevInfo, nullptr, &GUID_DEVINTERFACE_USB_DEVICE, index, &interfaceData)) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &interfaceData, nullptr, 0, &requiredSize, nullptr);

        PSP_DEVICE_INTERFACE_DETAIL_DATA detailData =
            (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        if (SetupDiGetDeviceInterfaceDetail(
                hDevInfo,
                &interfaceData,
                detailData,
                requiredSize,
                nullptr,
                &devInfoData
            )) {

            char name[512] = {};
            SetupDiGetDeviceRegistryPropertyA(
                hDevInfo,
                &devInfoData,
                SPDRP_FRIENDLYNAME,
                nullptr,
                (PBYTE)name,
                sizeof(name),
                nullptr
            );

            if (name[0] == 0) {
                SetupDiGetDeviceRegistryPropertyA(
                    hDevInfo,
                    &devInfoData,
                    SPDRP_DEVICEDESC,
                    nullptr,
                    (PBYTE)name,
                    sizeof(name),
                    nullptr
                );
            }

            char hardwareId[512] = {};
            SetupDiGetDeviceRegistryPropertyA(
                hDevInfo,
                &devInfoData,
                SPDRP_HARDWAREID,
                nullptr,
                (PBYTE)hardwareId,
                sizeof(hardwareId),
                nullptr
            );

            std::string hw = hardwareId;
            std::string vid = "UNKNOWN";
            std::string pid = "UNKNOWN";

            size_t vpos = hw.find("VID_");
            size_t ppos = hw.find("PID_");

            if (vpos != std::string::npos) vid = hw.substr(vpos + 4, 4);
            if (ppos != std::string::npos) pid = hw.substr(ppos + 4, 4);

            std::cout << "Name: " << name << std::endl;
            std::cout << "VID: " << vid << std::endl;
            std::cout << "PID: " << pid << std::endl;
            std::cout << "Path: " << detailData->DevicePath << std::endl;
            std::cout << std::endl;
        }

        free(detailData);
        index++;
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return 0;
}
