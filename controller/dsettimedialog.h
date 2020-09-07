/*****************************************************
**
** @author  : dcj
** @brief   : 时间设置弹框
** @date    : 2020-08-27 09:04:03
** @version : V0.0.1
**
*****************************************************/

#ifndef DSETTIMEDIALOG_H
#define DSETTIMEDIALOG_H

#include <QDialog>
#include <QMap>

class QComboBox;
class QLabel;
class QPushButton;

class DSetTimeDialog : public QDialog
{
    Q_OBJECT
public:
    enum DateTime
    {
        DateTime_Second,
        DateTime_Minute,
        DateTime_Hour,
        DateTime_Day,
        DateTime_Month,
        DateTime_Year,
        DateTime_NUM
    };
public:
    explicit DSetTimeDialog(QWidget *parent = 0);
    ~DSetTimeDialog();

    void switchLanguage();
    void buildTranslation();

public slots:
    void on_Year_CurrentTextChanged(const QString &text);
    void on_Month_CurrentTextChanged(const QString &text);
    void on_ModifyBtn_Clicked();

private:
    void initUI();
    void initStyleSheet();
    void initControl();
    void initSlot();

    void updateDateMap();

    void checkYear(int year);
    void setDays(int month);

private:
    QComboBox *m_pTimeBox[DateTime_NUM];
    QLabel *m_pDateLab;
    QLabel *m_pTimeLab;

    QPushButton *m_pModifyBtn;

    bool m_bLeapYear; //判断是否是闰年

    QMap<int, int> m_dateMap;
};

#endif // DSETTIMEDIALOG_H
