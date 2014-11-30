#ifndef GEOSERVER_H
#define GEOSERVER_H

#include <string>
#include <vector>

#include "Location.h"

class GeoServer
{
public:
    explicit GeoServer();
    void track(const std::string& user);
    bool isTracking(const std::string& user) const;

    void updateLocation(const std::string& user, const Location& location);
    Location locationOf(const std::string& user) const;


private:
    std::vector<std::string> users_;
};

#endif // GEOSERVER_H
