#include "DeformationClean.hpp"
#include <iostream>

//---------------------------------------------------------------

DeformationClean::DeformationClean()
{
}

//---------------------------------------------------------------

DeformationClean::DeformationClean(const std::vector<glm::vec2> &startSet, const std::vector<glm::vec2> &endSet):
    m_startMarkers(startSet),
    m_endMarkers(endSet)
{
}

//---------------------------------------------------------------

DeformationClean::DeformationClean(const DeformationClean &d):
    m_startMarkers(d.startMarkers()),
    m_endMarkers(d.endMarkers())
{

}

//---------------------------------------------------------------

DeformationClean::~DeformationClean()
{
}

//---------------------------------------------------------------

std::vector <glm::vec2>& DeformationClean::startMarkers()
{
    return m_startMarkers;
}

//---------------------------------------------------------------

std::vector <glm::vec2> DeformationClean::startMarkers() const
{
    return m_startMarkers;
}


//---------------------------------------------------------------

std::vector <glm::vec2>& DeformationClean::endMarkers()
{
    return m_endMarkers;
}

//---------------------------------------------------------------

std::vector <glm::vec2> DeformationClean::endMarkers() const
{
    return m_endMarkers;
}

//---------------------------------------------------------------

void DeformationClean::computeWeights(const glm::vec2 &p)
{
    m_weights.clear();
    for (unsigned int i = 0; i < m_startMarkers.size(); ++i)
    {
        m_weights.push_back(weight(p,m_startMarkers[i],1.0));
    }
}

//---------------------------------------------------------------

void DeformationClean::computeCentroids(const glm::vec2 &p)
{
    assert(m_startMarkers.size() == m_endMarkers.size());
//    computeWeights(p);

    // initializations
    double weightSum = 0.0;
    glm::vec2 zero(0.);
    m_startCentroid = zero;
    m_endCentroid = zero;

    m_weights.clear();

    // sum
    for (unsigned int i = 0 ; i < m_startMarkers.size(); ++i)
    {
        m_weights.push_back(weight(p,m_startMarkers[i],1.0));
        m_startCentroid =  m_startCentroid + (glm::vec2(m_weights[i]) * m_startMarkers[i]);
        m_endCentroid = m_endCentroid + (glm::vec2(m_weights[i]) * m_endMarkers[i]);
        weightSum += m_weights[i];
    }
    m_startCentroid = m_startCentroid / glm::vec2(weightSum);
    m_endCentroid = m_endCentroid / glm::vec2(weightSum);
}

//---------------------------------------------------------------

void DeformationClean::computeCentroidsDiff(const glm::vec2 &p)
{
    computeCentroids(p);
    m_diffStartCentroid = m_startMarkers;
    m_diffEndCentroid = m_endMarkers;
    for (unsigned int i = 0; i < m_startMarkers.size() ; ++i)
    {
        m_diffStartCentroid[i] = m_startMarkers[i]-m_startCentroid;
        m_diffEndCentroid[i] = m_endMarkers[i]-m_endCentroid;
    }
}

//---------------------------------------------------------------

glm::mat2 DeformationClean::MatrixAffine(const glm::vec2 & p)
{
//    computeCentroidsDiff(p);

    computeCentroids(p);
    glm::mat2 sumTmp1(0.);
    glm::mat2 sumTmp2(0.);

    m_diffStartCentroid = m_startMarkers;
    m_diffEndCentroid = m_endMarkers;
    glm::vec2 tmp(0.,0.);
    for (unsigned int i = 0 ; i < m_startMarkers.size(); ++i)
    {
        // first compute centroïds Diff
        m_diffStartCentroid[i] = m_startMarkers[i]-m_startCentroid;
        m_diffEndCentroid[i] = m_endMarkers[i]-m_endCentroid;
        // then compute sum
        tmp = glm::vec2(m_weights[i]) * m_diffStartCentroid[i];
        sumTmp1 += vec2Xvec2(m_diffStartCentroid[i],tmp);
        sumTmp2 += vec2Xvec2(tmp,m_diffEndCentroid[i]);
    }


    return glm::inverse(sumTmp1) * sumTmp2;
}

