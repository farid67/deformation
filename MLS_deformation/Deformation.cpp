#include "Deformation.hpp"
#include "DeformationClean.hpp"

//---------------------------------------------------------------

Deformation::Deformation()
{
}

//---------------------------------------------------------------

Deformation::Deformation(const PointSet &startSet, const PointSet &endSet):
    m_startMarkers(startSet),
    m_endMarkers(endSet)
{
}

//---------------------------------------------------------------

Deformation::~Deformation()
{
}

//---------------------------------------------------------------

PointSet& Deformation::startMarkers()
{
    return m_startMarkers;
}

//---------------------------------------------------------------

PointSet Deformation::startMarkers() const
{
    return m_startMarkers;
}

//---------------------------------------------------------------

void Deformation::computeWeights(const Point &p)
{
    m_weights.clear();
    for (unsigned int i = 0; i < m_startMarkers.getSize(); ++i)
    {
        m_weights.push_back(p.weight(m_startMarkers[i],1));
    }
}

//---------------------------------------------------------------

void Deformation::computeCentroids(const Point &p)
{
    assert(m_startMarkers.getSize() == m_endMarkers.getSize());
    computeWeights(p);

    // initializations
    double weightSum = 0.0;
    glm::vec2 zero(0.);
    m_startCentroid.getCoord() = zero;
    m_endCentroid.getCoord() = zero;
    // sum
    for (unsigned int i = 0 ; i < m_weights.size(); ++i)
    {
        m_startCentroid =  m_startCentroid + (m_weights.at(i) * m_startMarkers[i]);
        m_endCentroid = m_endCentroid + (m_weights.at(i) * m_endMarkers[i]);
        weightSum += m_weights[i];
    }
    m_startCentroid = m_startCentroid / weightSum;
    m_endCentroid = m_endCentroid / weightSum;
}

//---------------------------------------------------------------

void Deformation::computeCentroidsDiff(const Point &p)
{
    computeCentroids(p);
    m_diffStartCentroid = m_startMarkers;
    m_diffEndCentroid = m_endMarkers;

    for (unsigned int i = 0; i < m_startMarkers.getSize() ; ++i)
    {
        m_diffStartCentroid[i] = m_startMarkers[i]-m_startCentroid;
        m_diffEndCentroid[i] = m_endMarkers[i]-m_endCentroid;
    }
}

//---------------------------------------------------------------

glm::mat2 Deformation::MatrixAffine(const Point & p)
{
    computeCentroidsDiff(p);

    glm::mat2 sumTmp1(0.);
    glm::mat2 sumTmp2(0.);


    Point tmp(0.,0.);
    for (unsigned int i = 0 ; i < m_startMarkers.getSize(); ++i)
    {
        tmp = m_weights[i] * m_diffStartCentroid[i];
        sumTmp1 += vec2Xvec2(m_diffStartCentroid[i].getCoord(),tmp.getCoord());
        sumTmp2 += vec2Xvec2(tmp.getCoord(),m_diffEndCentroid[i].getCoord());
    }


    return glm::inverse(sumTmp1) * sumTmp2;
}

//---------------------------------------------------------------


Point Deformation::AffineDeformation(const Point & p)
{
    glm::mat2 def = MatrixAffine(p);

    Point tmp = p - m_startCentroid;
    glm::vec2 res = tmp.getCoord() * def + m_endCentroid.getCoord();
    tmp.getCoord() = res;
    return tmp;
}


//---------------------------------------------------------------

Point Deformation::SimilarityDeformation(const Point & p)
{
    glm::mat2 def = MatrixSimilarity(p);

    Point tmp = p - m_startCentroid;
    glm::vec2 res = tmp.getCoord() * def + m_endCentroid.getCoord();
    tmp.getCoord() = res;
    return tmp;
}

//---------------------------------------------------------------

Point Deformation::RigidDeformation(const Point & p)
{
    glm::mat2 def = MatrixRigid(p);

    Point tmp = p - m_startCentroid;
    glm::vec2 res = tmp.getCoord() * def + m_endCentroid.getCoord();
    tmp.getCoord() = res;
    return tmp;
}

//---------------------------------------------------------------

PointSet Deformation::AffineDeformationSet(const PointSet & pS)
{
    unsigned int size = pS.getSize();
    PointSet res (size);
    for (unsigned int i = 0; i < size; ++i)
    {
        res[i] = AffineDeformation(pS[i]);
    }

    return res;
}

//---------------------------------------------------------------

PointSet Deformation::SimilarityDeformationSet(const PointSet & pS)
{
    unsigned int size = pS.getSize();
    PointSet res (size);
    for (unsigned int i = 0; i < size; ++i)
    {
        res[i] = SimilarityDeformation(pS[i]);
    }

    return res;
}

