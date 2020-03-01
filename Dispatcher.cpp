#include <iostream>
#include <Dispatcher.h>

void dispatch(Request r){
    Controller ctl;

    std::string cmd = r.getType();
    
    switch (cmd){
    case 'GET':{
                    ctl.cmd_get(r);
                    break;
               }
    case 'POST':{
                    ctl.cmd_post(r);
                    break;
                }
    default:
                break;
}
}
