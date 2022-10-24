#ifndef DHINTDIALOG_H
#define DHINTDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;

class DHintDialog : public QDialog
{
    Q_OBJECT
private:
    explicit DHintDialog(QWidget *parent = 0);
    DHintDialog(const DHintDialog &obj);
    DHintDialog& operator =(const DHintDialog &obj);

public:
    static void getInstance(QString strText, int msec = 1000);
    ~DHintDialog();

protected:
    void timerEvent(QTimerEvent *event);

protected slots:
    void killSelf();


private:
    void setInfo(QString strText);
    void start(int msec);

private:
    QLabel *m_pLbInfo;
    QVBoxLayout *m_pBottomLayout;
    int m_timerID;

    static DHintDialog *instance;
    
};

#endif // EX_HINTDIALOG_H
