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
        currentColor = getColor(*(m_arrayValues+i));
//        *m_imageValues = currentColor.r;
//        *(m_imageValues+1) = currentColor.g;
//        *(m_imageValues+2) = currentColor.b;

        m_imageValues[i*3] = currentColor.r;
        m_imageValues[i*3+1] = currentColor.g;
        m_imageValues[i*3+2] = currentColor.b;
    }

    computeSizes();
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

