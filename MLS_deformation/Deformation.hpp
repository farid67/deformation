#ifndef DEFORMATION_HPP
#define DEFORMATION_HPP

#include "PointSet.hpp"

class Deformation
{
public:
    Deformation();
    Deformation(const PointSet& startSet, const PointSet& endSet);

    virtual ~Deformation();

    // set/get
    // unnecessary
    PointSet& startMarkers();
    PointSet startMarkers() const;

    Point AffineDeformation(const Point&);
    Point SimilarityDeformation(const Point&);
    Point RigidDeformation(const Point&);

    PointSet AffineDeformationSet (const PointSet&);
    PointSet SimilarityDeformationSet (const PointSet&);
    PointSet RigidDeformationSet (const PointSet&);

private:

    // Same for all points
    PointSet m_startMarkers;
    PointSet m_endMarkers;


    // ensemble de points = différence de chaque point avec le centroïd
    PointSet m_diffStartCentroid;
    PointSet m_diffEndCentroid;

    // --- Similarity PointSet
    PointSet m_diffStartCentroidSimilarity;
    PointSet m_diffEndCentroidSimilarity;

    // le vector de point se calcul à partir d'un point de base
    std::vector<double> m_weights;
    void computeWeights(const Point &);

    // le centroid est calculé relativement à un point de base
    Point m_startCentroid;
    Point m_endCentroid;

    // compute functions
    void computeCentroids(const Point&);

    void computeCentroidsDiff (const Point&);

    void computeCentroidsDiffSimilarity (const Point&);

    glm::mat2 MatrixAffine(const Point&);

    glm::mat2 MatrixSimilarity (const Point&);

    glm::mat2 MatrixRigid (const Point&);
};


//---------------------------------------------------------------

//inline glm::mat2 vec2Xvec2(const glm::vec2& v1, const glm::vec2& v2 )
//{
//    glm::mat2 res(0.);
//    res[0][0] = v1[0] * v2[0];
//    res[0][1] = v1[0] * v2[1];
//    res[1][0] = v1[1] * v2[0];
//    res[1][1] = v1[1] * v2[1];
//    return res;
//}

//---------------------------------------------------------------

#endif // DEFORMATION_HPP
