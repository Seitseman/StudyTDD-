#include "GeoServer.h"
#include "Location.h"


using namespace std;

GeoServer::GeoServer()
{
}


void GeoServer::track(const std::string &user)
{
    users_.push_back(user);
}

bool GeoServer::isTracking(const std::string &user) const
{
    bool found{false};
    for (const string & curUser : users_)
    {
        if (curUser == user)
        {
            found = true;
            break;
        }
    }

    return found;
}

void GeoServer::updateLocation(const string& user, const Location& location)
{
    // TODO:
}

Location GeoServer::locationOf(const string &user) const
{
    return Location();
}
