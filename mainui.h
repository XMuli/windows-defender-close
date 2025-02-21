// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2022-2025 XMuli
// SPDX-GitHub: https://github.com/XMuli/microsoft-defender-close
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
        WD_Stop,
        WD_Start,
        WD_Fore_enable
    };
    Q_ENUM(WDValues)

    MainUI(QWidget *parent = nullptr);
    ~MainUI();

private:
    void initUI();
    QString toLocaleNameByKey(const QString& language);
    QString toLocaleNameByVal(const QString& locale);
    std::map<QString, QString> languageMap();

    void loadTranslation(QString language = "en_us");
    void setLabDescribeText(const WDValues &val);

    void btnActivate_released(const WDValues &val);
    bool setGroupPolicy(const WDValues& val);

public slots:
    void on_cbbLanguage_currentTextChanged(const QString &arg1);

private slots:
    void on_tbStop_released();
    void on_tbStart_released();
    void on_tbForceEnable_released();

private:
    Ui::MainUI *ui;

};
#endif // MAINUI_H