//---------------------------------------------------------------

glm::vec2 DeformationClean::AffineDeformation(const glm::vec2 & p, bool print=false)
{
    glm::mat2 def = MatrixAffine(p);

    glm::vec2 tmp = p - m_startCentroid;
    glm::vec2 res = tmp * def + m_endCentroid;
    tmp = res;

    if (print)
    {
        std::cout << def[0][0] << "," << def[0][1] << std::endl;
        std::cout << def[1][0] << "," << def[1][1] << std::endl;
    }
    return tmp;
}

//---------------------------------------------------------------

glm::vec2 DeformationClean::SimilarityDeformation(const glm::vec2 & p,bool print=false)
{
    glm::mat2 def = MatrixSimilarity(p);

    glm::vec2 tmp = p - m_startCentroid;
    glm::vec2 res = tmp * def + m_endCentroid;
    tmp = res;
    if (print)
    {
        std::cout << def[0][0] << "," << def[0][1] << std::endl;
        std::cout << def[1][0] << "," << def[1][1] << std::endl;
    }
    return tmp;
}

//---------------------------------------------------------------

glm::vec2 DeformationClean::RigidDeformation(const glm::vec2 & p,bool print=false)
{
    glm::mat2 def = MatrixRigid(p);

//    glm::vec2 tmp = ;
    glm::vec2 res = (p - m_startCentroid ) * def + m_endCentroid;
//    tmp = res;
    if (print)
    {
        std::cout << def[0][0] << "," << def[0][1] << std::endl;
        std::cout << def[1][0] << "," << def[1][1] << std::endl;
    }
    return res;
}

//---------------------------------------------------------------

std::vector<glm::vec2> DeformationClean::AffineDeformationSet(const std::vector<glm::vec2> & pS)
{
    unsigned int size = pS.size();
    std::vector<glm::vec2> res (size);
    std::vector<glm::vec2>::iterator begin = m_startMarkers.begin();
    std::vector<glm::vec2>::iterator end = m_startMarkers.end();
    std::vector<glm::vec2>::iterator tmp = m_startMarkers.end();
    int index=0;
    for (unsigned int i = 0; i < size; ++i)
    {
        tmp = std::find(begin,end,pS[i]);
        if (tmp!=end)
        {
            index = tmp - begin;
            res[i] = m_endMarkers[index];
        }
//        else if (i == 1)
//            res[i] = AffineDeformation(pS[i],true);
        else
            res[i] = AffineDeformation(pS[i]);
    }

    return res;
}

//---------------------------------------------------------------

std::vector<glm::vec2> DeformationClean::SimilarityDeformationSet(const std::vector<glm::vec2> & pS)
{
    unsigned int size = pS.size();
    std::vector<glm::vec2> res (size);
    std::vector<glm::vec2>::iterator begin = m_startMarkers.begin();
    std::vector<glm::vec2>::iterator end = m_startMarkers.end();
    std::vector<glm::vec2>::iterator tmp = m_startMarkers.end();
    for (unsigned int i = 0; i < size; ++i)
    {
        tmp = std::find(begin,end,pS[i]);
        if (tmp!=end)
        {
//            std::cout << "here" << std::endl;
            int index = tmp - begin;
            res[i] = m_endMarkers[index];
        }
//        else if (i == 1)
//            res[i] = SimilarityDeformation(pS[i],true);
        else
            res[i] = SimilarityDeformation(pS[i]);
    }
    return res;
}

//---------------------------------------------------------------

