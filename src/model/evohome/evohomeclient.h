//
// evohomeclient.h is part of EvoTherm.
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

#ifndef EVOTHERM_EVOHOMECLIENT_H
#define EVOTHERM_EVOHOMECLIENT_H

#include <string>
#include <vector>
#include "../config.h"
#include "data/userinfo.h"
#include "data/installationinfo.h"

class EvohomeClient {

public:

    /**
     * Initializes the Evohome client.
     *
     * @param config the EvoTherm config.
     * @return the Evohome client.
     */
    EvohomeClient(const Config &config);

    /**
     * @return the installation info for this account.
     */
    const InstallationInfo &getInstallationInfo() const;

    /**
     * Checks whether the given zone is available.
     *
     * @param zone the zone to look for.
     * @return true if the zone is available, false otherwise.
     */
    const bool hasZone(const std::string &zone) const;

    /**
     * @return the zone.
     */
    const Zone &getZoneByName(const std::string &zone) const;

    /**
     * Sets the target temperature for the given zone until the given time.
     *
     * @param zone the zone to set the target temperature for.
     * @param temperature the target temperature.
     * @param until until the given time, leave blank to make permanent.
     */
    void setTargetTemperature(const Zone &zone, const std::string &temperature, const std::string &until);

    /**
     * Cancels an ongoing override.
     *
     * @param zone the zone to cancel the override for.
     */
    void cancelOverride(const Zone &zone);

    /**
     * Sets the thermostat mode.
     *
     * @param mode the mode to set.
     * @param until until the given date, leave blank to make permanent.
     */
    void setMode(const std::string &mode, const std::string &until);

private:

    Config config;
    std::string accessToken;
    std::vector<std::string> applicationHeader;

    UserInfo userInfo;
    InstallationInfo installationInfo;

    void login();
    void fetchUserAccount();
    void fetchInstallationInfo();
    void fetchTemperatures();

    void setZoneTargetTemp(const Zone &zone, const std::string &data);
};


#endif //EVOTHERM_EVOHOMECLIENT_H
