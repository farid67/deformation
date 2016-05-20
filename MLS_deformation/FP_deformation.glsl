#version 330

in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D image;
uniform isampler1D deformation_texture;
uniform sampler2D defo2D;

layout (std140) uniform shader_data
{
  vec2 array[100];
  int nbPoints;
  float val;
};

float height;
float width;


vec3 showMarkers(vec2 uv)
{
    vec3 col;
    ivec4 defo;
    ivec2 dimension = textureSize(image,0);
    width = float(dimension.x);
    height = float (dimension.y);
    int size = textureSize(deformation_texture,0);
//    col = texture(image,uv).rgb;
    col = vec3(0,0,0);
    if (col.rgb == vec3(1,1,1))
        col = vec3(0,0,0);


    for(int i = 0; i < size ; ++i)
    {
//        defo = texture(deformation_texture,float(i)/float(size));
        defo = texelFetch(deformation_texture,i,0);
        if ((uv.x == float(defo.r)/width )&& (uv.y == (height-float(defo.g))/height))
        {
            col = vec3(1,1,1);
            break;
        }

//        if ((uv.x >= float(defo.r)/width -0.01)&& (uv.x <= float(defo.r)/width +0.01) &&
//                (uv.y >= (height-float(defo.g))/height - 0.01) &&  (uv.y <= (height-float(defo.g))/height + 0.01))
//            col = vec3(0,0,0);

//        if (uv.x >= float(defo.b)/width && uv.x <= float(defo.b)/width +0.1)
//            col = vec3(0,0,0);
//        col = vec3(float(defo.r)/1920.);

    }

    return col;
}

vec3 showMarkers2(vec2 uv)
{
    vec3 col;
    if (texture(defo2D,uv).r == 1.)
        col = vec3(1,1,1);
    else
        col = texture(image,uv).rgb;
//        col = vec3 (0,0,0);

//    col = vec3(texture(defo2D,uv).r);

    return col;
}



void main()
{
//    if (Texcoord.x > 0.5)
//        outColor = texture(image,Texcoord); //* vec4(Color, 1.0);
//    else
//        vec3 Color = vec3(val,0.,0.);
//    outColor = vec4(clamp(Color,vec3(0.0),vec3(1.0)),1.0);

    // Transform the color into black if the input color is white

    vec3 col;
    col = showMarkers2(Texcoord);
    outColor = vec4(col,1);

    // test size corectly retrieved
//    ivec2 dimension = textureSize(image,0);
//    int size = textureSize(deformation_texture,0);
//    width = float(dimension.x);
//    height = float(dimension.y);

//    if (Texcoord.x > float(size)/58.)
//        outColor = texture (image,Texcoord);
//    else
//        outColor = texture (deformation_texture,Texcoord);

}
