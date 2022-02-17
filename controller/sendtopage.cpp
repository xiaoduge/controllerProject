#include "sendtopage.h"
#include "mainwindow.h"
#include "cbitmapbutton.h"
#include "dhintdialog.h"
#include <QTextStream>
#include <QListWidgetItem>
#include <QDir>
#include <QSqlQuery>
#include <QPrinter>

SendToPage::SendToPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    // creatTitle();

    initUi();

    buildTranslation();
}

void SendToPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SendToPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("SendToPage");

    setTitles(stringList);

}

void SendToPage::buildTranslation()
{
    int iLoop;

    CancelBtn->setTip(tr("Cancel"));
    SaveBtn->setTip(tr("OK"));
    lbTitle->setText(tr("send to"));

    for(iLoop = 0 ; iLoop < RECEIVER ; iLoop++)
    {
        lbName[iLoop]->setText(tr("send to"));
    }    
}

void SendToPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void SendToPage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

QColor SetColor[RECEIVER] = {
    QColor(255,255,255),
    QColor(255,255,255),
    QColor(255,255,255),
};

QString img_name[RECEIVER] = {
    QString(":/pic/computer.png"),
    QString(":/pic/Email.png"),
    QString(":/pic/Udisk.png"),
};

void SendToPage::initUi()
{
    setBackColor();

    QPalette pal;

    QString strQss4Chk = m_wndMain->getQss4Chk();

    lbTitle = new QLabel(m_widget);
    lbTitle->setGeometry(QRect(20,56,100,25));
    lbTitle->setStyleSheet(" font-size:18pt;color:#15191d;font-family:Arial;QFont::Bold;border:0px solid #dfe7ed");

    //m_widget->setStyleSheet("background-color:#ffffff;border:1px solid #dfe7ed");

    QPixmap imgType;

    for(int iLoop = 0 ; iLoop < RECEIVER ; iLoop++)
    {
        m_pSetWidget[iLoop] = new QWidget(m_widget);

        pal.setBrush(m_pSetWidget[iLoop]->backgroundRole(),QBrush(QColor(255,255,255)));

        m_pSetWidget[iLoop]->setAutoFillBackground(true);
        m_pSetWidget[iLoop]->setPalette(pal);
        m_pSetWidget[iLoop]->setGeometry(QRect(20,110 + 40 * iLoop,280,40));

        if(iLoop % 2)
        {
            m_pSetWidget[iLoop]->setStyleSheet("background-color:#ffffff;border:1px solid #dfe7ed");
        }
        else
        {
            m_pSetWidget[iLoop]->setStyleSheet("background-color:#f4f9fa;border:1px solid #dfe7ed");
        }

        imgType.load(img_name[iLoop]);

        lbPic[iLoop] = new QLabel(m_pSetWidget[iLoop]);
        lbPic[iLoop]->setGeometry(QRect(10,10,32,28));
        lbPic[iLoop]->setPixmap(imgType);
        lbPic[iLoop]->setStyleSheet("border:0px");

        lbName[iLoop] = new QLabel(m_pSetWidget[iLoop]);
        lbName[iLoop]->setGeometry(QRect(52,10,100,28));
        lbName[iLoop]->setStyleSheet(" font-size:18pt;color:#15191d;font-family:Arial;border:0px");

        m_chkSwitchs[iLoop] = new QCheckBox(m_pSetWidget[iLoop]);

        m_chkSwitchs[iLoop]->setGeometry(QRect(230 , 9 ,40,40));

        m_chkSwitchs[iLoop]->setStyleSheet(strQss4Chk);

        m_chkSwitchs[iLoop]->show();

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));
    }

    CancelBtn = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SENDPAGE_BTN_CANCEL);
    CancelBtn->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_NORMAL]);
    CancelBtn->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_CANEL_ACTIVE]);
    CancelBtn->setGeometry(QRect(60 , 285 ,CancelBtn->width(),CancelBtn->height()));
    CancelBtn->setStyleSheet("background-color:transparent");

    connect(CancelBtn, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));

    SaveBtn   = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SNEDPAGE_BTN_OK);
    SaveBtn->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_NORMAL]);
    SaveBtn->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_OK_ACTIVE]);
    SaveBtn->setGeometry(QRect(182 , 285 ,SaveBtn->width(),SaveBtn->height()));
    SaveBtn->setStyleSheet("background-color:transparent");

    connect(SaveBtn, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
}

void SendToPage::copyAlarmFile()
{
    QFile file("/media/sda1/Data/alarm_history.csv");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:alarm_history"), QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << tr("ID") << ","
        << tr("type") << ","
        << tr("Status") << ","
        << tr("Time") << "\n";

    QSqlQuery query;
    query.exec("select * from Alarm");
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toString() << ","
            << query.value(2).toInt() << ","
            << query.value(3).toString() << "\n";
    }
    file.close();

   //tobase64
    if(!file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:alarm_history.dcj"), QMessageBox::Ok);
        return;
    }
    QByteArray content = file.readAll().toBase64();
    QFile fileBase64("/media/sda1/Data/alarm_history.dcj");
    if(!fileBase64.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:alarm_history.dcj"), QMessageBox::Ok);
        return;
    }
    fileBase64.write(content);

    file.close();
    fileBase64.close();

    QFile::remove("/media/sda1/Data/alarm_history.csv");
}

