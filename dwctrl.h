/*******************************************************************
* Copyright (c) 2022~2024 XMuli All rights reserved.
*
* Author: XMuli <xmulitech@gmail.com>
* GitHub: https://github.com/XMuli/windows-defender-close
*
* Date:   2022.08.21
* Update: 2024.01.02
* Description:
******************************************************************/
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
