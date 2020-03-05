#include <iostream>
#include <Dispatcher.h>

char* Dispatcher::dispatch(Request r){
    Controller ctl;

    std::string cmd = r.getMethod();
    
    if (cmd == "GET"){
        return ctl.cmd_get(r);
    }

    else if (cmd == "POST"){
        return ctl.cmd_post(r);
    }

}
