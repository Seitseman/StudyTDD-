#include "GeoServer.h"
#include "Location.h"


using namespace std;

GeoServer::GeoServer()
{
}


void GeoServer::track(const std::string &user)
{
    locations_[user] = Location();
}

void GeoServer::stopTracking(const string& user)
{
    locations_.erase(user);
}

bool GeoServer::isTracking(const std::string &user) const
{
    return find(user) != locations_.end();
}

void GeoServer::updateLocation(const string& user, const Location& location)
{
    locations_[user] = location;
}

Location GeoServer::locationOf(const string &user) const
{
    if (!isTracking(user))
    {
        return Location{};
    }
    return find(user)->second;
}

unordered_map<string, Location>::const_iterator
GeoServer::find(const string& user) const
{
    return locations_.find(user);
}
