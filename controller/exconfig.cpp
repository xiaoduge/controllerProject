#include "exconfig.h"

#define __DEBUG__  //发布程序时注释掉

// 程序版本号
#ifndef __DEBUG__
#define VERSION "0.2.1" " Built on " __DATE__ " " __TIME__
#else
#define VERSION "0.2.1" " Built on " __DATE__ " " __TIME__ " debug"
#endif

#pragma message(VERSION)

const QString gAppVersion = VERSION;
