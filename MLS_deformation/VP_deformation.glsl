#version 330
in vec2 posAttr;
in vec2 texCoord;

layout (std140) uniform shader_data
{
  vec2 array[100];
  int nbPoints;
  float val;
};


uniform mat4 matrix;
out vec3 Color;
out vec2 Texcoord;

uniform sampler2D tex;
uniform sampler2D test;

void main()
{
//    Color = colAttr;
//    Texcoord = texCoord;
    Texcoord = texCoord + array[0];
//    gl_Position = matrix * vec4(posAttr, 0.0, 1.0);
    gl_Position = vec4(posAttr,0.0,1.0);
}

