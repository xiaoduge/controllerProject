#ifndef DCARDINFODIALOG_H
#define DCARDINFODIALOG_H

#include <QWidget>

class QLabel;
class QPushButton;

class DCardInfoWidget : public QWidget
{
    Q_OBJECT

public:
    DCardInfoWidget(QWidget *parent = 0);
    ~DCardInfoWidget();

    virtual void updatePage() = 0;

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    bool m_isPressed;
    QPoint m_startMovePos;
};


class DCardInfoDialog : public DCardInfoWidget
{
    Q_OBJECT
public:
    explicit DCardInfoDialog(QWidget *parent = 0);

    void updatePage();
    void setValue(const int value);
    void setCardID(const QByteArray &id);

private:
    void initUI();

private slots:
    void onQuitBtnClicked();

private:
    QPushButton *m_pQuitBtn;

    QLabel      *m_pCardIDLab;
    QLabel      *m_pCardIDValue;

    QLabel      *m_pBalanceLab;
    QLabel      *m_pBalanceValue;
};

#endif // DCARDINFODIALOG_H