//---------------------------------------------------------------

PointSet Deformation::RigidDeformationSet(const PointSet & pS)
{
    unsigned int size = pS.getSize();
    PointSet res (size);
    for (unsigned int i = 0; i < size; ++i)
    {
        res[i] = RigidDeformation(pS[i]);
    }

    return res;
}

//---------------------------------------------------------------

void Deformation::computeCentroidsDiffSimilarity(const Point & p)
{
    computeCentroidsDiff(p);

    m_diffStartCentroidSimilarity = m_diffStartCentroid;
    m_diffEndCentroidSimilarity = m_diffEndCentroid;

    // Optim
    // SEE : we can directly apply the - operator on the vec2 because we use -p^i and not p^i
    for (unsigned int i = 0; i < m_diffStartCentroidSimilarity.getSize(); ++i)
    {
        m_diffStartCentroidSimilarity[i].invertT();
        m_diffEndCentroidSimilarity[i].invertT();
    }
}

//---------------------------------------------------------------

glm::mat2 Deformation::MatrixSimilarity(const Point & p)
{
    computeCentroidsDiffSimilarity(p);
    glm::mat2 phi_s(0.);
    glm::mat2 mat2(0.);
    glm::mat2 zero(0.);
    glm::mat2 tmp(0.);
    glm::mat2 tmp2(0.);

    for (unsigned int i = 0; i < m_weights.size(); ++i)
    {
        tmp = zero;
        tmp2 = zero;

        phi_s += vec2Xvec2(glm::vec2(m_weights[i]) * m_diffStartCentroid[i].getCoord(),m_diffStartCentroid[i].getCoord());

        // tmp
        tmp[0] = m_diffStartCentroid[i].getCoord();
        tmp[1] = m_diffStartCentroidSimilarity[i].getCoord() * glm::vec2(-1.);
        tmp2[0][0] = m_diffEndCentroid[i].getCoord()[0];
        tmp2[1][0] = m_diffEndCentroid[i].getCoord()[1];
        tmp2[0][1] = m_diffEndCentroidSimilarity[i].getCoord()[0] * (-1.);
        tmp2[1][1] = m_diffEndCentroidSimilarity[i].getCoord()[1] * (-1.);

        mat2 += glm::mat2(m_weights[i]) * tmp * tmp2;
    }

    return mat2/phi_s;

}

//---------------------------------------------------------------

glm::mat2 Deformation::MatrixRigid(const Point & p)
{
    computeCentroidsDiffSimilarity(p);
    glm::mat2 phi_r(0.);
    glm::mat2 mat(0.);

    glm::mat2 phi_r1(0.);
    glm::mat2 phi_r2(0.);
    glm::mat2 zero(0.);
    glm::mat2 tmp(0.);
    glm::mat2 tmp2(0.);

    Point pt_tmp;

    for (unsigned int i = 0; i < m_weights.size(); ++i )
    {
        tmp = zero;
        tmp2 = zero;

        pt_tmp = m_weights[i] * m_diffEndCentroid[i];
        phi_r1 += vec2Xvec2(pt_tmp.getCoord(), m_diffStartCentroid[i].getCoord());
        phi_r2 += vec2Xvec2(pt_tmp.getCoord(), m_diffStartCentroidSimilarity[i].getCoord());

        tmp[0] = m_diffStartCentroid[i].getCoord();
        tmp[1] = m_diffStartCentroidSimilarity[i].getCoord() * glm::vec2(-1.);
        tmp2[0][0] = m_diffEndCentroid[i].getCoord()[0];
        tmp2[1][0] = m_diffEndCentroid[i].getCoord()[1];
        tmp2[0][1] = m_diffEndCentroidSimilarity[i].getCoord()[0] * (-1.);
        tmp2[1][1] = m_diffEndCentroidSimilarity[i].getCoord()[1] * (-1.);

        mat += glm::mat2(m_weights[i]) * tmp * tmp2;
    }

    Eigen::Matrix2Xd A (2,2);
    phi_r1 *= phi_r1;
    phi_r2 *= phi_r2;
    phi_r = phi_r1 + phi_r2;

    A << phi_r[0][0] , phi_r[0][1] , phi_r[1][0] , phi_r[1][1] ;
    A.sqrt();

    phi_r[0][0] = A(0,0);
    phi_r[0][1] = A(0,1);
    phi_r[1][0] = A(1,0);
    phi_r[1][1] = A(1,1);

    return mat/phi_r1;

}


//---------------------------------------------------------------



