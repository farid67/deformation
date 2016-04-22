#ifndef DEFORMATIONWINDOW_HPP
#define DEFORMATIONWINDOW_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "DeformationClean.hpp"
#include "DeformationFast.hpp"
#include "ColorMap.hpp"

#include <SOIL.h>
//#include "/home/farid/Documents/Soft/soil/SOIL/src/SOIL.h"
#include <qopenglshaderprogram.h>
#include "window.hpp"
#include <QScreen>

typedef struct
{
    glm::vec2 points[100];
    int nb_points;
    float val;
}point_struct;

class DeformationWindow : public window
{
public:
    DeformationWindow(bool use_texture);
    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_texAttr;
    GLuint m_matrixUniform;
    GLuint m_text; // Coord Text

    bool m_texture;

    QOpenGLShaderProgram* m_program;
    int m_frame;

    ColorMap* m_colorMap;
    std::vector <DeformationClean> m_DeformationSet;
    std::vector <glm::vec2> m_startMarkers;

    DeformationClean m_deformation;

    DeformationFast* m_defoFast;

    std::vector<glm::vec2> m_gridPoints; // grid Points

    unsigned char* m_image;
};

//---------------------------------------------------------------

inline std::string readShaderProg(const char* vpPath)
{
    std::stringstream content;
    std::string line;
    std::fstream file;
    file.open(vpPath,std::ios::in);
    while (std::getline(file,line))
        content << line << std::endl;
    file.close();
    return content.str();
}

//---------------------------------------------------------------

inline void DrawGrid(int size)
{
    glColor3d(0.0,1.0,0.0);
    glBegin(GL_LINES);

//    glVertex2d(0.1,2);
//    glVertex2d(0.1,-2);

    double d = static_cast<double>(size)/2.0;

    for (double i = -d; i< d; i+=0.1)
    {
        glVertex2d(i,-d);
        glVertex2d(i,d);

        glVertex2d(-d,i);
        glVertex2d(d,i);

        if (i > 100.)
            return;

    }
    glEnd();

}

//---------------------------------------------------------------



#endif // DEFORMATIONWINDOW_HPP
