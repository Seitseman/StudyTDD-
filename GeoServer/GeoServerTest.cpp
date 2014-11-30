#include "gmock/gmock.h"

#include "GeoServer.h"

using namespace std;
using namespace ::testing;

class AGeoServer : public Test
{
public:
    AGeoServer() {}

    static const string aUser;
    static const double LocationTolerance;

    GeoServer server;
};

const string AGeoServer::aUser{"auser"};
const double AGeoServer::LocationTolerance{0.005};

TEST_F(AGeoServer, TracksAUser) {
    server.track(AGeoServer::aUser);

    ASSERT_TRUE(server.isTracking(AGeoServer::aUser));
}

TEST_F(AGeoServer, IsNotTrackingAUserNotTracked) {
    ASSERT_FALSE(server.isTracking(AGeoServer::aUser));
}

TEST_F(AGeoServer, TracksMultipleUsers) {
    string anotherUser{"anotheruser"};
    server.track(AGeoServer::aUser);
    server.track(anotherUser);

    ASSERT_FALSE(server.isTracking("thirduser"));
    ASSERT_TRUE(server.isTracking(AGeoServer::aUser));
    ASSERT_TRUE(server.isTracking(anotherUser));
}

TEST_F(AGeoServer, DISABLED_UpdatesLocationOfUser)
{
    server.track(AGeoServer::aUser);
    server.updateLocation(AGeoServer::aUser, Location{38, -104});

    auto location = server.locationOf(AGeoServer::aUser);
    ASSERT_NEAR(38, location.latitude(), AGeoServer::LocationTolerance);
    ASSERT_NEAR(-104, location.longitude(), AGeoServer::LocationTolerance);
}
