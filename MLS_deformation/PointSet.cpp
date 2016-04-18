#include "PointSet.hpp"


//---------------------------------------------------------------

PointSet::PointSet()
{
}

//---------------------------------------------------------------

PointSet::PointSet(const PointSet &p):
    m_points(p.getPoints())
{

}

//---------------------------------------------------------------

PointSet::PointSet(int size)
{
    m_points.resize(size);
}

//---------------------------------------------------------------

PointSet::~PointSet()
{
}

//---------------------------------------------------------------

unsigned int PointSet::getSize() const
{
    return m_points.size();
}

//---------------------------------------------------------------

std::vector<Point> PointSet::getPoints() const
{
    return m_points;
}


//---------------------------------------------------------------

Point& PointSet::operator[] (int i)
{
    return m_points[i];
}


//---------------------------------------------------------------

Point PointSet::operator [](int i) const
{
    return m_points[i];
}

//---------------------------------------------------------------

void PointSet::addPoint(const Point &p)
{
    Point pNew (p);
    m_points.push_back(pNew);
}

//---------------------------------------------------------------

//---------------------------------------------------------------

