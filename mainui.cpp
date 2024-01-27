// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/windows-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "mainui.h"
#include "./ui_mainui.h"
#include "dwctrl.h"
#include <QButtonGroup>
#include <QMetaEnum>
#include <QRadioButton>
#include <QTranslator>

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
    , m_DWCtrl(new DWCtrl)
{
    ui->setupUi(this);
//    loadTranslation();
    const auto& title = QString("%1 %2 [%3]").arg(MY_PROJECT_NAME).arg(_PROJECT_VERSION).arg(tr("Free"));
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
    // 创建 QTranslator 对象
    static QTranslator* translator = nullptr;
    if (!translator) translator = new QTranslator(this);

    // 构建翻译文件的路径
    QString qmFile = QString("%1.qm").arg(language);

    const QString& qmDir = qGuiApp->applicationDirPath() + "/translations/";
    auto t1 = qmDir + qmFile;
    // 加载翻译文件
    if (translator->load(qmDir + qmFile)) {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);

        const auto& title = QString("%1 %2 [%3]").arg(MY_PROJECT_NAME).arg(_PROJECT_VERSION).arg(tr("Free"));
        setWindowTitle(title);  // AV Streaking
    }
}

void MainUI::initUI()
{
    ui->cbbLanguage->hide(); // 后续写
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


void MainUI::on_cbbLanguage_currentTextChanged(const QString &arg1)
{
    QString language = "en_us";
    if (arg1 == "简体中文") language = "zh_cn";
    else if (arg1 == "繁体中文") language = "zh_tw";
    else language = "en_us";

    loadTranslation(language);
}

