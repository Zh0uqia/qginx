#include <iostream>
#include <Dispatcher.h>

void Dispatcher::dispatch(Request r){
    Controller ctl;

    std::string cmd = r.getMethod();
    
    if (cmd == "GET"){
        ctl.cmd_get(r);
    }

}
