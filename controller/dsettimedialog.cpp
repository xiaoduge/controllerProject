/*****************************************************
**
** @author  : dcj
** @brief   : 时间设置弹框
** @date    : 2020-08-27 09:04:03
** @version : V0.0.1
**
*****************************************************/

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>
#include <time.h>

#include "dsettimedialog.h"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDateTime>
#include <QDebug>

DSetTimeDialog::DSetTimeDialog(QWidget *parent) : QDialog(parent)
{
    initUI();
    buildTranslation();
    this->setAttribute(Qt::WA_DeleteOnClose, true); //设置关闭对话框时销毁
}

DSetTimeDialog::~DSetTimeDialog()
{
    qDebug() << "~DSetTimeDialog()";
}

/**
 * @brief DSetTimeDialog::switchLanguage
 */
void DSetTimeDialog::switchLanguage()
{
    buildTranslation();
}

/**
 * @brief 设置UI文字描述
 */
void DSetTimeDialog::buildTranslation()
{
    this->setWindowTitle(tr("Set Time and Date"));
    m_pDateLab->setText(tr("Date"));
    m_pTimeLab->setText(tr("Time"));
    m_pModifyBtn->setText(tr("Save"));
}

/**
 * @brief 修改年份槽函数
 * @param text [修改后的年份]
 */
void DSetTimeDialog::on_Year_CurrentTextChanged(const QString &text)
{
    int year = text.toInt();
    checkYear(year);
    m_pTimeBox[DateTime_Month]->setCurrentIndex(0);
    m_pTimeBox[DateTime_Day]->setCurrentIndex(0);
}

/**
 * @brief 修改月份槽函数
 * @param text [修改后的月份]
 */
void DSetTimeDialog::on_Month_CurrentTextChanged(const QString &text)
{
    int month = text.toInt();
    m_pTimeBox[DateTime_Day]->clear();
    for(int i = 1; i <= m_dateMap[month]; i++)
    {
        m_pTimeBox[DateTime_Day]->addItem(QString::number(i));
    }
}

/**
 * @brief 保存当前时间，并退出
 */
void DSetTimeDialog::on_ModifyBtn_Clicked()
{
    time_t    Time_t;
    struct tm curtime;
    struct rtc_time rtctime ;

    int fb;

    int iYear,iMon,iDay,iHour,iMin,iSec;

    iSec  = m_pTimeBox[DateTime_Second]->currentText().toInt();
    iMin  = m_pTimeBox[DateTime_Minute]->currentText().toInt();
    iHour = m_pTimeBox[DateTime_Hour]->currentText().toInt();
    iDay  = m_pTimeBox[DateTime_Day]->currentText().toInt();
    iMon  = m_pTimeBox[DateTime_Month]->currentText().toInt();
    iYear = m_pTimeBox[DateTime_Year]->currentText().toInt();

    curtime.tm_sec  = iSec;
    curtime.tm_min  = iMin;
    curtime.tm_hour = iHour;
    curtime.tm_mday = iDay;
    curtime.tm_mon  = iMon  - 1;
    curtime.tm_year = iYear - 1900;
    curtime.tm_isdst= -1;

    Time_t = mktime(&curtime);

    stime(&Time_t);

    struct tm *tmp_ptr = NULL;

    tmp_ptr = gmtime(&Time_t);

    rtctime.tm_sec  = tmp_ptr->tm_sec;
    rtctime.tm_min  = tmp_ptr->tm_min;
    rtctime.tm_hour = tmp_ptr->tm_hour;
    rtctime.tm_mday = tmp_ptr->tm_mday;
    rtctime.tm_mon  = tmp_ptr->tm_mon;
    rtctime.tm_year = tmp_ptr->tm_year;
    rtctime.tm_isdst=-1;

    fb = ::open("/dev/rtc0", QIODevice::ReadWrite);
    if (fb != -1)
    {
        int ret = ioctl(fb , RTC_SET_TIME , &rtctime);

        if (ret)
        {
            qDebug() << " ioctl : " << ret;
        }
        ::close(fb);
    }

    this->close();
}

/**
 * @brief DSetTimeDialog::initUI
 */
void DSetTimeDialog::initUI()
{
    this->resize(500, 280);
    this->setWindowIcon(QIcon(":/pic/settime.png"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0, 103, 179));
    this->setPalette(palette);

    for (int i = 0; i < DateTime_NUM; i++)
    {
        m_pTimeBox[i] = new QComboBox(this);
    }

    m_pDateLab = new QLabel(tr("Date"), this);
    m_pTimeLab = new QLabel(tr("Time"), this);

    m_pModifyBtn = new QPushButton(tr("Save"), this);

    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout->addWidget(m_pTimeBox[DateTime_Year]);
    dateLayout->addWidget(m_pTimeBox[DateTime_Month]);
    dateLayout->addWidget(m_pTimeBox[DateTime_Day]);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(m_pTimeBox[DateTime_Hour]);
    timeLayout->addWidget(m_pTimeBox[DateTime_Minute]);
    timeLayout->addWidget(m_pTimeBox[DateTime_Second]);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_pModifyBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_pDateLab);
    mainLayout->addLayout(dateLayout);
    mainLayout->addWidget(m_pTimeLab);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(btnLayout);

    this->setLayout(mainLayout);
    initStyleSheet();
    initControl();
    initSlot();
}

