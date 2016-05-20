#version 330
in vec2 posAttr;
in vec2 texCoord;
in vec3 colAttr;

//layout (std140) uniform shader_data
//{
//  vec2 array[100];
//  int nbPoints;
//  float val;
//};


uniform mat4 matrix;
out vec3 Color;
out vec2 Texcoord;

uniform sampler2D image;
uniform isampler1D deformation_texture;
uniform sampler2D defo2D;


// Deformation markers
//vec2 startMarkers;
//vec2 endMarkers;





void main()
{
//    vec2 dimension = textureSize(image,0);
//    width = dimension.x;
//    height = dimension.y;


//    Color = initMarkers(texCoord,image);
//    Color = texture(image,texCoord).rgb;
//    Color = colAttr;
    Texcoord = texCoord;
//    Texcoord = texCoord + array[0];
//    gl_Position = matrix * vec4(posAttr, 0.0, 1.0);
    gl_Position = vec4(posAttr,0.0,1.0);
}

