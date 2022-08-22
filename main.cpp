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

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    QApplication a(argc, argv);

    MainUI w;
    w.show();

    return a.exec();
}
