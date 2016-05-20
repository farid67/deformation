#include "ColorMap.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

//---------------------------------------------------------------

ColorMap::ColorMap():
    m_arrayValues(NULL),m_size(0)
{

}

//---------------------------------------------------------------

ColorMap::ColorMap(double *array, unsigned int size):
    m_size(size)
{
    m_arrayValues = new double[size];
    // copy array values (in case of changing the colorMap for example
    std::copy (array,array+size,m_arrayValues);

    // get Min and Max Value
    std::pair<double*,double*> minmax = std::minmax_element(array,array+size);
    m_minValue = *minmax.first;
    m_maxValue = *minmax.second;

    // create Default ColorMap
    createDefaultColorMap(m_minValue, m_maxValue);

    m_imageValues = new unsigned char[size*3];

    Color currentColor;
    for (unsigned int i = 0;i < size; ++i)//,m_imageValues +=3)
    {
        if(*(m_arrayValues+i) == 0 )
        {
            m_imageValues[i*3] = 255;
            m_imageValues[i*3+1] = 255;
            m_imageValues[i*3+2] = 255;
        }

        else
        {

            currentColor = getColor(*(m_arrayValues+i));

            m_imageValues[i*3] = currentColor.r;
            m_imageValues[i*3+1] = currentColor.g;
            m_imageValues[i*3+2] = currentColor.b;
        }
    }

    computeSizes();

    computeIntegralImages();

    // test in important areas

    // border
//    std::cout << Variance(560,1035,735,1424) << std::endl;
    // region of interest
    // band
//    std::cout << Variance(200,0,300,1920) << std::endl;
//    std::cout << Variance(800,1100,900,1200) << std::endl;
    // all image
//        std::cout << Variance(0,0,1080,1920) << std::endl;

    // grid

    computeVariance(50,100);

}

//---------------------------------------------------------------

ColorMap::~ColorMap()
{
    if (m_arrayValues != nullptr)
        delete [] m_arrayValues;
    if (m_imageValues != nullptr)
        delete [] m_imageValues;
}

//---------------------------------------------------------------

void ColorMap::setColorMap(std::vector<Color> newColorMap)
{
    // here change the ColorMap and change the image based on this colorMap
}

//---------------------------------------------------------------

// we Can have more precision by changing RGB with double values !
Color ColorMap::getColor(double value)
{
    assert(value >= m_colorMap.begin()->first);
    assert(value <= m_colorMap.rbegin()->first);


    std::vector< std::pair<double,Color> >::iterator it;

    double val_sup, val_inf;
    Color color_sup, color_inf;
    for(it = m_colorMap.begin(); it < m_colorMap.end(); ++it)
    {
        val_sup = (*it).first;
        color_sup = (*it).second;
        if (val_sup > value)
        {
            it--;
            val_inf = (*it).first;
            if (val_inf != value)
            {
                color_inf = (*it).second;
                // compute interpolation between
                double val_ecart = val_sup - val_inf; // 100%
                double proportion = (value - val_inf)/val_ecart;

                Color resultColor;
                // r
                int diff_r = color_sup.r - color_inf.r;
                resultColor.r = proportion*diff_r + color_inf.r;
                // g
                int diff_g = color_sup.g - color_inf.g;
                resultColor.g = proportion*diff_g + color_inf.g;
                // b
                int diff_b = color_sup.b - color_inf.b;
                resultColor.b = proportion*diff_b + color_inf.b;

                return resultColor;
            }
            else
                return (*it).second;
        }
    }

    // if here return the last value
    return (*it).second;
}

//---------------------------------------------------------------

