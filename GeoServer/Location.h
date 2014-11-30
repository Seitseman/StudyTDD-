#ifndef LOCATION_H
#define LOCATION_H

#include <limits>
#include <cmath>
#include <ostream>

const double Pi{4.0 * atan(1.0)};
const double RadiusOfEarthInMeters{6372000};
const double ToRadiansConversionFactor{ Pi/180 };
const double MetersPerDegreeAtEquator{111111};


const double North{0};
const double West{90};
const double South{180};
const double East{270};

const double CloseMeters{3};

class Location
{
public:
    explicit Location();
    Location(double latitude, double longitude);

    inline double toRadians(double degrees) const {
        return degrees * ToRadiansConversionFactor;
    }

    inline double toCoordinate(double radians) const {
        return radians * (180 / Pi);
    }

    inline double latitudeAsRadians() const {
        return toRadians(latitude_);
    }

    inline double longitudeAsRadians() const {
        return toRadians(longitude_);
    }

    double latitude() const;
    double longitude() const;

    bool operator ==(const Location& other);
    bool operator!=(const Location& other);

    Location go(double meters, double bearing) const;
    bool isUnknown() const;
    double distanceInMeters(const Location& there) const;
    bool isVeryCloseTo(const Location& other) const;


private:
    double latitude_{std::numeric_limits<double>::infinity()};
    double longitude_{std::numeric_limits<double>::infinity()};

    double haversineDistance(const Location& there) const;

};

std::ostream& operator<<(std::ostream& output, const Location& location);

#endif // LOCATION_H
