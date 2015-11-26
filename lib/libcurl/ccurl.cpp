#include <stdio.h>
#include "ccurl.h"

#include <iostream>
using namespace std;

Curl::Curl() : curl_(NULL)
{
}

Curl::~Curl()
{
    if (NULL != curl_) {
        curl_easy_cleanup(curl_);
        curl_ = NULL;
    }
}

void Curl::init()
{
    if (NULL == curl_) {
        curl_ = curl_easy_init();
    }
}

void Curl::set_url(const char *url)
{
    url_ = url;
}

void Curl::set_curl_opt()
{
    curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_body_cb);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, write_head_cb);
    curl_easy_setopt(curl_, CURLOPT_HEADERDATA, this);
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L); // follow location
    // curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L); // output its progress
    // curl_easy_setopt(&curl_, CURLOPT_TIMEOUT, 60); // set timeout
}

size_t Curl::write_body_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    Curl *cd = static_cast< Curl * >(stream);
    return cd->write_body(ptr, size, nmemb);
}

size_t Curl::write_body(void *ptr, size_t size, size_t nmemb)
{
    const size_t len = size * nmemb;
    resp_body_.append((char *)ptr, len);
    return len;
}

size_t Curl::write_head_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    Curl *cd = static_cast< Curl * >(stream);
    return cd->write_head(ptr, size, nmemb);
}

size_t Curl::write_head(void *ptr, size_t size, size_t nmemb)
{
    const size_t len = size * nmemb;
    resp_head_.append((char *)ptr, len);
    return len;
}

int Curl::get_curl_status_code()
{
    int status_code;
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status_code);
    return status_code;
}

int Curl::get_last_url(char *p)
{
    curl_easy_getinfo(curl_, CURLINFO_EFFECTIVE_URL, &p);
    return 0;
}

int Curl::download()
{
    init();
    set_curl_opt();
    cout << "perform curl ..." << endl;

    CURLcode rc = curl_easy_perform(curl_);
    if (CURLE_OK != rc) {
        return rc;
    }
    cout << "curl success" << endl;
    return 0;
}
