#ifndef AREA_H
#define AREA_H

#include "Location.h"

class Area
{
public:
    Area(const Location& location, double width, double height);
    Location upperLeft() const;
    Location upperRight() const;
    Location lowerRight() const;
    Location lowerLeft() const;
    bool inBounds(const Location& location) const;

private:
    double left_;
    double right_;
    double top_;
    double bottom_;
};

#endif // AREA_H
