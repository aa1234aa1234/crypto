//
// Created by sw_303 on 2026-08-08.
//

#ifndef CURL_HTTP_H
#define CURL_HTTP_H
#include <mutex>
#include <string>
#include <curl/curl.h>
#include <curl/curlver.h>
#include <curl/easy.h>
#include <curl/urlapi.h>

#include "json.hpp"

static void init_curl()
{
    static std::once_flag flag;
    std::call_once(flag, []
    {
        CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (result != CURLE_OK) throw std::runtime_error("curl_global_init() failed");
    });
}

namespace curl_http
{
    inline void clear_curl()
    {
        curl_global_cleanup();
    }

    inline size_t write_buffer_callback(char* contents, size_t size, size_t nmemb, std::string* response)
    {
        size_t count = size * nmemb;
        if (response != nullptr && count > 0)
        {
            response->append(contents, count);
        }
        return count;
    }

    inline std::string http_post_request(const char* url, const std::vector<const char*>& lists, const nlohmann::json& data)
    {
        init_curl();

        CURL* curl = curl_easy_init();
        if (curl == nullptr)
        {
            throw std::runtime_error("curl_easy_init() failed");
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buffer_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


        curl_slist* list = nullptr;
        list = curl_slist_append(list, "Content-Type: application/json;charset=UTF-8");
        for (auto& p : lists) list = curl_slist_append(list, p);

        //std::string authorization = "authorization: Bearer " + token, api_id = "api-id: " + trcode;;
        // list = curl_slist_append(list, authorization.c_str());
        // list = curl_slist_append(list, "cont-yn: N");
        // list = curl_slist_append(list, "next-key: ");
        // list = curl_slist_append(list, api_id.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        auto jsonstr = data.dump(0);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);

        CURLcode err_code = curl_easy_perform(curl);
        if (err_code != CURLE_OK)
        {
            throw std::runtime_error("curl_easy_perform() failed");
        }

        curl_slist_free_all(list);
        curl_easy_cleanup(curl);

        return response;
    }
}

#endif //CURL_HTTP_H
