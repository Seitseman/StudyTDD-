#include "gmock/gmock.h"
#include "Location.h"

#include <sstream>

using namespace std;
using namespace ::testing;

class ALocation : public Test
{
public:
    ALocation() {}

    Location location;
    const Location arbitraryLocation{ 38.2, -104.5};
    static const double Tolerance;
};

const double ALocation::Tolerance{0.005};


TEST_F(ALocation, AnswersLattitudeAndLongitude)
{
    Location location{10, 20};

    ASSERT_DOUBLE_EQ(10, location.latitude());
    ASSERT_DOUBLE_EQ(20, location.longitude());
}


TEST_F(ALocation, isNotUnknownWhenLatitudeAndLongitudeProvided)
{
    Location location{1, 1};
    ASSERT_FALSE(location.isUnknown());
}

TEST_F(ALocation, isUnknownWhenLatitudeAndLongitudeNotProvided)
{
    Location location;
    ASSERT_TRUE(location.isUnknown());
}


TEST_F(ALocation, AnswersDistanceFromAnotherInMeters)
{
    Location point1{38.017, -104.84};
    Location point2{38.025, -104.99};

    ASSERT_NEAR(13170.0, point1.distanceInMeters(point2), 5.0);
}

TEST_F(ALocation, IsNotEqualToAnotherWhenLatDiffers)
{
    Location point1{10, 11};
    Location point2{11, 11};

    ASSERT_TRUE(point1 != point2);
}

TEST_F(ALocation, IsEqualToAnotherWhenLongAndLatMatch)
{
    Location point1{10, 11};
    Location point2{10, 11};

    ASSERT_TRUE(point1 == point2);
}

TEST_F(ALocation, AnswersNewLocationGivenDistanceAndBearing)
{
    Location start{0,0};

    auto newLocation = start.go(MetersPerDegreeAtEquator, East);

    Location expectedEnd{0,1};
    ASSERT_NEAR(expectedEnd.longitude(), newLocation.longitude(),
                ALocation::Tolerance);
    ASSERT_NEAR(expectedEnd.latitude(), newLocation.latitude(),
                ALocation::Tolerance);
}

TEST_F(ALocation, AnswersNewLocationGivenDistanceAndBearingVerifiedByHaversine)
{
    double distance{100};
    Location start{38, -78};

    auto end = start.go(distance, 35);

    ASSERT_NEAR(distance, start.distanceInMeters(end), ALocation::Tolerance);
}

TEST_F(ALocation, CanBeAPole)
{
    Location start{90,0};
    auto end = start.go(MetersPerDegreeAtEquator, South);

    ASSERT_NEAR(0, end.longitude(), ALocation::Tolerance);
    ASSERT_NEAR(89, end.latitude(), ALocation::Tolerance);
}

TEST_F(ALocation, IsVeryCloseToAnotherWhenSmallDistanceApart)
{
    Location threeMetersAway{arbitraryLocation.go(3, South)};

    ASSERT_TRUE(arbitraryLocation.isVeryCloseTo(threeMetersAway));
}

TEST_F(ALocation, IsNotVeryCloseToAnotherWhenNotSmallDistanceApart)
{
    Location fourMetersAway{arbitraryLocation.go(4, South) };

    ASSERT_FALSE(arbitraryLocation.isVeryCloseTo(fourMetersAway));
}

TEST_F(ALocation, ProvidesPrintableRepresentation)
{
    Location location{-32, -105};
    stringstream s;
    s << location;

    ASSERT_EQ("(-32,-105)", s.str());
}
