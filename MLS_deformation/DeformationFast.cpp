#include "DeformationClean.hpp"
#include "DeformationFast.hpp"


//---------------------------------------------------------------

DeformationFast::DeformationFast()
{

}

//---------------------------------------------------------------

DeformationFast::DeformationFast(const std::vector<glm::vec2> &keyPoints, const std::vector<glm::vec2> &gridPoints):
    m_keyPoints(keyPoints),m_gridPoints(gridPoints)
{
    // here we can compute the whole weights
    const unsigned int k_size = keyPoints.size(), gridSize = gridPoints.size();

    m_weights = new double* [k_size];
    for (unsigned int i = 0; i < k_size; ++i)
    {
        m_weights[i] = new double[gridSize];
        for (unsigned int j = 0; j < gridSize; ++j)
        {
            m_weights[i][j] = weight(gridPoints[j],keyPoints[i],1.0);
        }
    }
}

//---------------------------------------------------------------

DeformationFast::~DeformationFast()
{
    unsigned int i;
    for (i = 0; i < m_keyPoints.size(); ++i)
        delete m_weights[i];
    delete m_weights;
}

//---------------------------------------------------------------

std::vector<glm::vec2> DeformationFast::AffineDeformationSet(const std::vector<glm::vec2> &startMarkers, const std::vector<glm::vec2> &endMarkers)
{

    std::vector <glm::vec2> res(m_gridPoints.size());


    std::vector<glm::vec2>::iterator keyPointsBegin    = m_keyPoints.begin();
    std::vector<glm::vec2>::iterator keyPointsEnd = m_keyPoints.end();
    std::vector<glm::vec2>::iterator currentIt;
    std::vector<glm::vec2>::const_iterator currentItMarkers;
    unsigned int index_v, i,index_k;
    double w_i;

    // initializations
    double weightSum = 0.0;
    glm::vec2 zero(0.);
    glm::vec2 w_iVec2(0.);

    glm::mat2 matZero(0.);
    glm::mat2 currentM;
    glm::mat2 sumTmp1;
    glm::mat2 sumTmp2;
    glm::vec2 tmp_wi;

    int index;
    // compute all deformation points


    std::clock_t start;
    double duration;

    for (index_v = 0; index_v < m_gridPoints.size(); ++index_v)
    {
        //TODO -> add test indentity here (if the current point is found in the startMarkers -> res[index_v] = endMarkers[index elemt found]
        if ((currentItMarkers = std::find(startMarkers.begin(), startMarkers.end(), m_gridPoints[index_v])) != startMarkers.end())
        {
            index = currentItMarkers - startMarkers.begin();
            res[index_v] = endMarkers[index];
        }
        else
        {
            m_startCentroid = zero;
            m_endCentroid = zero;
            weightSum = 0.0;

            start = std::clock();
            // compute weighted Centroïds
            m_indexStartMarkers.clear();
            for (i = 0; i < startMarkers.size(); ++i)
            {
                if ((currentIt = std::find(keyPointsBegin, keyPointsEnd, startMarkers[i])) != keyPointsEnd)
                {
                    // get index_k
                    index_k = currentIt - keyPointsBegin;
                    m_indexStartMarkers.push_back(index_k);

                    // get wi
                    w_i = m_weights[index_k][index_v];
                    w_iVec2[0] = w_i;
                    w_iVec2[1] = w_i;

                    m_startCentroid += (w_iVec2 * startMarkers[i]);
                    m_endCentroid += (w_iVec2 * endMarkers[i]);
                    weightSum += w_i;
                }
            }

            duration = (std::clock() - start)/ (double) CLOCKS_PER_SEC;

            std::cout << "first loop " << duration << " seconds" << std::endl;

            // TODO -> test size of m_indexStartMarkers != 0
            m_startCentroid = m_startCentroid / glm::vec2(weightSum);
            m_endCentroid = m_endCentroid / glm::vec2(weightSum);



            // compute M
            start = std::clock();
            sumTmp1 = matZero;
            sumTmp2 = matZero;
            for (i = 0; i < startMarkers.size(); i++)
            {
                m_diffStartCentroid = startMarkers[i] - m_startCentroid;
                m_diffEndCentroid = endMarkers[i] - m_endCentroid;

                w_i = m_weights[m_indexStartMarkers[i]][index_v];
                tmp_wi = glm::vec2(w_i) * m_diffStartCentroid;
                sumTmp1 += vec2Xvec2(m_diffStartCentroid,tmp_wi);
                sumTmp2 += vec2Xvec2(tmp_wi,m_diffEndCentroid);
            }
            duration = (std::clock() - start)/ (double) CLOCKS_PER_SEC;

            std::cout << "second loop " << duration << " seconds" << std::endl;


            currentM = glm::inverse(sumTmp1) * sumTmp2;

            res[index_v] = (m_gridPoints[index_v] - m_startCentroid) * currentM + m_endCentroid;
        }
    }
    return res;
}

//---------------------------------------------------------------
