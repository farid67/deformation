#ifndef COLORMAP_HPP
#define COLORMAP_HPP
#include "DeformationClean.hpp"
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct Color
{
    Color():r(0),g(0),b(0){}
    Color(unsigned char r_,unsigned char g_,unsigned char b_):r(r_),g(g_),b(b_){}
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


// class which will take an array, and which will create an Image base on the ColorMap

class ColorMap
{
public:
    ColorMap();
    ColorMap(double* array, unsigned int size);
    virtual ~ColorMap();

    void setColorMap(std::vector<Color> newColorMap);
    unsigned int getHeight();
    unsigned int getWidth();

    unsigned char* getImage();

    void computeBoundingValue();

    // test values -> get the bounding box of the non-zero elements
    unsigned int max_i,min_i,max_j, min_j;

    void computeIntegralImages();

    // compute the variance of the image in the sub-square defined by the coordinates
    double Variance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

private:
    //test function to compute variance in different areas
    void computeVariance(unsigned int x_spacing, unsigned int y_spacing);

    cv::Mat m_squareSumIntegral;
    cv::Mat m_sumIntegral;

    double* m_arrayValues;
    unsigned int m_size;

    unsigned int m_height;
    unsigned int m_width;
    void computeSizes();

    // get Interpolated Color based on the ColorMap
    Color getColor (double value);
    void createDefaultColorMap(double min, double max);
    double m_minValue,m_maxValue;
    std::vector< std::pair<double,Color> > m_colorMap;
    unsigned char* m_imageValues;
};

#endif // COLORMAP_HPP