std::vector<glm::vec2> DeformationClean::RigidDeformationSet(const std::vector<glm::vec2> & pS)
{
    unsigned int size = pS.size();
    std::vector<glm::vec2> res (size);
    std::vector<glm::vec2>::iterator begin = m_startMarkers.begin();
    std::vector<glm::vec2>::iterator end = m_startMarkers.end();
    std::vector<glm::vec2>::iterator tmp = m_startMarkers.end();
    int index=0;

    std::clock_t start = std::clock();
    double duration;

    for (unsigned int i = 0; i < size; ++i)
    {
        tmp = std::find(begin,end,pS[i]);
        if (tmp!=end)
        {
//            std::cout << "here" << std::endl;
            index = tmp - begin;
            res[i] = m_endMarkers[index];
        }
//        else if (i == 1)
//            res[i] = RigidDeformation(pS[i],true);
        else
            res[i] = RigidDeformation(pS[i]);
    }

    duration = (std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "duration : " << duration << std::endl;

    return res;
}

//---------------------------------------------------------------

void DeformationClean::computeCentroidsDiffSimilarity(const glm::vec2 & p)
{
    computeCentroidsDiff(p);

    m_diffStartCentroidNormal = m_diffStartCentroid;
    m_diffEndCentroidNormal = m_diffEndCentroid;

    // Optim
    // SEE : we can directly apply the - operator on the vec2 because we use -p^i and not p^i
    for (unsigned int i = 0; i < m_diffStartCentroidNormal.size(); ++i)
    {
        m_diffStartCentroidNormal[i] = invertT(m_diffStartCentroidNormal[i]);
        m_diffEndCentroidNormal[i] = invertT(m_diffEndCentroidNormal[i]);
    }
}

//---------------------------------------------------------------

glm::mat2 DeformationClean::MatrixSimilarity(const glm::vec2 & p)
{
    computeCentroidsDiffSimilarity(p);
//    glm::mat2 phi_s(0.);
    double mu_s = 0.;
    glm::mat2 mat2(0.);
    glm::mat2 zero(0.);
    glm::mat2 tmp(0.);
    glm::mat2 tmp2(0.);

    for (unsigned int i = 0; i < m_weights.size(); ++i)
    {
        tmp = zero;
        tmp2 = zero;

        mu_s += m_weights[i] * glm::dot(m_diffStartCentroid[i],m_diffStartCentroid[i]);

        // tmp
        tmp[0][0] = m_weights[i] * m_diffStartCentroid[i][0];
        tmp[1][0] = m_weights[i] * m_diffStartCentroid[i][1];
        tmp[0][1] = m_weights[i] * m_diffStartCentroidNormal[i][0] * -1.;
        tmp[1][1] = m_weights[i] * m_diffStartCentroidNormal[i][1] * -1.;

        tmp2[0] = m_diffEndCentroid[i];
        tmp2[1] = m_diffEndCentroidNormal[i] * glm::vec2(-1.);
        mat2 += tmp * tmp2;
    }

    return mat2/glm::mat2(mu_s);

}

//---------------------------------------------------------------

glm::mat2 DeformationClean::MatrixRigid(const glm::vec2 & p)
{
    computeCentroidsDiffSimilarity(p);
    double mu_r(0.);
    double mu_r1(0.);
    double mu_r2(0.);

    glm::mat2 mat(0.);
    glm::mat2 zero(0.);
    glm::mat2 tmp(0.);
    glm::mat2 tmp2(0.);

    for (unsigned int i = 0; i < m_weights.size(); ++i )
    {
        tmp = zero;
        tmp2 = zero;

        mu_r1 += m_weights[i] * glm::dot(m_diffEndCentroid[i],m_diffStartCentroid[i]);
        mu_r2 += m_weights[i] * glm::dot(m_diffEndCentroid[i],m_diffStartCentroidNormal[i]);


        tmp[0][0] = m_weights[i] * m_diffStartCentroid[i][0];
        tmp[1][0] = m_weights[i] * m_diffStartCentroid[i][1];
        tmp[0][1] = m_weights[i] * m_diffStartCentroidNormal[i][0] * -1.;
        tmp[1][1] = m_weights[i] * m_diffStartCentroidNormal[i][1] * -1.;

        tmp2[0] = m_diffEndCentroid[i];
        tmp2[1] = m_diffEndCentroidNormal[i] * glm::vec2(-1.);

        mat += tmp * tmp2;
    }


    mu_r1 *= mu_r1;
    mu_r2 *= mu_r2;
    mu_r = mu_r1 + mu_r2;
    mu_r = sqrt(mu_r);

    return mat/glm::mat2(mu_r);

}

//---------------------------------------------------------------

