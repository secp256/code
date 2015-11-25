#include <curl/curl.h>
#include <string>

#ifndef INCLUDE_CURL_H
#define INCLUDE_CURL_H

class Curl
{
public:
    Curl();
    ~Curl();

private:
    void init();
    void set_curl_opt();

public:
    // set
    void set_url(const char *url);

    // get
    std::string get_body() { return resp_body_; }
    std::string get_head() { return resp_head_; }

    int download();
    
    int get_curl_status_code();
    int get_last_url(char *p);

private:
    size_t write_head(void *ptr, size_t size, size_t nmemb);
    size_t write_body(void *ptr, size_t size, size_t nmemb);

    // callback
    static size_t write_body_cb(void *ptr, size_t size, size_t nmemb, void *stream);
    static size_t write_head_cb(void *ptr, size_t size, size_t nmemb, void *stream);

private:
    CURL *curl_;
    std::string url_;
    std::string resp_body_;
    std::string resp_head_;
};

#endif // INCLUDE_CURL_H
