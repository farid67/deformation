#include "DeformationWindow.hpp"
#include "DeformationFast.hpp"
#include <QToolBar>
#include <QWidget>


//---------------------------------------------------------------

DeformationWindow::DeformationWindow(bool use_texture):
    m_program(0),
    m_frame(0),
    m_texture(use_texture)
{
}

//---------------------------------------------------------------

void DeformationWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

//    DrawGrid(10);

    unsigned char* image;

    m_program->bind();

    //Point test
//    /*

    if (m_texture == false)
    {


        QMatrix4x4 matrix;
        matrix.perspective(60.0f, width()/height(), 0.1f, 1000.0f);
        matrix.translate(0, 0, -100);
    //    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

        m_program->setUniformValue(m_matrixUniform, matrix);


        std::vector<GLfloat> vertices;

        std::vector<GLfloat> colors;

        std::vector<GLuint> elements;

        if (m_frame < m_DeformationSet.size())
        {
            m_deformation = m_DeformationSet[m_frame];
            std::cout << "Set de points num " << m_frame << " with  " << m_deformation.startMarkers().size()  << " points" << std::endl;
        }
        else
        {
            std::cout << "No more set of points"  << std::endl;
            m_deformation = *(m_DeformationSet.rbegin());
        }


        int deformation_grid_size = 5;

        std::vector <glm::vec2> dest_points = m_deformation.RigidDeformationSet(m_gridPoints);
//        std::vector <glm::vec2> dest_points = m_defoFast->AffineDeformationSet(m_deformation.startMarkers(),m_deformation.endMarkers());
//        std::vector <glm::vec2> dest_points = points;

        std::cout << "deformation computed" << std::endl;
        unsigned int height = 1080, width = 1920, i,j;
        image= m_image;

        int compteur_tmp = 0;
        int line_short = (width-1)/deformation_grid_size;
        int column_short = (height-1)/deformation_grid_size;
        for (i = 0; i < height; i+=deformation_grid_size)
        {
            for (j = 0; j < width; j+=deformation_grid_size)
            {
                glm::vec2 dest = dest_points[compteur_tmp];

                // get vertice position
                vertices.push_back( (dest[0]/width-0.5)*2);
                vertices.push_back( (-(dest[1]/height -0.5))*2);

                // get color
                GLfloat red  = image[(static_cast<int>(i)*width + static_cast<int>(j) ) * 3];
                colors.push_back(red/255.f);

                GLfloat green  = image[(static_cast<int>(i)*width + static_cast<int>(j) ) * 3 +1];
                colors.push_back(green/255.f);

                GLfloat blue  = image[(static_cast<int>(i)*width + static_cast<int>(j) ) * 3 +2];
                colors.push_back(blue/255.f);


                // set elements indexes to draw
                if (j/deformation_grid_size < line_short)
                {
                    if (i == 0)
                    {
                        elements.push_back(compteur_tmp);
                        elements.push_back(compteur_tmp+(line_short+1));
                        elements.push_back(compteur_tmp+1);
                    }
                    else if (i/deformation_grid_size == column_short)
                    {
                        elements.push_back(compteur_tmp);
                        elements.push_back(compteur_tmp+1);
                        elements.push_back(compteur_tmp-line_short);
                    }
                    else
                    {
                        elements.push_back(compteur_tmp);
                        elements.push_back(compteur_tmp+(line_short+1));
                        elements.push_back(compteur_tmp+1);

                        elements.push_back(compteur_tmp);
                        elements.push_back(compteur_tmp+1);
                        elements.push_back(compteur_tmp-line_short);
                    }
                }
                compteur_tmp ++;
            }
        }


        // FULL POINTS test -> modify deformation set to have all points (loop for all points)


    //    for (i = 0; i < dest_points.size(); ++i)
    //    {
    //        glm::vec2 dest = dest_points[i];
    //        // position
    //        vertices.push_back(2* dest[0]/width - 1.0);
    //        vertices.push_back(-(2* dest[1]/height - 1.0));

    //        // color
    //        GLfloat red  = image[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0]) ) * 3];
    //        colors.push_back(red/255.f);

    //        GLfloat green  = image[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0]) ) * 3 +1];
    //        colors.push_back(green/255.f);

    //        GLfloat blue  = image[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0]) ) * 3 +2];
    //        colors.push_back(blue/255.f);
    //    }

        // end FULL POINTS

        // test Vao - Vbo

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo[2];
        glGenBuffers(2, vbo);

        glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat),&vertices[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(m_posAttr);
        glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
        glBufferData(GL_ARRAY_BUFFER,colors.size()*sizeof(GLfloat),&colors[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(m_colAttr);
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);



        GLuint ebo;
        glGenBuffers(1,&ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*elements.size(),&elements[0],GL_STATIC_DRAW);


        // End test Vao - Vbo

        glDisable(GL_CULL_FACE);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);

        // delete Part

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(2, vbo);

        glDeleteVertexArrays(1, &vao);
    }

    else // m_texture = true
    {
        // Texture location
        GLuint image_texture, deformation_texture, defo2D;
        image_texture = m_program->uniformLocation("image");
        deformation_texture = m_program->uniformLocation("deformation_texture");
        defo2D = m_program->uniformLocation("defo2D");
        // bind the location to the texture unit
        glUniform1i(image_texture,0);
        glUniform1i(deformation_texture,1);
        glUniform1i(defo2D,2);


        // VAO - VBO Part

        // Create Vertex Array Object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create a Vertex Buffer Object and copy the vertex data to it
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = {
        //  Position      Texcoords
            -1.f,  1.f, 0.0f, 0.0f, // Top-left
             1.f,  1.f, 1.0f, 0.0f, // Top-right
             1.f, -1.f, 1.0f, 1.0f, // Bottom-right
            -1.f, -1.f, 0.0f, 1.0f  // Bottom-left
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint ebo;
        glGenBuffers(1,&ebo);

        GLuint elements[] = {
            0,1,2,
            2,3,0
        };

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

        // position
        glEnableVertexAttribArray(m_posAttr);
        glVertexAttribPointer(m_posAttr,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),0);

        // color
//        glEnableVertexAttribArray(m_colAttr);
//        glVertexAttribPointer(m_colAttr,3,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(void*) (2*sizeof(GLfloat)));

        // texture Coord
        glEnableVertexAttribArray(m_texAttr);
        glVertexAttribPointer(m_texAttr,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),(void*) (2*sizeof(GLfloat)));

        // load Texture

