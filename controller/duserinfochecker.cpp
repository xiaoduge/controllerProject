#include "duserinfochecker.h"
#include "mainwindow.h"
#include "exconfig.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "dloginwarningdialog.h"

DUserInfoChecker::DUserInfoChecker(QObject *parent) :
    QObject(parent)
{
}

int DUserInfoChecker::checkUserInfo(const QString &userName, const QString &passWord)
{
    //超级用户，内部使用
    if((userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Super_Name]) == 0))
    {
        return 4;
    }
    //工程师权限，
    if((userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Service_Name]) == 0))
    {
        return 3;
    }

    //VWR， 超级用户
    if((userName.compare(managerName[Super_Service_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Super_Service_Name]) == 0))
    {
        return 31;
    }

    //用户管理权限，
    if((userName.compare(managerName[Manager_Name], Qt::CaseInsensitive) == 0)
        && (passWord.compare(userPassword[Manager_Name]) == 0))
    {
        return 2;
    }


    QString strQuery = "select * from User where Permission = 2";
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        QString pass = query.value(2).toString();
        if((userName.compare(name, Qt::CaseInsensitive) == 0) && (passWord.compare(pass, Qt::CaseInsensitive) == 0))
        {
            return 2;
        }
    }

    strQuery = "select * from User where Permission = 1";
    query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        QString pass = query.value(2).toString();

        if((userName.compare(name, Qt::CaseInsensitive) == 0) && (passWord.compare(pass, Qt::CaseInsensitive) == 0))
        {
            return 1;
        }
    }
    return 0;
}

bool DUserInfoChecker::checkManagerInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Super_Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Manager_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }

    QString strQuery = "select * from User where Permission = 2";
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString name = query.value(1).toString();
        if(userName.compare(name, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }

    return false;
}

bool DUserInfoChecker::checkEngineerInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    if(userName.compare(managerName[Super_Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    return false;
}

bool DUserInfoChecker::checkSuperService(const QString &userName)
{
    //超级用户，内部使用
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }

    //VWR， 超级用户
    if(userName.compare(managerName[Super_Service_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    return false;
}

bool DUserInfoChecker::checkSuperInfo(const QString &userName)
{
    if(userName.compare(managerName[Super_Name], Qt::CaseInsensitive) == 0)
    {
        return true;
    }
    return false;
}
