#include <iostream>
#include <archived/Dispatcher.h>

std::string Dispatcher::dispatch(RequestHandler rhandl){
    Controller ctl;

    MethodState cmd = rhandl.getMethod();

    if (cmd == METHOD_GET_STATIC || cmd == METHOD_GET_DYNAMIC || cmd == METHOD_OTHER){
        return ctl.cmdGet(rhandl);
    }

    else if (cmd == METHOD_POST){
        return ctl.cmdPost(rhandl);
    }else
        return "error happened in dispatcher";
}
