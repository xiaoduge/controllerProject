#include "dhintdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimerEvent>
#include <QMutexLocker>
#include <QTimer>
#include <QDebug>

DHintDialog *DHintDialog::instance = NULL;

QMutex hintDialogmutex;

DHintDialog::DHintDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QColor(112,128,144));
    this->setPalette(palette);

    palette = this->palette();
    palette.setBrush(QPalette::WindowText, Qt::white);
    this->setPalette(palette);//

    setFixedSize(250, 70); //200 60

    m_pLbInfo    = new QLabel;
    m_pLbInfo->setAlignment(Qt::AlignCenter);

    m_pBottomLayout =  new QVBoxLayout;

    m_pBottomLayout->addWidget(m_pLbInfo);

    setLayout(m_pBottomLayout);

}

DHintDialog::~DHintDialog()
{
    qDebug() << tr("~DHintDialog()");
}

void DHintDialog::setInfo(QString strText)
{
    m_pLbInfo->setText(strText);
}


void DHintDialog::getInstance(QString strText, int msec)
{
    if (NULL == instance)
    {
        QMutexLocker locker(&hintDialogmutex);
        if(NULL == instance)
        {
            DHintDialog *pDlg = new DHintDialog();
            pDlg->setInfo(strText);
            pDlg->start(msec);
            instance = pDlg;
        }
    }
}

void DHintDialog::start(int msec)
{
//    m_timerID = startTimer(msec);
    QTimer::singleShot(msec, this, SLOT(killSelf()));

    show();
}

void DHintDialog::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerID)
    {
        if(instance != NULL)
        {
            killTimer(m_timerID);
            delete this;
            instance = NULL;
        }
    }
}

void DHintDialog::killSelf()
{
    if(instance != NULL)
    {
        delete this;
        instance = NULL;
    }
}


