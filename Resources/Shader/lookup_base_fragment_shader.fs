out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTexture2;
uniform float uQuadDim;
uniform float uQuadPixel;
void main()
{
    vec4 vCameraColor = texture2D(inputImageTexture, TexCoord);
    float parm1 = 1.0 / uQuadDim;
    float parm2 = 1.0 / uQuadPixel;

    float blueColor = vCameraColor.b * (uQuadPixel - 1.0);
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / uQuadDim);
    quad1.x = floor(blueColor) - (quad1.y * uQuadDim);

    vec2 quad2;
    quad2.y = floor(ceil(blueColor) / uQuadDim);
    quad2.x = ceil(blueColor) - (quad2.y * uQuadDim);

    vec2 texPos1;
    texPos1.x = (quad1.x * parm1) + 0.5/(uQuadPixel * uQuadDim) + ((parm1 - 1.0/(uQuadPixel * uQuadDim)) * vCameraColor.r);
    texPos1.y = 1.0- ((quad1.y * parm1) + 0.5/(uQuadPixel * uQuadDim) + ((parm1 - 1.0/(uQuadPixel * uQuadDim)) * vCameraColor.g));

    vec2 texPos2;
    texPos2.x = (quad2.x * parm1) + 0.5/(uQuadPixel * uQuadDim) + ((parm1 - 1.0/(uQuadPixel * uQuadDim)) * vCameraColor.r);
    texPos2.y = 1.0 - ((quad2.y * parm1) + 0.5/(uQuadPixel * uQuadDim) + ((parm1 - 1.0/(uQuadPixel * uQuadDim)) * vCameraColor.g));

    vec4 newColor1 = texture2D(inputImageTexture2, texPos1);
    vec4 newColor2 = texture2D(inputImageTexture2, texPos2);
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));

//    vec4 vCameraColor2 = texture2D(inputImageTexture2, TexCoord);
    FragColor = newColor;
//    gl_FragColor = vCameraColor2;

}
