// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2025 Wengeng Zhang
// SPDX-GitHub: https://github.com/XMuli/microsoft-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "mainui.h"
#include "./ui_mainui.h"
#include <iostream>
#include <guiddef.h>
#include <combaseapi.h>
#include <prsht.h>
#include <GPEdit.h>
//#include <WbemCli.h>
#include <QMetaEnum>
#include <QTranslator>

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);
    QString name = QLocale::system().name().toLower();
    const auto& langMap = languageMap();
    bool isContain = std::find_if(langMap.begin(), langMap.end(), [&name](const std::pair<QString, QString>& pair) { return pair.second == name; }) != langMap.end();
    ui->cbbLanguage->setCurrentText(toLocaleNameByVal(isContain ? name : "en_us"));
    loadTranslation(isContain ? name : "en_us");

    const auto& title = tr("%1   %2").arg(MY_PROJECT_NAME).arg(_PROJECT_VERSION);
    setWindowTitle(title);  // AV Streaking
    resize(1000, 500);

    initUI();
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::loadTranslation(QString language)
{
    static QTranslator* translator = nullptr;
    if (!translator) translator = new QTranslator(this);

    QString qmFile = QString("%1.qm").arg(language);

    const QString& qmDir = qGuiApp->applicationDirPath() + "/translations/";
    auto t1 = qmDir + qmFile;
    if (translator->load(t1)) {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);

        const auto& title = QString("%1 %2").arg(MY_PROJECT_NAME).arg(_PROJECT_VERSION);
        setWindowTitle(title);  // AV Streaking
    }
}

void MainUI::setLabDescribeText(const WDValues &val)
{
    QString str = "";
    if (val == WDValues::WD_Stop) str = tr("Microsoft Defender Antivirus does not run, and will not scan computers for malware or other potentially unwanted software.");
    else if (val == WDValues::WD_Start) str = tr("Windows will internally manage Microsoft Defender Antivirus. If you install another antivirus program, Windows automatically disables Microsoft Defender Antivirus. Otherwise, Microsoft Defender Antivirus will scan your computers for malware and other potentially unwanted software.");
    else if (val == WDValues::WD_Fore_enable) str = tr("Microsoft Defender Antivirus will run regardless of any other installed antivirus product.");
    ui->labDescribe->setText(str);
}

void MainUI::btnActivate_released(const WDValues &val)
{
    setProperty("WDValues", QVariant::fromValue(val));
    setLabDescribeText(val);

    QString str;
    if (val == WDValues::WD_Stop) str = tr("STOP");
    else if (val == WDValues::WD_Start) str = tr("START");
    else if (val == WDValues::WD_Fore_enable) str = tr("FORE ENABLE");

    bool ok = setGroupPolicy(val);
    QString msg = tr("Set Microsoft Defender status: %1 is %2 %3!").arg(str).arg(ok ? tr("Succeed") : tr("Failed")).arg(ok ? "😎🎉🎉🎉" : "⚠️");
    ui->textEdit->clear();
    ui->textEdit->appendPlainText(msg);

    if (!ok) ui->textEdit->appendPlainText(tr("Please try running with administrator.\n"));
}

bool MainUI::setGroupPolicy(const WDValues &val)
{
    const IID IID_IGroupPolicyObject = { 0xea502723, 0xa23d, 0x11d1, {0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };
    const CLSID CLSID_GroupPolicyObject = { 0xea502722, 0xa23d, 0x11d1,{ 0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };
    const CLSID CLSID_GPESnapIn = { 0x8fc0b734, 0xa0e1, 0x11d1,{ 0xa7, 0xd3, 0x0, 0x0, 0xf8, 0x75, 0x71, 0xe3 } };

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

    if (val == WDValues::WD_Start) {

        // 删除 DisableAntiSpyware 键值
        status = RegDeleteValue(hKey, L"DisableAntiSpyware");

        if (status == ERROR_SUCCESS) {
            std::wcout << L"DisableAntiSpyware 删除成功" << std::endl;
        } else if (status == ERROR_FILE_NOT_FOUND) {
            std::wcout << L"DisableAntiSpyware 不存在，无需删除" << std::endl;
        } else {
            std::wcout << L"DisableAntiSpyware 删除失败，错误代码：" << status << std::endl;
            return false;
        }

    } else {

        DWORD lpData;
        if (val == WDValues::WD_Stop) lpData = 1;
        else if (val == WDValues::WD_Fore_enable) lpData = 0;
        status = RegSetKeyValue(hKey, NULL, L"DisableAntiSpyware", REG_DWORD, (LPCVOID)&lpData, sizeof(lpData));
        if (status == ERROR_SUCCESS) {
            std::wcout << L"DisableAntiSpyware RegSetKeyValue 键值" << int(lpData) << L"设置成功" << std::endl;
        } else {
            std::wcout << L"DisableAntiSpyware RegSetKeyValue 键值" << int(lpData) << L"设置失败" << std::endl;
            return false;
        }

    }

    status = RegCloseKey(hKey);

    GUID RegistryId = REGISTRY_EXTENSION_GUID;
    GUID ThisAdminToolGuid = { 0x0F6B957E, 0x509E, 0x11D1, { 0xA7, 0xCC, 0x00, 0x00, 0xF8, 0x75, 0x71, 0xE3 } }; /*{ CLSID_PolicySnapinUser/* */

    status = pGPO->Save(TRUE, TRUE, &RegistryId, &ThisAdminToolGuid);
    RegCloseKey(hGPOKey);
    pGPO->Release();
    std::wcout << L"Gpo save status " << status << std::endl;

    ::CoUninitialize();
    return true;
}

void MainUI::initUI()
{
}

QString MainUI::toLocaleNameByKey(const QString &language)
{
    const auto& map = languageMap();
    auto it = map.find(language);
    return it != map.cend() ? it->second : "en_us";
}

QString MainUI::toLocaleNameByVal(const QString &locale)
{
    for (const auto& pair : languageMap()) {
        if (pair.second == locale)
            return pair.first;
    }

    return "English";
}

std::map<QString, QString> MainUI::languageMap()
{
    static  std::map<QString, QString> map = {
          {"English", "en_us"}
        , {"简体中文", "zh_cn"}
        , {"繁体中文", "zh_tw"}
    };
    return map;
}

void MainUI::on_cbbLanguage_currentTextChanged(const QString &arg1)
{
    loadTranslation(toLocaleNameByKey(arg1));
    WDValues currentVal = qvariant_cast<WDValues>(property("WDValues"));
    setLabDescribeText(currentVal);
}

void MainUI::on_tbStop_released()
{
    btnActivate_released(WDValues::WD_Stop);
}

void MainUI::on_tbStart_released()
{
    btnActivate_released(WDValues::WD_Start);
}

void MainUI::on_tbForceEnable_released()
{
    btnActivate_released(WDValues::WD_Fore_enable);
}

