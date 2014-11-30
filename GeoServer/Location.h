#ifndef LOCATION_H
#define LOCATION_H

class Location
{
public:
    explicit Location();
    Location(double latitude, double longitude);

    double latitude() const;
    double longitude() const;
};

#endif // LOCATION_H
