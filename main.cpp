// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/windows-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

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
