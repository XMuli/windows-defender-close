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
#include "mainui.h"
#include "./ui_mainui.h"
#include "dwctrl.h"
#include <QButtonGroup>
#include <QMetaEnum>
#include <QRadioButton>

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
    , m_DWCtrl(new DWCtrl)
{
    ui->setupUi(this);
    const auto& title = QString("%1 %2 [%3]").arg(MY_PROJECT_NAME).arg(MY_BUILD_TIME).arg(tr("Free"));
    setWindowTitle(title);  // AV Streaking
    resize(730, 370);

    initUI();
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::initUI()
{
    auto group = new QButtonGroup(this);
    group->addButton(ui->rbDisabled);
    group->addButton(ui->rbEnabled);
    group->addButton(ui->rbRemove);
    group->addButton(ui->rbDefault);
    group->setExclusive(true);

    void (QButtonGroup::* sigFun)(QAbstractButton*) = &QButtonGroup::buttonClicked;
    connect(group, sigFun, this, &MainUI::onRbClicked);
}

void MainUI::onRbClicked(QAbstractButton *bt)
{
    auto rb = qobject_cast<QRadioButton *>(bt);
    if (!rb)
        return;

    ui->textEdit->append(rb->text() + tr(" RadioButton is clicked."));
}


void MainUI::on_btActivate_released()
{
    auto group = findChild<QButtonGroup*>();
    if (!group)
        return;

    const auto& pbtn = qobject_cast<QRadioButton *>(group->checkedButton());
    if (!pbtn)
        return;

    WDValues val = WDValues::WD_Disabled;
    if (pbtn->objectName().compare("rbEnabled") == 0)
        val = WDValues::WD_Enabled;
    else if (pbtn->objectName().compare("rbDisabled") == 0)
        val = WDValues::WD_Disabled;
    else if (pbtn->objectName().compare("rbRemove") == 0)
        val = WDValues::WD_Remove;
    else if (pbtn->objectName().compare("rbDefault") == 0)
        val = WDValues::WD_Default;

    bool ok = m_DWCtrl->setWindowsDefender(val);

    QMetaEnum metaEnum = QMetaEnum::fromType<MainUI::WDValues>();
    QString msg = QString("setWindowsDefender: %1 is %2.").arg(metaEnum.valueToKey(val)).arg(QVariant(ok).toString());
    ui->textEdit->append(msg);

    if (!ok)
        ui->textEdit->append("Please try running with administrator.\n");
}

