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
#ifndef DWCTRL_H
#define DWCTRL_H

class DWCtrl
{
public:
    void disableWD();

private:
    bool setGroupPolicy(bool mark);
};

#endif // DWCTRL_H
