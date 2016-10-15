//
// evohomeclient.cpp is part of EvoTherm.
// 
// Copyright (c) 2016 Gianni Van Hoecke <gianni.vh@gmail.com>
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// User: Gianni Van Hoecke <gianni.vh@gmail.com>
// Date: 12/10/16
// Time: 17:58
//

#include <iostream>
#include <map>
#include "evohomeclient.h"
#include "../io/httpclient.h"
#include "../../ext/jsoncpp/json/json.h"
#include "../exitcode.h"
#include "../markup.h"

EvohomeClient::EvohomeClient(const Config &config)
        : config(config) {

    this->login();
    this->getUserAccount();
    this->getInstallationInfo();
}

void EvohomeClient::login() {

    // do login
    std::string url = "https://tccna.honeywell.com/Auth/OAuth/Token";

    std::vector<std::string> headerList;
    headerList.push_back("Authorization: Basic YjAxM2FhMjYtOTcyNC00ZGJkLTg4OTctMDQ4YjlhYWRhMjQ5OnRlc3Q=");
    headerList.push_back("Accept: application/json, application/xml, text/json, text/x-json, text/javascript, text/xml");

    std::map<std::string, std::string> dataMap;
    dataMap.insert(std::make_pair("Content-Type", "application/x-www-form-urlencoded; charset=utf-8"));
    dataMap.insert(std::make_pair("Host", "tccna.honeywell.com/"));
    dataMap.insert(std::make_pair("Cache-Control", "no-store no-cache"));
    dataMap.insert(std::make_pair("Pragma", "no-cache"));
    dataMap.insert(std::make_pair("grant_type", "password"));
    dataMap.insert(std::make_pair("scope", "EMEA-V1-Basic EMEA-V1-Anonymous EMEA-V1-Get-Current-User-Account"));
    dataMap.insert(std::make_pair("Username", this->config.getUsername()));
    dataMap.insert(std::make_pair("Password", this->config.getPassword()));
    dataMap.insert(std::make_pair("Connection", "Keep-Alive"));

    std::string content = HttpClient::post(url, headerList, dataMap);

    // parse access token
    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(content, obj) || obj["access_token"].isNull()) {

        std::cerr << "Could not log in. Please double check your credentials." << std::endl;
        exit(EXIT_LOGIN_FAILED);
    }

    this->accessToken = obj["access_token"].asString();

    // set post login header
    this->applicationHeader.clear();
    this->applicationHeader.push_back("Authorization: bearer " + this->accessToken);
    this->applicationHeader.push_back("applicationId: b013aa26-9724-4dbd-8897-048b9aada249");
    this->applicationHeader.push_back("Accept: application/json, application/xml, text/json, text/x-json, text/javascript, text/xml");
}

void EvohomeClient::getUserAccount() {

    // get account info
    std::string url = "https://tccna.honeywell.com/WebAPI/emea/api/v1/userAccount";
    std::string content = HttpClient::request(url, this->applicationHeader);

    // parse response
    this->userInfo.parse(content);
}

void EvohomeClient::getInstallationInfo() {

    // get installation info
    std::string url = "https://tccna.honeywell.com/WebAPI/emea/api/v1/location/installationInfo?userId=" +
            this->userInfo.userId + "&includeTemperatureControlSystems=True";
    std::string content = HttpClient::request(url, this->applicationHeader);

    // parse response
    this->installationInfo.parseInstallationInfo(content);

    // get temperatures
    url = "https://tccna.honeywell.com/WebAPI/emea/api/v1/location/" + this->installationInfo.locationId +
            "/status?includeTemperatureControlSystems=True";
    content = HttpClient::request(url, this->applicationHeader);

    // parse response
    this->installationInfo.parseTemperature(content);
}

const std::vector<Zone> &EvohomeClient::getAllZones() const {

    return this->installationInfo.zones;
}

const bool EvohomeClient::hasZone(const std::string &zone) const {

    for (int i = 0; i < this->installationInfo.zones.size(); i++) {

        if (this->installationInfo.zones[i].name == zone) {

            return true;
        }
    }

    return false;
}

const Zone &EvohomeClient::getZoneByName(const std::string &zone) const {

    for (int i = 0; i < this->installationInfo.zones.size(); i++) {

        if (this->installationInfo.zones[i].name == zone) {

            return this->installationInfo.zones[i];
        }
    }

    throw std::runtime_error("Zone not found.");
}

void EvohomeClient::setTargetTemperature(const Zone &zone, const std::string &temperature, const std::string &until) const {

    // data
    std::string data = "";
    if (until == "") {

        // set permanently
        data = "{\"HeatSetpointValue\":" + temperature + ",\"SetpointMode\":1,\"TimeUntil\":\"\"}";
    }
    else {

        // set temporarily
        data = "{\"HeatSetpointValue\":" + temperature + ",\"SetpointMode\":2,\"TimeUntil\":\"" + until + "\"}";
    }

    // make request
    setZoneTargetTemp(zone, data);

    // print success
    std::cout << GREEN_LIGHT
              << "Target temperature for " << zone.name << " set to " << temperature << " °C"
              << ((until == "")? "": " until ") << until << "."
              << RESET
              << std::endl;
}

void EvohomeClient::cancelOverride(const Zone &zone) const {

    // data
    std::string data = "{\"HeatSetpointValue\":0.0,\"SetpointMode\":0,\"TimeUntil\":\"\"}";

    // make request
    setZoneTargetTemp(zone, data);

    // print success
    std::cout << GREEN_LIGHT
              << "Target temperature override for " << zone.name << " cancelled."
              << RESET
              << std::endl;
}

void EvohomeClient::setZoneTargetTemp(const Zone &zone, const std::string data) const {

    // url
    std::string url = "https://tccna.honeywell.com/WebAPI/emea/api/v1/temperatureZone/" + zone.zoneId + "/heatSetpoint";

    // headers
    std::vector<std::string> headers(this->applicationHeader);
    headers.push_back("Content-Type: application/json");

    // send
    std::string content = HttpClient::put(url, headers, data);

    // parse result (we shoud get {"id": "xxxxxxx"})
    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(content, obj) || obj["id"].isNull()) {

        std::cerr << "Could not set target temperature. Please try again." << std::endl;
        exit(EXIT_TEMP_SET_FAILED);
    }
}
