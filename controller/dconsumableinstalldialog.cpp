#include "dconsumableinstalldialog.h"
#include "Display.h"
#include "mainwindow.h"
#include "dlineedit.h"
#include "exconfig.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

DConsumableInstallDialog::DConsumableInstallDialog(int id, QWidget * parent) :
    DBaseWindow(parent),
    m_instanceID(id)
{
    initUI();
    buildTranslation();
    this->setWindowTitle(tr("Components Installation"));
}


void DConsumableInstallDialog::switchLanguage()
{
    buildTranslation();
}

void DConsumableInstallDialog::initUI()
{
    m_pcomboBox = new QComboBox(this);

    m_pCatLabel = new QLabel(this);
    QPalette palette = m_pCatLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pCatLabel->setPalette(palette);

    m_pLineEditCat = new DLineEdit(this);
    m_pLineEditCat->setAlignment(Qt::AlignCenter);
    m_pLineEditCat->setReadOnly(true);

    m_pLotLabel = new QLabel(this);
    palette = m_pLotLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pLotLabel->setPalette(palette);

    m_pLineEditLot = new DLineEdit(this);
    m_pLineEditLot->setAlignment(Qt::AlignCenter);
    m_pLineEditLot->setReadOnly(true);

    m_pUserLabel = new QLabel(this);
    palette = m_pUserLabel->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    m_pUserLabel->setPalette(palette);

    m_pLineEditUser = new DLineEdit(this);
    m_pLineEditUser->setAlignment(Qt::AlignCenter);

    m_pInstallBtn = new QPushButton(tr("Install"), this);

    m_pcomboBox->setGeometry(120, 50, 180, 30);
    m_pLineEditCat->setGeometry(120, 80, 180, 30);
    m_pLineEditLot->setGeometry(120, 110, 180, 30);
    m_pLineEditUser->setGeometry(120, 140, 180, 30);

    m_pCatLabel->setGeometry(25, 80, 90, 30);
    m_pLotLabel->setGeometry(25, 110, 90, 30);
    m_pUserLabel->setGeometry(25, 140, 90, 30);

    m_pInstallBtn->setGeometry(120, 180, 180, 30); //135, 180, 100, 30
    m_pInstallBtn->setObjectName("loginButton");
    QString qss = QString("QPushButton#loginButton\
                         {\
                            color:white;\
                            background-color:rgb(14 , 150 , 254);\
                            border-radius:5px;\
                         }\
                         QPushButton#loginButton:hover \
                         {\
                            color:white;\
                            background-color:rgb(44 , 137 , 255);\
                         }\
                         QPushButton#loginButton:pressed\
                         {\
                            color:white;\
                            background-color:rgb(14 , 135 , 228);\
                            padding-left:3px;\
                            padding-top:3px;\
                         }");
    m_pInstallBtn->setStyleSheet(qss);


    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
    {
        m_pUserLabel->hide();
        m_pLineEditUser->hide();
    }

    connect(m_pInstallBtn, SIGNAL(clicked()), this, SLOT(on_installBtn_clicked()));
    connect(m_pcomboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboBox_currentIndexChanged(int)));
}

bool DConsumableInstallDialog::checkUserInfo(const QString &userName)
{
    if((userName.compare("Super", Qt::CaseInsensitive) == 0)
     ||(userName.compare("Service", Qt::CaseInsensitive) == 0)
     ||(userName.compare("Manager", Qt::CaseInsensitive) == 0))
    {
        return true;
    }

    QString strQuery = QString("select * from User where name = '%1'").arg(userName);
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        if((userName.compare(name, Qt::CaseInsensitive) == 0))
        {
            return true;
        }
    }

    return false;
}

void DConsumableInstallDialog::buildTranslation()
{
    this->setTitleText(tr("Components Installation"), Qt::white, 16);

    m_pCatLabel->setText(tr("Cat No.:"));
    m_pLotLabel->setText(tr("Lot No.:"));
    m_pUserLabel->setText(tr("User Name:"));

    m_pInstallBtn->setText(tr("Install"));
}

void DConsumableInstallDialog::updatePage()
{
    buildTranslation();
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization))
    {
        if(m_pUserLabel->isVisible())
        {
            m_pUserLabel->hide();
            m_pLineEditUser->hide();
        }
    }
    else
    {
        if(!m_pUserLabel->isVisible())
        {
            m_pUserLabel->show();
            m_pLineEditUser->show();
        }
    }
}

