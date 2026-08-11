//
// Created by sw_303 on 2026-08-08.
//

#include "CurlClient.h"
#include "curl_http.h"

void CurlClient::renew_access_code()
{
    nlohmann::json object;
    object["grant_type"] = "client_credentials";
    object["appkey"] = "ZRK-dYuSHW-0NIgqmZsxmqGNxrxrULstTOmgnfyq8f0";
    object["secretkey"] = "8ORyOtPtCvlzl4UEpsqiQodNZpwcpRJi-K8EVaslvVY";
    access_code = nlohmann::json::parse(curl_http::http_post_request("https://api.kiwoom.com/oauth2/token", {}, object))["token"];
}

price_info CurlClient::getCurrentPrice(const std::string& stockcode)
{
    std::string authorization = "authorization: Bearer " + access_code, api_id = "api-id: ka10095";;
    nlohmann::json object;
    object["stk_cd"] = stockcode;
    std::string response = curl_http::http_post_request("https://api.kiwoom.com/api/dostk/stkinfo", {authorization.c_str(), "cont-yn: N", "next-key: ", api_id.c_str()}, object);
    price_info priceinfo;
    price_info::from_json(nlohmann::json::parse(response), priceinfo);
    return priceinfo;
}