/**
 * @brief 设置样式表
 */
void DSetTimeDialog::initStyleSheet()
{
    QFile qss(":/app/settimedlg.qss");
    qss.open(QFile::ReadOnly);
    QString strQss = QLatin1String (qss.readAll());
    qss.close();
    this->setStyleSheet(strQss);
}

/**
 * @brief 以当前系统时间初始化时间选框
 */
void DSetTimeDialog::initControl()
{
    for(int i = 1970; i < 2100; i++)
    {
        m_pTimeBox[DateTime_Year]->addItem(QString::number(i));
    }

    for(int i = 1; i < 13; i++)
    {
        m_pTimeBox[DateTime_Month]->addItem(QString::number(i));
    }

    QDateTime now = QDateTime::currentDateTime();
    QDate date = now.date();
    int year = date.year();
    int month = date.month();
    int day = date.day();

    checkYear(year);
    setDays(month);

    m_pTimeBox[DateTime_Year]->setCurrentIndex(m_pTimeBox[DateTime_Year]->findText(QString::number(year)));
    m_pTimeBox[DateTime_Month]->setCurrentIndex(m_pTimeBox[DateTime_Month]->findText(QString::number(month)));
    m_pTimeBox[DateTime_Day]->setCurrentIndex(m_pTimeBox[DateTime_Day]->findText(QString::number(day)));

    for(int i = 0; i < 24; i++)
    {
        m_pTimeBox[DateTime_Hour]->addItem(QString::number(i));
    }
    for(int i = 0; i < 60; i++)
    {
        m_pTimeBox[DateTime_Minute]->addItem(QString::number(i));
        m_pTimeBox[DateTime_Second]->addItem(QString::number(i));
    }

    QTime time = now.time();
    int hour = time.hour();
    int min = time.minute();
    int sec = time.second();

    m_pTimeBox[DateTime_Hour]->setCurrentIndex(m_pTimeBox[DateTime_Hour]->findText(QString::number(hour)));
    m_pTimeBox[DateTime_Minute]->setCurrentIndex(m_pTimeBox[DateTime_Minute]->findText(QString::number(min)));
    m_pTimeBox[DateTime_Second]->setCurrentIndex(m_pTimeBox[DateTime_Second]->findText(QString::number(sec)));
}

/**
 * @brief 关联相关的信号与槽
 */
void DSetTimeDialog::initSlot()
{
    connect(m_pTimeBox[DateTime_Year],  SIGNAL(currentIndexChanged(const QString&)), this, SLOT(on_Year_CurrentTextChanged(const QString&)));
    connect(m_pTimeBox[DateTime_Month], SIGNAL(currentIndexChanged(const QString&)), this, SLOT(on_Month_CurrentTextChanged(const QString&)));

    connect(m_pModifyBtn, SIGNAL(clicked()), this, SLOT(on_ModifyBtn_Clicked()));
}

/**
 * @brief 设置每个月的天数
 */
void DSetTimeDialog::updateDateMap()
{
    m_dateMap.clear();

    m_dateMap.insert(1, 31);
    m_dateMap.insert(2, 28);
    m_dateMap.insert(3, 31);
    m_dateMap.insert(4, 30);
    m_dateMap.insert(5, 31);
    m_dateMap.insert(6, 30);
    m_dateMap.insert(7, 31);
    m_dateMap.insert(8, 31);
    m_dateMap.insert(9, 30);
    m_dateMap.insert(10, 31);
    m_dateMap.insert(11, 30);
    m_dateMap.insert(12, 31);

    if(m_bLeapYear)
    {
        m_dateMap[2] = 29;
    }

}

/**
 * @brief 检查当前年份是闰年还是平年，并更新每个月的天数
 * @param year [当前年份]
 */
void DSetTimeDialog::checkYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        m_bLeapYear = true;
        qDebug() << "Leap Year";
    }
    else
    {
        m_bLeapYear = false;
        qDebug() << "Common year";
    }
    updateDateMap();
}

/**
 * @brief 设置指定月份的天数
 * @param month [当前月份]
 */
void DSetTimeDialog::setDays(int month)
{
    m_pTimeBox[DateTime_Day]->clear();
    for(int i = 1; i <= m_dateMap[month]; i++)
    {
        m_pTimeBox[DateTime_Day]->addItem(QString::number(i));
    }
}
