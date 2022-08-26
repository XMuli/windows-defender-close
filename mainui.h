/*******************************************************************
* Copyright (c) 2022 XMuli All rights reserved.
*
* Author: XMuli <xmulitech@gmail.com>
* GitHub: https://github.com/XMuli

*
* Date:   2022.08.21
* Description:
******************************************************************/
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

public slots:
    void onRbClicked(QAbstractButton* bt);
    void on_btActivate_released();

private:
    Ui::MainUI *ui;
    DWCtrl* m_DWCtrl;
};
#endif // MAINUI_H
