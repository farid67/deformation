#ifndef DEFORMATIONFAST_HPP
#define DEFORMATIONFAST_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <glm/glm.hpp>
#include <ctime>
#include <cstdio>


/*
 *  Try to do the same thing as in Deformation Clean by storing all weights in an array and keep in in a class
 *  but this is not more fast...
 */

class DeformationFast
{
public:
    DeformationFast();
    DeformationFast(const std::vector<glm::vec2> &keyPoints, const std::vector<glm::vec2> &gridPoints);


    virtual ~DeformationFast();


    // set/get

//    std::vector<glm::vec2>& startMarkers();
//    std::vector<glm::vec2> startMarkers() const;

//    std::vector<glm::vec2>& endMarkers();
//    std::vector<glm::vec2> endMarkers() const;

    std::vector<glm::vec2> AffineDeformationSet (const std::vector<glm::vec2>& startMarkers, const std::vector<glm::vec2>& endMarkers);

//    glm::vec2 AffineDeformation(const glm::vec2&, bool print);

private:
    std::vector <glm::vec2> m_keyPoints;
    std::vector <glm::vec2> m_gridPoints;

    double** m_weights;


    std::vector<int> m_indexStartMarkers;
    std::vector<glm::vec2> m_endMarkers;

    // ensemble de glm::vec2s = différence de chaque glm::vec2 avec le centroïd
    glm::vec2 m_diffStartCentroid;
    glm::vec2 m_diffEndCentroid;

    glm::vec2 m_startCentroid;
    glm::vec2 m_endCentroid;




};


#endif // DEFORMATIONFAST_HPP
