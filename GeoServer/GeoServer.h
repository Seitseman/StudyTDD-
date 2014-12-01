#ifndef GEOSERVER_H
#define GEOSERVER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Location.h"
#include "Area.h"
#include "User.h"

class GeoServer
{
public:
    explicit GeoServer();
    void track(const std::string& user);
    void stopTracking(const std::string& user);
    bool isTracking(const std::string& user) const;

    void updateLocation(const std::string& user, const Location& location);
    Location locationOf(const std::string& user) const;

    std::vector<User> usersInBox(const std::string& user,
                                 double widthInMeters,
                                 double heightInMeters) const;
    bool isDifferentUserInBounds(
            const std::pair<std::string, Location>& each,
            const std::string& user,
            const Area& box) const;


private:
    std::unordered_map<std::string, Location> locations_;
    std::unordered_map<std::string, Location>::const_iterator find(const std::string& user) const;
};

#endif // GEOSERVER_H
