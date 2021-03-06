#ifndef CONSUMABLESTATEPAGE_H
#define CONSUMABLESTATEPAGE_H

#include "subpage.h"
#include "Display.h"

#define CS_MAX_CsListNum (DISP_ROC12 + 1)

class MainWindow;
class CsListItem;
class QListWidget;
class QListWidgetItem;

class ConsumableStatePage : public CSubPage
{
    Q_OBJECT
public:
    ConsumableStatePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    virtual void update();

private:

    void createList();

    void setBackColor();

    void buildTitles();

    struct CS_INFO{
       int iType;
       int iId;
    }aIds[CS_MAX_CsListNum];

    CsListItem      *m_pCslistItem[CS_MAX_CsListNum];
    QListWidgetItem *listWidgetItem[CS_MAX_CsListNum];
    QListWidget     *m_listWidget;
    int              m_realCsNum;

public slots:
    
    void on_btn_clicked(int index);
};

#endif // CONSUMABLESTATEPAGE_H
