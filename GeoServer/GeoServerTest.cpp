#include "gmock/gmock.h"

#include "GeoServer.h"
#include "VectorUtil.h"
#include "User.h"

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

TEST_F(AGeoServer, UpdatesLocationOfUser)
{
    server.track(AGeoServer::aUser);
    server.updateLocation(AGeoServer::aUser, Location{38, -104});

    auto location = server.locationOf(AGeoServer::aUser);
    ASSERT_NEAR(38, location.latitude(), AGeoServer::LocationTolerance);
    ASSERT_NEAR(-104, location.longitude(), AGeoServer::LocationTolerance);
}

TEST_F(AGeoServer, AnswersUnknownLoactionForUserNotTracked)
{
    ASSERT_TRUE(server.locationOf("anAbUser").isUnknown());
}

TEST_F(AGeoServer, AnswersUnknownLocationForUserNoLongerTracked)
{
    server.track(AGeoServer::aUser);
    server.updateLocation(AGeoServer::aUser, Location(40, 100));
    server.stopTracking(AGeoServer::aUser);
    ASSERT_TRUE(server.locationOf(AGeoServer::aUser).isUnknown());
}


class AGeoServer_UsersInBox : public Test
{
public:
    GeoServer server;
    const double TenMeters {10};
    const double Width {2000 + TenMeters};
    const double Height {4000 + TenMeters};
    const string aUser {"auser"};
    const string bUser {"buser"};
    const string cUser {"cuser"};

    Location aUserLocation {38, -103};

    void SetUp() override {
        server.track(aUser);
        server.track(bUser);
        server.track(cUser);
        server.updateLocation(aUser, aUserLocation);

    }

    vector<string> UserNames(const vector<User>& users) {
        return Collect<User, string>(users, [](User each){return each.name();});
    }
};


TEST_F(AGeoServer_UsersInBox, AnswersUsersInSpecifiedRange)
{
    server.updateLocation(bUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)});

    auto users = server.usersInBox(aUser, Width, Height);

    ASSERT_EQ(vector<string>{bUser}, UserNames(users));
}

TEST_F(AGeoServer_UsersInBox, AnswersOnlyUsersWithinSpecifiedRange)
{
    server.updateLocation(bUser,
                          Location{aUserLocation.go(Width / 2 + TenMeters, East)});
    server.updateLocation(cUser,
                          Location{aUserLocation.go(Width/2 - TenMeters, East)});

    auto users = server.usersInBox(aUser, Width, Height);

    ASSERT_EQ(vector<string>{cUser}, UserNames(users));
}
