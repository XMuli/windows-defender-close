/*******************************************************************
* Copyright (c) 2022 XMuli All rights reserved.
*
* Author: XMuli <xmulitech@gmail.com>
* GitHub: https://github.com/XMuli
* Blog:   https://ifmet.cn
*
* Date:   2022.08.21
* Description:
******************************************************************/
#include "dwctrl.h"
#include <iostream>
#include <guiddef.h>
#include <combaseapi.h>
#include <prsht.h>
#include <GPEdit.h>
//#include <WbemCli.h>

const IID IID_IGroupPolicyObject = { 0xea502723, 0xa23d, 0x11d1, {0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };
const CLSID CLSID_GroupPolicyObject = { 0xea502722, 0xa23d, 0x11d1,{ 0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };
const CLSID CLSID_GPESnapIn = { 0x8fc0b734, 0xa0e1, 0x11d1,{ 0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };

void DWCtrl::disableWD()
{
    setGroupPolicy(true);
}

bool DWCtrl::setGroupPolicy(bool mark)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    LRESULT status;
    LRESULT hr = S_OK;
    IGroupPolicyObject* pGPO = NULL;
    hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER, IID_IGroupPolicyObject, (LPVOID*)&pGPO);
    if (hr == S_OK) {
        std::wcout << L"GPO 创建成功" << std::endl;
    } else {
        std::wcout << L"GPO 创建失败" << std::endl;
        return false;
    }

    HKEY hGPOKey = 0;
    hr = pGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
    if (SUCCEEDED(hr)) {
        std::wcout << L"打开本地机器成功" << std::endl;
    } else {
        std::wcout << L"打开本地机器失败" << std::endl;
        return false;
    }

    hr = pGPO->GetRegistryKey(GPO_SECTION_MACHINE, &hGPOKey);
    if (SUCCEEDED(hr)) {
        std::wcout << L"加载注册表成功" << std::endl;
    } else {
        std::wcout << L"加载注册表失败" << std::endl;
        return false;
    }

    //开关
    HKEY hKey;
    status = RegOpenKeyEx(hGPOKey, L"Software\\Policies\\Microsoft\\Windows Defender", 0, KEY_WRITE, &hKey);
    if (status != ERROR_SUCCESS) {
        status = RegCreateKeyEx(hGPOKey, L"Software\\Policies\\Microsoft\\Windows Defender", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

        if (status == ERROR_SUCCESS) {
            std::wcout << L"键值创建成功" << std::endl;
        } else {
            std::wcout << L"键值创建失败" << std::endl;
            return false;
        }
    }

    DWORD lpData;
    mark ? lpData = 1 : lpData = 0;
    status = RegSetKeyValue(hKey, NULL, L"DisableAntiSpyware", REG_DWORD, (LPCVOID)&lpData, sizeof(lpData));
    if (status == ERROR_SUCCESS) {
        std::wcout << L"DisableAntiSpyware RegSetKeyValue 键值设置成功" << std::endl;
    } else {
        std::wcout << L"DisableAntiSpyware RegSetKeyValue 键值设置失败" << std::endl;
            return false;
    }
    status = RegCloseKey(hKey);

    //GUID Registerid = REGISTRY_EXTENSION_GUID;
    ////GUID ThisGuid = {
    ////0xf323e14,
    ////0xf35a,
    ////0x4583,
    ////{0x8c, 0x96, 0x30, 0xcb, 0x6c, 0x11, 0xc8, 0x44}
    ////};
    //GUID ThisGuid = CLSID_GPESnapIn;

    GUID RegistryId = REGISTRY_EXTENSION_GUID;
    GUID ThisAdminToolGuid = { 0x0F6B957E, 0x509E, 0x11D1, { 0xA7, 0xCC, 0x00, 0x00, 0xF8, 0x75, 0x71, 0xE3 } }; /*{ CLSID_PolicySnapinUser/* */

    status = pGPO->Save(TRUE, TRUE, &RegistryId, &ThisAdminToolGuid);
    RegCloseKey(hGPOKey);
    pGPO->Release();
    std::wcout << L"Gpo save status " << status << std::endl;

    ::CoUninitialize();
    return true;
}
