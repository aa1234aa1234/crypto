//
// Created by sw_303 on 2026-08-08.
//

#ifndef CURLCLIENT_H
#define CURLCLIENT_H
#include "ExchangeInfo.h"

class CurlClient {
    std::string access_code;
public:
    CurlClient() {}
    ~CurlClient() {}

    void renew_access_code();
    price_info getCurrentPrice(const std::string& stockcode);
    //add more for buying and selling
};



#endif