//        glGenTextures(1,&m_text);

        // ------------------  Test send UBO to GPU
        /*

        GLint ubo_max, ubo_max_size;
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&ubo_max);
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&ubo_max_size);
        std::cout << "nombre max d'ubo : "<< ubo_max <<  " taille maximale pour chaque ubo " << ubo_max_size << std::endl;


        point_struct test_p ;
//        test_p.points = new glm::vec2[100];
        test_p.points[0] = glm::vec2(0.2,0.2);
        test_p.points[1] = glm::vec2(0.2,0.2);
        test_p.nb_points = 2;
        test_p.val = 0.5f;
        // initialisation
        GLuint ubo;
        glGenBuffers(1,&ubo);
        std::cout << sizeof(test_p) << std::endl;

        glBindBuffer(GL_UNIFORM_BUFFER,ubo);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(point_struct),&test_p,GL_DYNAMIC_COPY);
        //unbind
        glBindBuffer(GL_UNIFORM_BUFFER,0);


        // update GPU value
        glBindBuffer(GL_UNIFORM_BUFFER,ubo);
        GLvoid* gpuBuff = glMapBuffer(GL_UNIFORM_BUFFER,GL_WRITE_ONLY);
        memcpy(gpuBuff,&test_p,sizeof(point_struct));
        // Unbind
        glUnmapBuffer(GL_UNIFORM_BUFFER);

        // connection to the GPU
        GLuint buffer_index = glGetUniformBlockIndex(m_program->programId(),"shader_data");
        // Bind to point 0 because there is no other blocks to bind
        glBindBufferBase(GL_UNIFORM_BUFFER,0,ubo);
        // once the binding is done, we need to create the connection
        glUniformBlockBinding(m_program->programId(),buffer_index,0); // 0 because the binding is done at this point

        */
        // ------------------- end UBO Test

        glActiveTexture(GL_TEXTURE0+0);
        glBindTexture(GL_TEXTURE_2D,image_texture);

        // deformation

        unsigned int height = m_colorMap->getHeight(), width = m_colorMap->getWidth();
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,m_image);



        // filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//        glBindTexture(GL_TEXTURE_1D,deformation_texture);

        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_1D,deformation_texture);

        if (m_frame < m_DeformationSet.size())
        {
            m_deformation = m_DeformationSet[0];
            std::cout << "Set de points num " << m_frame << " with  " << m_deformation.startMarkers().size()  << " points" << std::endl;
        }
        else
        {
            std::cout << "No more set of points"  << std::endl;
            m_deformation = *(m_DeformationSet.rbegin());
        }

        std::vector<glm::vec2> startMarkers = m_deformation.startMarkers();
        std::vector<glm::vec2> endMarkers = m_deformation.endMarkers();
        unsigned int nbDefo = startMarkers.size();

        // texture data
        int deformationData[nbDefo*4];

        for (unsigned int i = 0; i < nbDefo; i++)
        {
            deformationData[i*4]   = static_cast<int>(startMarkers[i][0]);
            deformationData[i*4+1] = startMarkers[i][1];
            deformationData[i*4+2] = endMarkers[i][0];
            deformationData[i*4+3] = endMarkers[i][1];

        }
        glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA32I,nbDefo,0,GL_RGBA_INTEGER,GL_INT,deformationData);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


        glActiveTexture(GL_TEXTURE0+2);
        glBindTexture(GL_TEXTURE_2D,defo2D);

        unsigned char defo2Data[height*width*3];
        std::fill(defo2Data,defo2Data+height*width*3, 0);
        for (unsigned int i = 0; i < nbDefo; i++)
        {
            int index =static_cast<int>(startMarkers[i][1]) * width + static_cast<int>(startMarkers[i][0]);
            defo2Data[index*3] = 255;
        }

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,defo2Data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // delete part
        // unbind 2D Texture
        glBindTexture(GL_TEXTURE_2D,0);
        // unbind 1D Texture
        glBindTexture(GL_TEXTURE_1D,0);

        glDisableVertexAttribArray(m_texAttr);
        glDisableVertexAttribArray(m_posAttr);


        glDeleteTextures(1,&image_texture);
        glDeleteTextures(1,&defo2D);
        glDeleteTextures(1,&deformation_texture);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);

        glDeleteVertexArrays(1, &vao);


    }