void DConsumableInstallDialog::on_closeBtn_clicked()
{
    this->close();
}

void DConsumableInstallDialog::on_installBtn_clicked()
{
    if (!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_User_Authorization)))
    {
        QString userName = m_pLineEditUser->text();
        if(userName.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter a valid username"), QMessageBox::Ok);
            return;
        }

        bool ok = this->checkUserInfo(userName);
        if(!ok)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Invalid username"), QMessageBox::Ok);
            return;
        }
        gpMainWnd->saveLoginfo(userName);
    }
    else
    {
        gpMainWnd->saveLoginfo("unknow");
    }

    emit installConusumable();
    this->close();
}

void DConsumableInstallDialog::on_comboBox_currentIndexChanged(int index)
{

}

void DConsumableInstallDialog::setConsumableName(int iType, const QString &catNo, const QString &lotNo)
{
    if(this->isVisible())
    {
        return;
    }
    m_pcomboBox->clear();
    m_pLineEditUser->clear();
    m_iType = iType;
    switch(iType)
    {
    case DISP_P_PACK:
        m_pcomboBox->addItem(tr("P Pack"));
        break;
    case DISP_AC_PACK:
        m_pcomboBox->addItem(tr("AC Pack"));
        break;
    case DISP_U_PACK:
        m_pcomboBox->addItem(tr("U Pack"));
        break;
    case DISP_H_PACK:
        m_pcomboBox->addItem(tr("H Pack"));
        break;
    case DISP_PRE_PACK:
        m_pcomboBox->addItem(tr("Prefilter"));
        break;
    case DISP_ICP_PACK:
        m_pcomboBox->addItem(tr("ICP Pack"));
        break;
    case DISP_P_PACK | (1 << 16):
        m_pcomboBox->addItem(tr("Clean Pack"));
        break;
    case DISP_AT_PACK:
        m_pcomboBox->addItem(tr("AT Pack"));
        break;
    case DISP_T_PACK:
        m_pcomboBox->addItem(tr("T Pack"));
        break;
    case DISP_MACHINERY_RO_MEMBRANE:
        m_pcomboBox->addItem(tr("RO Membrane"));
        break;
    case DISP_N2_UV:
        m_pcomboBox->addItem(tr("185 UV Lamp"));
        break;
    case DISP_N1_UV:
        m_pcomboBox->addItem(tr("254 UV Lamp"));
        break;
    case DISP_N3_UV:
        m_pcomboBox->addItem(tr("Tank UV Lamp"));
        break;
    case DISP_MACHINERY_RO_BOOSTER_PUMP:
        m_pcomboBox->addItem(tr("RO Pump"));
        break;
    case DISP_MACHINERY_CIR_PUMP:
        m_pcomboBox->addItem(tr("Recir. Pump"));
        break;
    case DISP_T_A_FILTER:
        m_pcomboBox->addItem(tr("Final Fliter A"));
        break;
    case DISP_T_B_FILTER:
        if(0 == gAdditionalCfgParam.productInfo.iCompany)
        {
             m_pcomboBox->addItem(tr("Final Fliter B"));
        }
        else
        {
            m_pcomboBox->addItem(tr("Bio-filter"));
        }
        break;
    case DISP_MACHINERY_EDI:
        m_pcomboBox->addItem(tr("EDI Module"));
        break;
    case DISP_W_FILTER:
        m_pcomboBox->addItem(tr("Tank Vent Filter"));
        break;
    case DISP_TUBE_FILTERLIFE:
        m_pcomboBox->addItem(tr("Loop Filter"));
        break;
    case DISP_N4_UV:
        m_pcomboBox->addItem(tr("Tube UV"));
        break;
	case DISP_TUBE_DI:
		m_pcomboBox->addItem(tr("Loop DI"));
    default:
        break;
    }

    m_pLineEditCat->setText(catNo);
    m_pLineEditLot->setText(lotNo);

    this->show();
}

void DConsumableInstallDialog::closeEvent(QCloseEvent *e)
{
    emit setCheckStatus(false);
    QWidget::closeEvent(e);
}
