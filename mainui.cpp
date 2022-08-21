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
#include "./ui_mainui.h"

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);
}

MainUI::~MainUI()
{
    delete ui;
}

