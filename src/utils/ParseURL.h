#pragma once

class ParseURL
{
public:
    ParseURL(std::string url) {
        init(url);
    }
    
    void init(std::string url){
        url_ = url;
        parse();
    }

    std::string path();
    std::string query();

private:
    int parse();

    std::string url_;
    std::string path_;
    std::string query_;

    int args_start_{0};
};

