//
// Created by sw_303 on 2026-08-08.
//

#include "CurlClient.h"

#include <iostream>

#include "curl_http.h"
#include <regex>

void CurlClient::renew_access_code()
{
    nlohmann::json object;
    object["grant_type"] = "client_credentials";
    object["appkey"] = "ZRK-dYuSHW-0NIgqmZsxmqGNxrxrULstTOmgnfyq8f0";
    object["secretkey"] = "8ORyOtPtCvlzl4UEpsqiQodNZpwcpRJi-K8EVaslvVY";
    access_code = nlohmann::json::parse(curl_http::http_post_request("https://api.kiwoom.com/oauth2/token", {}, object))["token"];
}

price_info CurlClient::getCurrentPrice(const std::string& stockcode, const std::string& stextp)
{
    std::string api_id = "", uri = "https://api.kiwoom.com/api/dostk/stkinfo", type="dom";
    nlohmann::json object;
    if (std::regex_match(stockcode, std::regex(R"(^[0-9]{6}$)"))) api_id = "api-id: ka10095";
    else
    {
        api_id = "api-id: usa20100";
        object["stex_tp"] = stextp;
        uri = "https://api.kiwoom.com/api/us/mrkcond";
        type = "int";
    }
    std::string authorization = "authorization: Bearer " + access_code;

    object["stk_cd"] = stockcode;
    std::string response = curl_http::http_post_request(uri.c_str(), {authorization.c_str(), "cont-yn: N", "next-key: ", api_id.c_str()}, object);
    price_info priceinfo;
    nlohmann::json jsonobject = nlohmann::json::parse(response);
    price_info::from_json(type == "dom" ? jsonobject["atn_stk_infr"][0] : jsonobject, priceinfo, type);
    return priceinfo;
}