//    SOIL_free_image_data(image);

    m_program->release();

    ++m_frame;

}

//---------------------------------------------------------------

void DeformationWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    if (m_texture == false )
    {
        std::string vp = readShaderProg("VP.glsl");
        std::string fp = readShaderProg("FS.glsl");
//        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"VP.glsl");
//        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"FS.glsl");
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vp.c_str());
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fp.c_str());
    }
    else // using texture to send deformation info
    {
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"VP_deformation.glsl");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"FP_deformation.glsl");
    }
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texAttr = m_program->attributeLocation("texCoord");
    m_colAttr = m_program->attributeLocation("colAttr");
    if (m_colAttr == -1)
        std::cout << "Invalid colAttr" << std::endl;
    m_matrixUniform = m_program->uniformLocation("matrix");

    // Creation of the colorMap based on a raw file
    std::ifstream binaryFile("../Ressources/hd_video", std::ios::in|std::ios::binary);
    binaryFile.seekg(0,binaryFile.end);
    int length = static_cast<int>(binaryFile.tellg());
    binaryFile.seekg(0,binaryFile.beg);

    size_t size = length /sizeof(double);
    double* buffer = new double[size];

    binaryFile.read((char*)buffer,length);
    binaryFile.close();

    // creation ColorMap
    m_colorMap = new ColorMap(buffer,size);


    // creation of the deformation Sets
    m_DeformationSet = getDeformationSetFromFile("../Ressources/HD_50/matches.txt");
    m_startMarkers = getStartMarkers("../Ressources/HD_50/kP.txt");

//    m_deformation = DeformationClean(m_startMarkers);


    int deformation_grid_size = 5;

    m_image = m_colorMap->getImage();
    unsigned int height = m_colorMap->getHeight(), width = m_colorMap->getWidth();
    m_colorMap->computeBoundingValue();

    unsigned int min_i = m_colorMap->min_i;
    unsigned int min_j = m_colorMap->min_j;
    unsigned int max_i = m_colorMap->max_i;
    unsigned int max_j = m_colorMap->max_j;

    std::cout << "Bounding box color Map -> (" << min_i << "," << min_j << "," << max_i << "," << max_j << ")" << std::endl;

    unsigned int i,j;

    // Deformation set
//    std::vector <glm::vec2> points; // points to deform

    m_gridPoints.resize((height*width)/(deformation_grid_size*deformation_grid_size));
    unsigned int compteur;
    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            if ((i%deformation_grid_size == 0) && (j%deformation_grid_size == 0))
                m_gridPoints[compteur++] = glm::vec2(j,i);
        }
    }


//    m_defoFast = new DeformationFast(m_startMarkers,m_gridPoints);
}

//---------------------------------------------------------------
