#ifndef DEFORMATIONCLEAN_HPP
#define DEFORMATIONCLEAN_HPP

#include <ctime>
#include <cstdio>

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <thread>
#include <glm/glm.hpp>
#include <eigen3/unsupported/Eigen/MatrixFunctions>


class DeformationClean
{
public:
    DeformationClean();
    DeformationClean(const std::vector<glm::vec2>& startSet, const std::vector<glm::vec2>& endSet);

    DeformationClean(const DeformationClean& );
    virtual ~DeformationClean();

    // set/get

    std::vector<glm::vec2>& startMarkers();
    std::vector<glm::vec2> startMarkers() const;

    std::vector<glm::vec2>& endMarkers();
    std::vector<glm::vec2> endMarkers() const;



    glm::vec2 AffineDeformation(const glm::vec2&, bool print);
    glm::vec2 SimilarityDeformation(const glm::vec2&, bool print);
    glm::vec2 RigidDeformation(const glm::vec2&, bool print);

    std::vector<glm::vec2> AffineDeformationSet (const std::vector<glm::vec2>&);
    std::vector<glm::vec2> SimilarityDeformationSet (const std::vector<glm::vec2>&);
    std::vector<glm::vec2> RigidDeformationSet (const std::vector<glm::vec2>&);

private:

    // Same for all glm::vec2s
    std::vector<glm::vec2> m_startMarkers;
    std::vector<glm::vec2> m_endMarkers;

    // ensemble de glm::vec2s = différence de chaque glm::vec2 avec le centroïd
    std::vector<glm::vec2> m_diffStartCentroid;
    std::vector<glm::vec2> m_diffEndCentroid;

    // --- Similarity std::vector<glm::vec2>
    std::vector<glm::vec2> m_diffStartCentroidNormal;
    std::vector<glm::vec2> m_diffEndCentroidNormal;

    // le vector de glm::vec2 se calcul à partir d'un glm::vec2 de base
    std::vector<double> m_weights;
    void computeWeights(const glm::vec2 &);

    // le centroid est calculé relativement à un glm::vec2 de base
    glm::vec2 m_startCentroid;
    glm::vec2 m_endCentroid;


    // compute functions
    void computeCentroids(const glm::vec2&);

    void computeCentroidsDiff (const glm::vec2&);

    void computeCentroidsDiffSimilarity (const glm::vec2&);

    glm::mat2 MatrixAffine(const glm::vec2&);

    glm::mat2 MatrixSimilarity (const glm::vec2&);

    glm::mat2 MatrixRigid (const glm::vec2&);

};

//---------------------------------------------------------------

inline double weight(const glm::vec2& p,const glm::vec2& p2 ,double alpha=1.0 )
{
    double Dist;
    Dist = glm::distance(p,p2);
    return 1.0/pow(Dist,2.0*alpha);
}

//---------------------------------------------------------------


inline glm::vec2 invertT (const glm::vec2& p)
{
    glm::vec2 pNew;
    pNew[0] = -p[1];
    pNew[1] = p[0];
    return pNew;
}

//---------------------------------------------------------------

inline glm::mat2 vec2Xvec2(const glm::vec2& v1, const glm::vec2& v2 )
{
    glm::mat2 res(0.);
    res[0][0] = v1[0] * v2[0];
    res[0][1] = v1[0] * v2[1];
    res[1][0] = v1[1] * v2[0];
    res[1][1] = v1[1] * v2[1];
    return res;
}

//---------------------------------------------------------------

inline void printSet(const std::vector<glm::vec2> & pS)
{
    for (unsigned int i = 0; i < pS.size(); i++)
        std::cout << "(" << pS[i][0] << "," << pS[i][1] << ")" << std::endl;
    std::cout << std::endl;
}

//---------------------------------------------------------------

inline std::vector<DeformationClean> getDeformationSetFromFile(std::string fileName)
{
    std::ifstream file;
    file.open(fileName.c_str(),std::fstream::in);

    std::string line;

    std::vector < DeformationClean > defSets ;
    glm::vec2 a,b;
    float distance_th = 200.;
    std::vector<glm::vec2> vecA,vecB;
    while (getline(file,line))
    {
        vecA.clear();
        vecB.clear();
        int size = std::stoi(line);
        vecA.reserve(size);
        vecB.reserve(size);
        for (int i = 0; i < size; ++i)
        {
            getline(file,line);
            std::istringstream lineStream(line);
            std::string current_int_string;
            // PARSE LINE
            std::getline(lineStream,current_int_string,' ');
            a[0] = std::stoi(current_int_string);
            std::getline(lineStream,current_int_string,' ');
            a[1]= std::stoi(current_int_string);
            std::getline(lineStream,current_int_string,' ');
            b[0] = std::stoi(current_int_string);
            std::getline(lineStream,current_int_string,' ');
            b[1] = std::stoi(current_int_string);
            if (glm::distance(a,b)>distance_th)
                std::cout << "distance seems to be too high between (" << a[0] << "," << a[1] << ") and (" << b[0] << "," << b[1] << ")" << std::endl;
            else
            {
                // PUSH ELEMENTS iN ARRAYs
                vecA.push_back(a);
                vecB.push_back(b);
            }
        }
        // GET NEWLINE
        getline(file,line);
        if (vecA.size() < 5 )
            std::cout << "Can not add this set, not enough points" << std::endl;
        else
            defSets.push_back(DeformationClean(vecA,vecB));
    }

    file.close();
    return defSets;

}

//---------------------------------------------------------------

inline std::vector<glm::vec2> getStartMarkers(std::string fileName)
{
    std::ifstream file;
    file.open(fileName.c_str(),std::fstream::in);

    std::vector<glm::vec2> startMarkers;
    std::string line;
    glm::vec2 currentPoint;
    while (getline(file,line))
    {
        std::istringstream lineStream(line);
        std::string current_int_string;
        std::getline(lineStream,current_int_string,' ');
        currentPoint[0] = std::stoi(current_int_string);
        std::getline(lineStream,current_int_string,' ');
        currentPoint[1] = std::stoi(current_int_string);
        startMarkers.push_back(currentPoint);
    }

    return startMarkers;
}

//---------------------------------------------------------------


#endif // DEFORMATIONCLEAN_HPP
