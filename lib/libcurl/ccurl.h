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
    // set download url
    void set_url(const char *url);

    // add request http head
    void add_request_head(const std::string &key, const std::string &value);
    // clear request http head
    void clear_request_head();

    // get response body
    std::string get_body() { return resp_body_; }
    // get response head
    std::string get_head() { return resp_head_; }

    // perfrom curl
    int download();

    // get download info
    // get status code
    int get_curl_status_code();
    // get redirected url
    int get_last_url(char **p);
    // get content-type
    int get_content_type(char **p);

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

	struct curl_slist *head_list_;
};

#endif // INCLUDE_CURL_H
