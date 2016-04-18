#ifndef POINTSET_HPP
#define POINTSET_HPP

#include "Point.hpp"
#include "vector"

class PointSet
{
public:
    // constructors
    PointSet();
    PointSet(const PointSet& );
    PointSet(int size);

    virtual ~PointSet();

    Point& operator[] (int i);
    Point operator[](int i) const;

    std::vector<Point> getPoints() const;

    unsigned int getSize() const;

    void addPoint(const Point&);

private:
    std::vector<Point> m_points;
};

//---------------------------------------------------------------

inline std::ostream& operator << (std::ostream& in, const PointSet& pS)
{
    for (unsigned int i =0; i < pS.getSize(); ++i)
    {
        in << pS[i] << std::endl;
    }
    return in;
}

//---------------------------------------------------------------


#endif // POINTSET_HPP