void ColorMap::createDefaultColorMap(double min, double max)
{
    // 0 -> 0 0 128
    // 0.25 -> 0 128 255
    // 0.5 -> 128 255 128
    // 0.75 -> 255 128 0
    // 1 -> 128 0 0

    double scale = max-min;
    double proportion = scale/4;

    Color col0(0,0,128);
    m_colorMap.push_back( std::pair<double,Color>(min,col0));

    Color col1(0,128,255);
    m_colorMap.push_back( std::pair<double,Color>(proportion+min,col1));

    Color col2(128,255,128);
    m_colorMap.push_back( std::pair<double,Color>(proportion*2+min,col2));

    Color col3(255,128,0);
    m_colorMap.push_back( std::pair<double,Color>(proportion*3+min,col3));

    Color col4(128,0,0);
    m_colorMap.push_back( std::pair<double,Color>(proportion*4+min,col4));

}

//---------------------------------------------------------------

void ColorMap::computeSizes()
{
    switch (m_size)
    {
    case 345600:
        m_height = 480;
        m_width = 720;
        break;
    case 2073600:
        m_height = 1080;
        m_width = 1920;
        break;
    default:
        std::cout << "size not currently implemented" << std::endl;
        m_height = 0;
        m_width = 0;
        break;
    }
}

//---------------------------------------------------------------

unsigned int ColorMap::getHeight()
{
    return m_height;
}

//---------------------------------------------------------------

unsigned int ColorMap::getWidth()
{
    return m_width;
}

//---------------------------------------------------------------

unsigned char* ColorMap::getImage()
{
    return m_imageValues;
}

//---------------------------------------------------------------

void ColorMap::computeBoundingValue()
{
    long sum=0;
    min_i = std::numeric_limits<unsigned int>::max(), min_j = std::numeric_limits<unsigned int>::max(), max_i = 0, max_j = 0;

    unsigned char min = std::numeric_limits<unsigned char>::max();
    unsigned char max = std::numeric_limits<unsigned char>::min();
    for (unsigned int i =  0; i < m_height; ++i)
    {
        for (unsigned int j = 0; j < m_width; ++j)
        {
            sum += m_arrayValues[i* m_width + j];
            if (m_arrayValues[i* m_width + j] < min)
                min = m_arrayValues[i* m_width + j];
            if (m_arrayValues[i* m_width + j] > max)
                max = m_arrayValues[i* m_width + j];
            if (m_arrayValues[i* m_width + j]>0)
            {
                if(i > max_i)
                    max_i = i;
                if (i < min_i)
                    min_i = i;
                if (j > max_j)
                    max_j = j;
                if (j < min_j)
                    min_j = j;
            }
        }
    }

    std::cout << "min : " << static_cast<int>(min) << " max : " << static_cast<int>(max) << std::endl;
}

//---------------------------------------------------------------

void ColorMap::computeIntegralImages()
{
    computeBoundingValue();

    // convert original Image into cv Image
    cv::Mat originalImage (m_height,m_width,CV_8UC1);
    unsigned int size = m_height * m_width;
    std::copy(m_arrayValues,m_arrayValues+size,originalImage.data);

    cv::Mat blurImage (m_height,m_width,CV_8UC1);
    cv::bilateralFilter(originalImage,blurImage,10,10,50);

    m_squareSumIntegral.create(m_height,m_width,CV_64F);
    m_sumIntegral.create(m_height,m_width,CV_64F);


//    cv::integral(originalImage,Sum,squareSum);
    cv::integral(blurImage,m_sumIntegral,m_squareSumIntegral,CV_64F);
    cv::imwrite("test.png",blurImage);


    std::vector<glm::vec2> startMarkers = getStartMarkers("../Ressources/HD_Set/kP.txt");
    cv::Mat keyPointsImage(originalImage);

    std::cout << "KeyPoints.size() : " << startMarkers.size() << std::endl;

    for (unsigned int i = 0; i < startMarkers.size(); ++i)
        keyPointsImage.at<unsigned char>(startMarkers[i][1],startMarkers[i][0]) = 255;

//    keyPointsImage.at<unsigned char>() = 255;

    cv::imwrite("keyPointsImage.png",keyPointsImage);
}


//---------------------------------------------------------------

