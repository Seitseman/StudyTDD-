#include "Location.h"

#include <ostream>

using namespace std;

ostream& operator <<(ostream& output, const Location& location)
{
    output <<"(" << location.latitude() << "," << location.longitude() << ")";
    return output;
}

Location::Location()
{
}

Location::Location(double latitude, double longitude)
    : latitude_(latitude)
    , longitude_(longitude)
{
}


double Location::latitude() const
{
    return latitude_;
}

double Location::longitude() const
{
    return longitude_;
}

bool Location::operator==(const Location& other)
{
    return longitude_ == other.longitude() &&
            latitude_ == other.latitude();
}

bool Location::operator !=(const Location& other)
{
    return !(*this == other);
}

Location Location::go(double meters, double bearing) const
{
    bearing = toRadians(bearing);
    double distance {meters / RadiusOfEarthInMeters };
    double newLat { asin(sin(latitudeAsRadians()) * cos(distance) +
                         cos(latitudeAsRadians()) * sin(distance) *
                         cos(bearing)) };

    double newLong = longitudeAsRadians();
    if (cos(latitudeAsRadians()) != 0)
    {
        newLong = fmod(longitudeAsRadians() -
                       asin(sin(bearing) * sin(distance) / cos(newLat)) +
                       Pi, 2 * Pi) -Pi;
    }

    return Location(toCoordinate(newLat), toCoordinate(newLong));
}

bool Location::isUnknown() const
{
    return latitude_ == std::numeric_limits<double>::infinity() ||
            longitude_ == std::numeric_limits<double>::infinity();
}

double Location::distanceInMeters(const Location& there) const
{
    return RadiusOfEarthInMeters * haversineDistance(there);
}

bool Location::isVeryCloseTo(const Location& other) const
{
    return distanceInMeters(other) <= CloseMeters;
}

double Location::haversineDistance(const Location& there) const
{
    double deltaLongitude{longitudeAsRadians() - there.longitudeAsRadians()};
    double deltaLatitude{latitudeAsRadians() - there.latitudeAsRadians()};

    double aHaversine { pow(sin(deltaLatitude / 2.0), 2.0) +
                cos(latitudeAsRadians()) * cos(there.latitudeAsRadians()) *
                      pow(sin(deltaLongitude / 2),
                          2) };

    return 2 * atan2(sqrt(aHaversine), sqrt(1.0 - aHaversine));
}
