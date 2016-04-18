#version 330

in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D tex;
uniform sampler2D test;

layout (std140) uniform shader_data
{
  vec2 array[100];
  int nbPoints;
  float val;
};


void main()
{
//    if (Texcoord.x > 0.5)
        outColor = texture(tex,Texcoord); //* vec4(Color, 1.0);
//    else
//        vec3 Color = vec3(val,0.,0.);
//    outColor = vec4(clamp(Color,vec3(0.0),vec3(1.0)),1.0);

}