void SendToPage::copyGetWater()
{
    QFile file("/media/sda1/Data/dispense_history.csv");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:dispense_history"), QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << tr("ID") << ","
        << tr("WName") << ","
        << tr("Quantity") << ","
        << tr("Quality") << ","
        << tr("TOC") << ","
        << tr("Tmp") << ","
        << tr("Time") << "\n";

    QSqlQuery query;
    query.exec("select * from GetW");
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toString() << ","
            << query.value(2).toDouble() << ","
            << query.value(3).toDouble() << ","
            << query.value(4).toInt() << ","
            << query.value(5).toDouble() << ","
            << query.value(6).toString() << "\n";
    }
    file.close();

    //tobase64
    if(!file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:dispense_history.dcj"), QMessageBox::Ok);
        return;
    }
    QByteArray content = file.readAll().toBase64();
    QFile fileBase64("/media/sda1/Data/dispense_history.dcj");
    if(!fileBase64.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:dispense_history.dcj"), QMessageBox::Ok);
        return;
    }
    fileBase64.write(content);

    file.close();
    fileBase64.close();

    QFile::remove("/media/sda1/Data/dispense_history.csv");
}

void SendToPage::copyProduceWater()
{
    QFile file("/media/sda1/Data/product_history.csv");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:product_history"), QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << tr("ID") << ","
        << tr("Duration") << ","
        << tr("Ecoroin") << ","
        << tr("Tmproin") << ","
        << tr("Ecorores") << ","
        << tr("eEcoropw") << ","
        << tr("Tmpropw") << ","
        << tr("Ecoedi") << ","
        << tr("Tmpedi") << ","
        << tr("Time") << "\n";

    QSqlQuery query;
    query.exec("select * from PWater");
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toInt() << ","
            << query.value(2).toDouble() << ","
            << query.value(3).toDouble() << ","
            << query.value(4).toDouble() << ","
            << query.value(5).toDouble() << ","
            << query.value(6).toDouble() << ","
            << query.value(7).toDouble() << ","
            << query.value(8).toDouble() << ","
            << query.value(9).toString() << "\n";
    }
    file.close();

    //tobase64
    if(!file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:product_history.dcj"), QMessageBox::Ok);
        return;
    }
    QByteArray content = file.readAll().toBase64();
    QFile fileBase64("/media/sda1/Data/product_history.dcj");
    if(!fileBase64.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:product_history.dcj"), QMessageBox::Ok);
        return;
    }
    fileBase64.write(content);

    file.close();
    fileBase64.close();

    QFile::remove("/media/sda1/Data/product_history.csv");
}

void SendToPage::copyLog()
{
    QFile file("/media/sda1/Data/log.csv");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:log"), QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << tr("ID") << ","
        << tr("Name") << ","
        << tr("Action") << ","
        << tr("Info") << ","
        << tr("Time") << "\n";

    QSqlQuery query;
    query.exec("select * from Log");
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toString() << ","
            << query.value(2).toString() << ","
            << query.value(3).toString() << ","
            << query.value(4).toString() << "\n";
    }
    file.close();

    //tobase64
    if(!file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:log.dcj"), QMessageBox::Ok);
        return;
    }
    QByteArray content = file.readAll().toBase64();
    QFile fileBase64("/media/sda1/Data/log.dcj");
    if(!fileBase64.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:log.dcj"), QMessageBox::Ok);
        return;
    }
    fileBase64.write(content);

    file.close();
    fileBase64.close();

    QFile::remove("/media/sda1/Data/log.csv");
}

#ifdef WATERCARDREADER
void SendToPage::copyWaterCardInfo()
{
    QFile file("/media/sda1/Data/watercard.csv");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:watercard"), QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << tr("ID") << ","
        << tr("CardID") << ","
        << tr("Balance") << ","
        << tr("Time") << "\n";

    QSqlQuery query;
    query.exec("select * from WaterCards");
    while(query.next())
    {
        out << query.value(0).toInt() << ","
            << query.value(1).toString() << ","
            << query.value(2).toDouble() << ","
            << query.value(3).toString() << "\n";
    }
    file.close();

    //tobase64
    if(!file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:watercard.dcj"), QMessageBox::Ok);
        return;
    }
    QByteArray content = file.readAll().toBase64();
    QFile fileBase64("/media/sda1/Data/watercard.dcj");
    if(!fileBase64.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Failed to send data:watercard.dcj"), QMessageBox::Ok);
        return;
    }
    fileBase64.write(content);

    file.close();
    fileBase64.close();

    QFile::remove("/media/sda1/Data/watercard.csv");

}
#endif

void SendToPage::copyHistoryToUsb()
{
    QString pathName = QString("/media/sda1");
    QDir dir(pathName);
    if(!dir.exists())
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("U disk does not exist"), QMessageBox::Ok);
        return;
    }

    pathName = QString("/media/sda1/Data");
    dir.setPath(pathName);
    if(!dir.exists())
    {
        bool ok = dir.mkdir(pathName);
        if(!ok)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Directory does not exist"), QMessageBox::Ok);
            return;
        }
    }

    if(m_chkSwitchs[2]->checkState() == Qt::Checked)
    {
        copyAlarmFile();
        copyGetWater();
        copyProduceWater();
        copyLog();
#ifdef WATERCARDREADER
        copyWaterCardInfo();
#endif
        DHintDialog::getInstance(tr("Successfully"));
    }
}

void SendToPage::on_btn_clicked(int btnID)
{
    switch(btnID)
    {
    case SNEDPAGE_BTN_OK:
        copyHistoryToUsb();
        break;
    default:
        break;
    }
    show(false);

}

void SendToPage::on_checkBox_changeState(int state)
{
    //int iLoop;
    
    QCheckBox *pChkBox = (QCheckBox *)this->sender();

    if (!pChkBox)
    {
        return ;
    }
    
    //int tmp = (Qt::Checked == pChkBox->checkState()) ? 1 : 0;

}

