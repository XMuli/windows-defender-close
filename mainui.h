// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/windows-defender-close
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
class DWCtrl;

QT_BEGIN_NAMESPACE
class QAbstractButton;
namespace Ui { class MainUI; }
QT_END_NAMESPACE

class MainUI : public QWidget
{
    Q_OBJECT

public:
    enum WDValues {
        WD_Enabled,
        WD_Disabled,
        WD_Remove,
        WD_Default
    };
    Q_ENUM(WDValues)

    MainUI(QWidget *parent = nullptr);
    ~MainUI();

private:
    void initUI();
    void loadTranslation(QString language = "en_us");  // 默认加载系统语言

public slots:
    void onRbClicked(QAbstractButton* bt);
    void on_btActivate_released();
    void on_cbbLanguage_currentTextChanged(const QString &arg1);

private:
    Ui::MainUI *ui;
    DWCtrl* m_DWCtrl;
};
#endif // MAINUI_H
