#ifndef DUSERINFOCHECKER_H
#define DUSERINFOCHECKER_H

#include <QObject>

enum ManagerName
{
    Super_Name,
    Super_Service_Name,
    Service_Name,
    Manager_Name,
    Name_Num
};

const QString userPassword[Name_Num] =
{
    "888888",
    "666666",
    "860860",
    "111111"
};

const QString managerName[Name_Num] =
{
    "Super",
    "Master",
    "Service",
    "Manager"
};


class DUserInfoChecker : public QObject
{
    Q_OBJECT

public:
    explicit DUserInfoChecker(QObject *parent = 0);

    int checkUserInfo(const QString& userName, const QString& passWord);
    bool checkManagerInfo(const QString &userName);
    bool checkEngineerInfo(const QString &userName);
    bool checkSuperService(const QString &userName);
    bool checkSuperInfo(const QString& userName); 
};

#endif // EX_USERINFO_H
