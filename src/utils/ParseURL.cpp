#include <iostream>
#include <utils/ParseURL.h>

/* this is based on Nginx URL parser implementation */

static uint32_t  usual[] = {
    0xffffdbfe, /* 1111 1111 1111 1111  1101 1011 1111 1110 */

    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
    0x7fff37d6, /* 0111 1111 1111 1111  0011 0111 1101 0110 */

    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
    0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
    0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

    0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
};


int ParseURL::parse(){
    enum {
        sw_start = 0,
        sw_after_slash_in_uri,
        sw_check_uri,
        sw_uri
                                                
    } state;

    state = sw_start;

    if (url_.size() == 1 && url_[0] == '/'){
        path_ = url_;
        return 1;
    }

    u_char ch;

    for (int pos=0; pos<url_.size(); pos++){
        ch = url_[pos];
        
        switch(state){
        case sw_start:
            if (ch!='/'){
                return 0;
            }
            state = sw_after_slash_in_uri;
            break;
    
        /* check "/.", "//", and "%" in URI */
        case sw_after_slash_in_uri:
            if (usual[ch >> 5] & (1U << (ch & 0x1f))){ // ch is a ASCII char 
                state = sw_check_uri;
                break;
            }
        case sw_check_uri:
            if (usual[ch >> 5] & (1U << (ch & 0x1f))) {
                break;                            
            }

            switch(ch){
            case '?':
                args_start_ = pos+1; // param begin 
                state = sw_uri;
                break;
            }
            break;
        case sw_uri:
            if (usual[ch >> 5] & (1U << (ch & 0x1f))) {
                break;                            
            }
        }
        
    }
    return 1;
}

std::string ParseURL::path(){
    if (args_start_ > 0)
        path_ = url_.substr(0, args_start_-1);
    return path_;
}

std::string ParseURL::query(){
    query_ = url_.substr(args_start_);
    return query_;
}
