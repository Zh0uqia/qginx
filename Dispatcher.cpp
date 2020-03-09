#include <iostream>
#include <Dispatcher.h>

char* Dispatcher::dispatch(RequestHandler rhandl){
    Controller ctl;

    std::string cmd = rhandl.getMethod();
    
    if (cmd == STATE_GET_STATIC || cmd == STATE_GET_DYNAMIC){
        return ctl.cmdGet(rhandl);
    }

    else if (cmd == STATE_METHOD_POST){
        return ctl.cmdPost(rhandl);
    }else{
        return ctl.cmdOther(rhandl);
    }

}
