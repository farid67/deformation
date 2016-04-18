#include "DeformationWindow.hpp"
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

//        int width, height;
//        image = SOIL_load_image("../Ressources/tarsier.jpg",&width,&height,0,SOIL_LOAD_RGB);

        // test simple image red and blue
//        int height = 1080, width =1920;
//        image = new unsigned char[height*width*3*sizeof(unsigned char)];
//        int offset = 0;
//        for (offset = 0; offset < (height*width)/2; ++offset)
//        {
//            image[offset*3+0] = 255;
//            image[offset*3+1] = 0;
//            image[offset*3+2] = 0;
//        }
//        for (; offset < height*width; ++offset)
//        {
//            image[offset*3+0] = 0;
//            image[offset*3+1] = 0;
//            image[offset*3+2] = 255;
//        }

        // test reading from file
        image = m_colorMap->getImage();
        unsigned int height = m_colorMap->getHeight(), width = m_colorMap->getWidth();


        int i,j;

        // test Deformation set
//        glm::vec2 middle(width/2,height/2),middle_2(width/2 + m_frame,height/2),tr(0,0),tl(width-1,0),br(0,height-1),bl(width-1,height-1);
//        glm::vec2 top(width/2,0),left(0,height/2),right(width-1,height/2),bottom(width/2,height-1);
//        glm::vec2 other(width/4,height/4),other2(width/4 - m_frame,height/4);

//        std::vector<glm::vec2> p;
//        p.push_back(tr);
//        p.push_back(tl);
//        p.push_back(br);
//        p.push_back(bl);
//        p.push_back(top);
//        p.push_back(left);
//        p.push_back(right);
//        p.push_back(bottom);
//        p.push_back(middle);
//        p.push_back(other);

//        std::vector<glm::vec2> q;
//        q.push_back(tr);
//        q.push_back(tl);
//        q.push_back(br);
//        q.push_back(bl);
//        q.push_back(top);
//        q.push_back(left);
//        q.push_back(right);
//        q.push_back(bottom);
//        q.push_back(middle_2);
//        q.push_back(other2);

//        DeformationClean d (p,q);

        DeformationClean d;
        if (m_frame < m_DeformationSet.size())
            d = m_DeformationSet[m_frame];
        else
            d = *(m_DeformationSet.end());

        int deformation_grid_size = 5;
        // Deformation set
        std::vector <glm::vec2> points; // points to deform
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if ((i%deformation_grid_size == 0) && (j%deformation_grid_size == 0))
                    points.push_back(glm::vec2(j,i));
            }
        }
        std::cout << points.size() << std::endl;
        std::cout << "set created" << std::endl;

        std::vector <glm::vec2> dest_points = d.RigidDeformationSet(points);

        std::cout << "deformation computed" << std::endl;

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
        GLuint texture, test_texture;
        texture = m_program->uniformLocation("tex");
        test_texture = m_program->uniformLocation("test");

        // bind the location to the texture unit
        glUniform1i(texture,0);
        glUniform1i(test_texture,1);


        // VAO - VBO Part

        // Create Vertex Array Object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create a Vertex Buffer Object and copy the vertex data to it
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = {
        //  Position      Color             Texcoords
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
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
        glVertexAttribPointer(m_posAttr,2,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),0);

        // color
        glEnableVertexAttribArray(m_colAttr);
        glVertexAttribPointer(m_colAttr,3,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(void*) (2*sizeof(GLfloat)));

        // texture Coord
        glEnableVertexAttribArray(m_texAttr);
        glVertexAttribPointer(m_texAttr,2,GL_FLOAT,GL_FALSE,7*sizeof(GLfloat),(void*) (5*sizeof(GLfloat)));

        // load Texture

//        glGenTextures(1,&m_text);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D,texture);

        // ------------------  Test send UBO to GPU

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

        // ------------------- end UBO Test

        // deformation


        int width, height;
        image = SOIL_load_image("../Ressources/tarsier.jpg",&width,&height,0,SOIL_LOAD_RGB);

        int i,j;

        glm::vec2 middle(width/2,height/2),middle_2(width/2 + m_frame*10,height/2),tr(0,0),tl(width-1,0),br(0,height-1),bl(width-1,height-1);
        glm::vec2 top(width/2,0),left(0,height/2),right(width-1,height/2),bottom(width/2,height-1);

        std::vector<glm::vec2> p;
        p.push_back(tr);
        p.push_back(tl);
        p.push_back(br);
        p.push_back(bl);
        p.push_back(top);
        p.push_back(left);
        p.push_back(right);
        p.push_back(bottom);
        p.push_back(middle);

        std::vector<glm::vec2> q;
        q.push_back(tr);
        q.push_back(tl);
        q.push_back(br);
        q.push_back(bl);
        q.push_back(top);
        q.push_back(left);
        q.push_back(right);
        q.push_back(bottom);
        q.push_back(middle_2);

        DeformationClean d (p,q);

        // Deformation set
        std::vector <glm::vec2> points; // points to deform
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if ((i%5 == 0) && (j%5 == 0))
                    points.push_back(glm::vec2(j,i));
            }
        }
        std::cout << points.size() << std::endl;
        std::cout << "set created" << std::endl;

        std::vector <glm::vec2> dest_points = d.RigidDeformationSet(points);

        std::cout << "deformation computed" << std::endl;

        int compteur_tmp = 0;
        unsigned char image_deformed[height*width*3*sizeof(char)];
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if ((i%5 == 0) && (j%5 == 0))
                {
                    glm::vec2 dest = dest_points[compteur_tmp++];

                    // for out of bound values
    //                if (dest[0] < 0.0f)
    //                    dest[0] = 0.0f;
    //                if (dest[1] < 0.0f)
    //                    dest[1] = 0.0f;
    //                if (dest[0] > width-1)
    //                    dest[0] = static_cast<double>(width-1);
    //                if (dest[1] > height-1)
    //                    dest[1] = static_cast<double>(height-1);
        //            std::cout << dest[0] << "," << dest[1] << std::endl;

                    image_deformed[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0])) * 3] = image[(width*i + j)* 3]; //r
                    image_deformed[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0])) * 3 +1] = image[(width*i + j)* 3 +1]; //g
                    image_deformed[(static_cast<int>(dest[1])*width + static_cast<int>(dest[0])) * 3 +2] = image[(width*i + j)* 3 +2]; //b


    //                image_deformed[(width*i +j) * 3] = image[(width*i + j)* 3]; //r
    //                image_deformed[(width*i +j) * 3 +1] = image[(width*i + j)* 3 +1]; //g
    //                image_deformed[(width*i +j) * 3 +2] = image[(width*i + j)* 3 +2]; //b


                    // debug zone
    //                if ((width * i + j) % 8000  == 0)
    //                {
    //                    std::cout << "Point courant (" << i <<","<< j<< ")" << std::endl;
    //                    std::cout << "      Point ref (" << dest[1] << "," << dest[0] <<")"<< std::endl;
    //                }
                }
            }
        }



//        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image_deformed);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);


        // filtering options
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D,test_texture);

        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // delete part

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);

        glDeleteTextures(1,&m_text);
        glDeleteBuffers(1, &ubo);
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
    else
    {
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"VP_deformation.glsl");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"FP_deformation.glsl");
    }
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_texAttr = m_program->attributeLocation("texCoord");

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
    m_DeformationSet = getDeformationSetFromFile("../Ressources/shortMatches.txt");
}

//---------------------------------------------------------------
