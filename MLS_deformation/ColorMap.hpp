#ifndef COLORMAP_HPP
#define COLORMAP_HPP
#include <vector>

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

private:

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
