precision mediump float;
uniform sampler2D inputImageTexture;
uniform sampler2D uLookupSampler;
varying vec2 textureCoordinate;
void main()
{
    vec4 vCameraColor = texture2D(inputImageTexture, textureCoordinate);
    float blueColor = vCameraColor.b * 15.0;
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 4.0);
    quad1.x = floor(blueColor) - (quad1.y * 4.0);

    vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 4.0);
    quad2.x = ceil(blueColor) - (quad2.y * 4.0);

    vec2 texPos1;
    texPos1.x = (quad1.x * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * vCameraColor.r);
    texPos1.y = (quad1.y * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * vCameraColor.g);

    vec2 texPos2;
    texPos2.x = (quad2.x * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * vCameraColor.r);
    texPos2.y = (quad2.y * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * vCameraColor.g);

    vec4 newColor1 = texture2D(uLookupSampler, texPos1);
    vec4 newColor2 = texture2D(uLookupSampler, texPos2);
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));

    gl_FragColor = mix(vCameraColor, vec4(newColor.rgb, vCameraColor.w), 1.0);
}
