out vec4 FragColor;
in vec2 TexCoord;
uniform lowp float uQuadDim;
uniform lowp float uQuadPixel;

void main()
{
    float uQuadDim = 4.0;
    float uQuadPixel = 16.0;
    float parm1 = 1.0 / uQuadDim;
    float parm2 = 1.0 / uQuadPixel;
    float parm3 = 0.5 / (uQuadDim * uQuadPixel);//四舍五入
    float rForward = TexCoord.x;
    float gForward = TexCoord.y;
    float bHor = TexCoord.x;
    float bVert = 1.0 - TexCoord.y;
    float r = (rForward - floor(rForward * uQuadDim) * parm1) * uQuadDim;
    float g = 1.0 - ((gForward - floor(gForward * uQuadDim) * parm1) * uQuadDim);
    r = parm2 * floor(r * uQuadPixel);
    g = parm2 * floor(g * uQuadPixel);
    float b = floor(bVert * uQuadDim) * parm1 + floor(bHor * uQuadDim) * parm1 * parm1;
    FragColor = vec4(r + parm3, g + parm3, b + parm3, 1.0);
}