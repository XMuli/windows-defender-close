// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/windows-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef DWCTRL_H
#define DWCTRL_H
#include "mainui.h"

using WDValues = MainUI::WDValues;

class DWCtrl
{
public:
    bool setWindowsDefender(WDValues val= WDValues::WD_Default);

private:
    bool setGroupPolicy(bool mark);
};

#endif // DWCTRL_H