// the two coordinates correspond to the top left corner and bottom right corner
// -> given in [0, width-1] for y and [0,height-1] for x
double ColorMap::Variance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    assert (x2 >= x1 && y2 >= y1);

    assert (x2 <= m_height && y2 <= m_width);

    unsigned int top_right_x = x1, top_right_y = y2, bottom_left_x = x2, bottom_left_y = y1;

    // compute integral sum for each corner
    double sum0 = m_sumIntegral.at<double>(x1,y1);
    double sum1 = m_sumIntegral.at<double>(top_right_x,top_right_y);
    double sum2 = m_sumIntegral.at<double>(bottom_left_x,bottom_left_y);
    double sum3 = m_sumIntegral.at<double>(x2,y2);

    unsigned int size = (x2 - x1) * (y2 - y1);

    double mean = (sum0+sum3-sum1-sum2)/size;

    double squareSum0 = m_squareSumIntegral.at<double>(x1,y1);
    double squareSum1 = m_squareSumIntegral.at<double>(top_right_x,top_right_y);
    double squareSum2 = m_squareSumIntegral.at<double>(bottom_left_x,bottom_left_y);
    double squareSum3 = m_squareSumIntegral.at<double>(x2,y2);


    double squareSum = (squareSum0 + squareSum3 - squareSum1 - squareSum2);

//    std::cout << "taille échantillon " << size << " somme " << sum3 <<" moyenne " << mean << " square sum " << squareSum3 << std::endl;

//    return (1./static_cast<double>(size)) *  squareSum - mean*mean;

    return mean;
}

//---------------------------------------------------------------

void ColorMap::computeVariance(unsigned int x_spacing,unsigned int y_spacing)
{
    unsigned int i,j,img_i,img_j;
    double currentVariance = 0.;
    cv::Mat VarianceImage (m_height,m_width,CV_8UC1);
    for (i = 0; i < m_height && i+x_spacing < m_height; i+=x_spacing)
    {
        std::cout << "          I : " << i << std::endl;
        for (j = 0; j < m_width && j+y_spacing < m_width; j+=y_spacing)
        {
//            std::cout << "          J : " << j << std::endl;
            currentVariance = Variance(i, j, i+x_spacing, j+y_spacing);
//            std::cout << currentVariance << std::endl;
            // writing in image
            for (img_i = i; img_i < i+x_spacing; ++img_i)
                for (img_j = j ; img_j < j+y_spacing; ++img_j)
                    VarianceImage.at<unsigned char>(img_i,img_j) = (currentVariance);///600.) * 255.;
        }
        if (j < m_width)
        {
//            std::cout << "Last          J : " << j << std::endl;
            currentVariance = Variance(i,j,i+x_spacing,m_width);
//            std::cout << currentVariance << std::endl;
            for (img_i = i; img_i < i+x_spacing; ++img_i)
                for (img_j = j ; img_j < m_width; ++img_j)
                    VarianceImage.at<unsigned char>(img_i,img_j) = (currentVariance);///600.) * 255.;
        }
    }
    if (i< m_height)
    {
        for (j = 0; j < m_width && j+y_spacing < m_width; j+=y_spacing)
        {
            currentVariance = Variance(i, j, m_height, j+y_spacing);
//            std::cout << currentVariance << std::endl;
            for (img_i = i; img_i < m_height; ++img_i)
                for (img_j = j ; img_j <  j+y_spacing; ++img_j)
                    VarianceImage.at<unsigned char>(img_i,img_j) = (currentVariance);///600.) * 255.;
        }
        if (j < m_width)
        {
            currentVariance = Variance(i,j,m_height,m_width);
//            std::cout << currentVariance << std::endl;
            for (img_i = i; img_i < m_height; ++img_i)
                for (img_j = j ; img_j <  m_width; ++img_j)
                    VarianceImage.at<unsigned char>(img_i,img_j) = (currentVariance);///600.) * 255.;
        }
    }

    cv::imwrite("variance.png",VarianceImage);


}

//---------------------------------------------------------------

