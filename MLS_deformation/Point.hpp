#ifndef POINT_HPP
#define POINT_HPP

#include "ostream"
#include "iostream"
#include "glm/glm.hpp"
#include "eigen3/Eigen/Core"
#include "eigen3/unsupported/Eigen/MatrixFunctions"

class Point
{
public:
    // constructor
    Point();
    Point(const Point&);
    Point(double x, double y);
    // destructor
    virtual ~Point();



    // set/get
    glm::vec2& getCoord();
    glm::vec2 getCoord() const;
    void setX(double x);
    void setY(double y);


    // compute functions
    double distance(const Point&) const;

    //  operator _|_
    void invertT ();

    double weight(const Point& currentPoint ,int alpha) const;


private:
    glm::vec2 m_coord;
};

//---------------------------------------------------------------

inline Point operator+ (const Point& p, const Point& p2)
{
    Point pNew(p);
    pNew.getCoord() += p2.getCoord();
    return pNew;
}

//---------------------------------------------------------------

inline Point operator- (const Point& p, const Point& p2)
{
    Point pNew(p);
    pNew.getCoord() -= p2.getCoord();
    return pNew;
}

//---------------------------------------------------------------

inline Point operator* (double val, const Point& p)
{
    Point pNew(p);
    pNew.getCoord() *= val;
    return pNew;
}

//---------------------------------------------------------------

inline Point operator* (const Point& p, double val)
{
    Point pNew(p);
    pNew.getCoord() *= val;
    return pNew;
}

//---------------------------------------------------------------

inline Point operator/ (double val, const Point& p)
{
    Point pNew(p);
    pNew.getCoord() /= val;
    return pNew;
}

//---------------------------------------------------------------

inline Point operator/ (const Point& p, double val)
{
    Point pNew(p);
    pNew.getCoord() /= val;
    return pNew;
}

//---------------------------------------------------------------

inline std::ostream& operator << (std::ostream& in, const Point& p)
{
    in << "(" << p.getCoord()[0] << "," << p.getCoord()[1] << ")";
    return in;
}

//---------------------------------------------------------------

#endif // POINT_HPP
