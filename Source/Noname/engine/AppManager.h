#ifndef ____APPMANAGER_H
#define ____APPMANAGER_H

#include "TcpClient.h"

namespace app
{

class AppManager
{
public:
    AppManager();
    ~AppManager();
    void init();
};
extern int  run();
extern void onUpdate();

extern net::TcpClient* __TcpClient;
}  // namespace app

#endif
