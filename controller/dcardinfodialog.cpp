#include "dcardinfodialog.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFormLayout>

DCardInfoWidget::DCardInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(300, 180);
}

DCardInfoWidget::~DCardInfoWidget()
{

}

void DCardInfoWidget::showEvent(QShowEvent *event)
{
    updatePage();
    QWidget::showEvent(event);
}


void DCardInfoWidget::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 10, 10);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(38, 102, 192)));  //38, 102, 192

//    painter.setPen(Qt::white);
//    painter.drawLine(0, 140, 300, 140);
//    painter.drawLine(150, 140, 150, 180);
    return QWidget::paintEvent(event);
}

void DCardInfoWidget::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_startMovePos = event->globalPos();

    return QWidget::mousePressEvent(event);
}

void DCardInfoWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->pos();
        m_startMovePos = event->globalPos();
        this->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void DCardInfoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

DCardInfoDialog::DCardInfoDialog(QWidget *parent) : DCardInfoWidget(parent)
{
    initUI();
}

void DCardInfoDialog::updatePage()
{
    m_pCardIDLab->setText(tr("ID") + QString(": "));
    m_pBalanceLab->setText(tr("Balance") + QString(": "));
    m_pQuitBtn->setText(tr("Close"));
}

void DCardInfoDialog::setValue(const int value)
{
    m_pBalanceValue->setText(QString::number(value * 1.0 / 100, 'f', 2));
}

void DCardInfoDialog::setCardID(const QByteArray &id)
{
    m_pCardIDValue->setText(id);
}

void DCardInfoDialog::initUI()
{
    m_pCardIDLab = new QLabel;
    m_pCardIDValue = new QLabel;

    m_pBalanceLab = new QLabel;
    m_pBalanceValue = new QLabel;

    QFormLayout *form = new QFormLayout;
    form->addRow(m_pCardIDLab, m_pCardIDValue);
    form->addRow(m_pBalanceLab, m_pBalanceValue);

    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addStretch();
    h2->addLayout(form);
    h2->addStretch();

    QHBoxLayout *h3 = new QHBoxLayout;
    m_pQuitBtn = new QPushButton;
    h3->addStretch();
    h3->addWidget(m_pQuitBtn);
    h3->addStretch();

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(h2);
    mainlayout->addLayout(h3);

    this->setLayout(mainlayout);

    connect(m_pQuitBtn, SIGNAL(clicked()), this, SLOT(onQuitBtnClicked()));

    this->setStyleSheet("QPushButton{ \
                             background-color: #2666C0; \
                             color: white; \
                             border-width: 2px; \
                             border-color: white; \
                             min-height: 40px; \
                             min-width: 120; \
                         } \
                         QPushButton:hover{  \
                             background-color: #5052BF;\
                         } \
                         QPushButton:pressed{ \
                             background-color: #7471DD; \
                         }\
                         QLabel{ \
                             color:white; \
                             margin-top: 20px;\
                         }");
}

void DCardInfoDialog::onQuitBtnClicked()
{
    this->close();
}
