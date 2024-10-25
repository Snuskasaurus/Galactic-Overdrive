uniform sampler2D texture;
uniform vec4 newColor;

void main()
{
	//Gets textures' pixel...
	vec4 color;
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (pixel == vec4(1.0f, 0.0f, 1.0f, 1.0f))
	{
		//Apply new color...
		color = newColor;
	}
	else
	{
		color = pixel;
	}
	gl_FragColor = color;
}