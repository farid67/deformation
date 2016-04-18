#include <iostream>

#include "QApplication"
#include "QToolBar"

#include "Deformation.hpp"
#include "DeformationWindow.hpp"

int main(int argc, char* argv[])
{

    // Test using Deformation
    /*
    PointSet p;

    p.addPoint (Point (2.0,1.0));
//    p.addPoint (Point (3.0,1.0));
//    p.addPoint (Point (4.0,1.0));
    p.addPoint (Point (5.0,1.0));
//    p.addPoint (Point (2.0,2.0));
//    p.addPoint (Point (2.0,3.0));
    p.addPoint (Point (2.0,4.0));
//    p.addPoint (Point (3.0,4.0));
//    p.addPoint (Point (4.0,4.0));
    p.addPoint (Point (5.0,4.0));
//    p.addPoint (Point (5.0,2.0));
//    p.addPoint (Point (5.0,3.0));


//    p[0] = p0;
//    p[1] = p1;
//    p[2] = p2;
//    p[3] = p3;

    Point q0(2.0,1.0);
    Point q1(5.0,1.0);
    Point q2(3.0,4.0);
    Point q3(6.0,4.0);

    PointSet q(4);
    q[0] = q0;
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;


    PointSet defPoints;
    Point v0(3.0,2.0);
    defPoints.addPoint(v0);
    Point v1(4.0,2.0);
    defPoints.addPoint(v1);
    Point v2(3.0,3.0);
    defPoints.addPoint(v2);
    Point v3(4.0,3.0);
    defPoints.addPoint(v3);
    Point v4(3.0,4.0);
    defPoints.addPoint(v4);
    Point v5(2.0,2.0);
    defPoints.addPoint(v5);
    // test with one of the begin points
    defPoints.addPoint(Point (2.0,1.0));


    Deformation d(p,q);

    //-- Test single points
//    Point v_arr = d.AffineDeformation(v);
//    std::cout << v_arr << std::endl;

    // -- Test set of points
    std::cout << defPoints << std::endl;
    PointSet res = d.AffineDeformationSet(defPoints);
    std::cout << res << std::endl;
    PointSet res2 = d.SimilarityDeformationSet(defPoints);
    std::cout << res2 << std::endl;
    PointSet res3 = d.RigidDeformationSet(defPoints);
    std::cout << res3 << std::endl;

    */


    // Same with DeformationClean
    /*


    std::vector <glm::vec2> p ;
//    for (int i = 2 ; i <= 5; ++i )
//        for (int j = 1; j <= 4; ++j)
//            p.push_back(glm::vec2(i,j));

    p.push_back(glm::vec2(2.0,1.0));
    p.push_back(glm::vec2(5.0,1.0));
    p.push_back(glm::vec2(2.0,4.0));
    p.push_back(glm::vec2(5.0,4.0));
    p.push_back(glm::vec2(3.5,2.5));

//    std::vector <glm::vec2> q;
//    q.push_back(glm::vec2(2.0,1.0));
//    q.push_back(glm::vec2(5.0,1.0));
//    q.push_back(glm::vec2(3.0,4.0));
//    q.push_back(glm::vec2(6.0,4.0));

    std::vector<glm::vec2> q(p);

    std::vector <glm::vec2> defPoints;
//    defPoints.push_back(glm::vec2(3.0,2.0));
//    defPoints.push_back(glm::vec2(4.0,2.0));
//    defPoints.push_back(glm::vec2(3.0,3.0));
//    defPoints.push_back(glm::vec2(4.0,3.0));
    defPoints.push_back(glm::vec2(3.0,4.0));
//    defPoints.push_back(glm::vec2(2.0,2.0));
//    defPoints.push_back(glm::vec2(5.0,2.0));
//    defPoints.push_back(glm::vec2(2.0,1.0));
//    defPoints.push_back(glm::vec2(3.5,2.5));

    DeformationClean d(p,q);

    std::vector <glm::vec2> res3 = d.SimilarityDeformationSet(defPoints);
    printSet(res3);
    */



    // render Window
//    /*

    QGuiApplication app (argc, argv);

    QSurfaceFormat format;

    format.setSamples(16);
    format.setDepthBufferSize(24);

    DeformationWindow win(false);
    win.setFormat(format);
    win.resize(960,540);
    win.show();

    win.setAnimating(true);

    return app.exec();
//    */
//    return 0;
}

