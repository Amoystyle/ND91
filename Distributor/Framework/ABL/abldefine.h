#pragma once

#define GET_DEVICEDATA DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);if (!pDevice) return false;

#define CHECK_DEVICE_ACTIVE        {\
    if (!pDevice->IsActivate())     \
        return false;               \
    }

#define WAIT_DEVICE_ACTIVE(nWait)       {\
    DWORD nTickBegin = ::GetTickCount(); \
    while (!pDevice->IsActivate() && ::GetTickCount() - nTickBegin < nWait)     \
    {\
        Sleep(100);\
    }\
    CHECK_DEVICE_ACTIVE;\
    }

#define WAIT_DEVICE_ACTIVE_NORMAL WAIT_DEVICE_ACTIVE(30000)
