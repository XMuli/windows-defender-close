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
#include "mainui.h"
#include <iostream>
#include <QApplication>
#include "dwctrl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setlocale(LC_ALL, "");

    MainUI w;
    w.show();

    DWCtrl dwCtrl;
    dwCtrl.disableWD();

    return a.exec();
}
