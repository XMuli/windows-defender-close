﻿// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2025 Wengeng Zhang
// SPDX-GitHub: https://github.com/XMuli/microsoft-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "mainui.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    setlocale(LC_ALL, "");
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough); // support 1.25 1.5
    QApplication a(argc, argv);

    MainUI w;
    w.show();

    return a.exec();
}
