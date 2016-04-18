#version 330

in vec2 posAttr;
in vec3 colAttr;
in vec2 texCoord;
uniform mat4 matrix;
out vec3 Color;
out vec2 Texcoord;

void main()
{
    Color = colAttr;
    Texcoord = texCoord;
//    gl_Position = matrix * vec4(posAttr, 0.0, 1.0);
    gl_Position = vec4(posAttr,0.0,1.0);
}

