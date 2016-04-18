#include "Point.hpp"

//---------------------------------------------------------------

Point::Point():
    m_coord(0.0)
{
}

//---------------------------------------------------------------

Point::Point(const Point &p):
    m_coord(p.getCoord())
{
}

//---------------------------------------------------------------

Point::Point(double x, double y)
{
    m_coord[0] = x;
    m_coord[1] = y;
}

//---------------------------------------------------------------

Point::~Point()
{
}

//---------------------------------------------------------------

glm::vec2 Point::getCoord() const
{
    return m_coord;
}

//---------------------------------------------------------------

glm::vec2& Point::getCoord()
{
    return m_coord;
}

//---------------------------------------------------------------

void Point::setX(double x)
{
    m_coord[0] = x;
}

//---------------------------------------------------------------

void Point::setY(double y)
{
    m_coord[1] = y;
}

//---------------------------------------------------------------

double Point::distance(const Point &p) const
{
    return glm::distance(m_coord,p.getCoord());
}

//---------------------------------------------------------------

double Point::weight(const Point &currentPoint, int alpha = 1) const
{
    double Dist;
    Dist = currentPoint.distance(*this);
    return 1.0/pow(Dist,2*alpha);
}


//---------------------------------------------------------------

void Point::invertT()
{
    double y = m_coord[0];
    m_coord[0] = m_coord[1];
    m_coord[1] = -y;
}

//---------------------------------------------------------------
