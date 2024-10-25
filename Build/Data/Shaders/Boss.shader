uniform sampler2D texture;
uniform sampler2D mask;

uniform vec4 color0;
uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;

void main()
{
    vec4 pixelTexture = texture2D(texture, gl_TexCoord[0].xy);
    vec4 pixelMask = texture2D(mask, gl_TexCoord[0].xy);

	vec4 color = pixelTexture;
	if (pixelMask == color0 || pixelMask == color1 || pixelMask == color2 || pixelMask == color3)
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 1.0;
		color.a = 1.0;
	}

    gl_FragColor = color;
}