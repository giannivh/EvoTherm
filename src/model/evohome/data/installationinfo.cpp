//
// installationinfo.cpp is part of EvoTherm.
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
// Date: 13/10/16
// Time: 16:17
//

#include <iostream>
#include "installationinfo.h"
#include "../../../ext/jsoncpp/json/json.h"
#include "../../exitcode.h"
#include "../../util/dateutil.h"
#include "../../exception/evothermexception.h"

void InstallationInfo::parseInstallationInfo(const std::string &jsonData) {

    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(jsonData, obj) || obj[0]["gateways"][0]["temperatureControlSystems"][0]["systemId"].isNull()) {

        throw EvoThermException("Could not get installation data.", EXIT_INSTALL_DATA_ERROR);
    }

    // parse useful data
    this->locationId = obj[0]["locationInfo"]["locationId"].asString();
    this->systemId = obj[0]["gateways"][0]["temperatureControlSystems"][0]["systemId"].asString();
    this->modelType = obj[0]["gateways"][0]["temperatureControlSystems"][0]["modelType"].asString();

    // parse zones
    const Json::Value &zones = obj[0]["gateways"][0]["temperatureControlSystems"][0]["zones"];
    for (int i = 0; i < zones.size(); i++) {

        this->zones.push_back(Zone(zones[i]["zoneId"].asString(), zones[i]["modelType"].asString(),
                                   zones[i]["zoneType"].asString(), zones[i]["name"].asString()));
    }

    // parse modes
    const Json::Value &modes = obj[0]["gateways"][0]["temperatureControlSystems"][0]["allowedSystemModes"];
    for (int i = 0; i < modes.size(); i++) {

        Mode mode = Mode(modes[i]["systemMode"].asString(), modes[i]["canBePermanent"].asBool(),
             modes[i]["canBeTemporary"].asBool());

        if (mode.canBeTemporary) {

            mode.maxDuration = modes[i]["maxDuration"].asString();
            mode.timingResolution = modes[i]["timingResolution"].asString();
            mode.timingMode = modes[i]["timingMode"].asString();
        }

        this->modes.push_back(mode);
    }
}

void InstallationInfo::parseTemperature(const std::string &jsonData) {

    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(jsonData, obj) || obj["gateways"][0]["temperatureControlSystems"][0]["systemId"].isNull()) {

        throw EvoThermException("Could not get temperature data.", EXIT_TEMP_DATA_ERROR);
    }

    // parse zone temperatures
    for (int i = 0; i < zones.size(); i++) {

        Zone &zone = zones[i];

        const Json::Value &temps = obj["gateways"][0]["temperatureControlSystems"][0]["zones"];
        for (int j = 0; j < temps.size(); j++) {

            if (zone.zoneId == temps[i]["zoneId"].asString()) {

                zone.temperature = temps[i]["temperatureStatus"]["temperature"].asDouble();
                zone.targetTemperature = temps[i]["heatSetpointStatus"]["targetTemperature"].asDouble();
                zone.setpointMode = temps[i]["heatSetpointStatus"]["setpointMode"].asString();
                zone.until = temps[i]["heatSetpointStatus"]["until"].asString();

                continue;
            }
        }
    }

    // parse current mode
    currentMode = Mode::toReadable(obj["gateways"][0]["temperatureControlSystems"][0]["systemModeStatus"]["mode"].asString());
    if (!obj["gateways"][0]["temperatureControlSystems"][0]["systemModeStatus"]["isPermanent"].asBool()) {

        currentModeUntil = DateUtil::toLocal(
                obj["gateways"][0]["temperatureControlSystems"][0]["systemModeStatus"]["timeUntil"].asString());
    }
    else {

        // just to make it reset after an update
        currentModeUntil = "";
    }
}
