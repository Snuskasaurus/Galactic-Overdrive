uniform sampler2D texture;
uniform vec4 cockpitColor;
uniform vec4 textureColor;
uniform int seeThrough;
uniform float time;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	// If the color is the purple, the pixel will be replaced by the color of the players health.
	if (pixel == vec4(1.0, 0.0, 1.0, 1.0))
	{
		pixel = cockpitColor;
		pixel.r += abs(sin(time * 16.0));
		pixel.g += abs(sin(time * 16.0));
		pixel.b += abs(sin(time * 16.0));
	}
	// Else, the color will be the color of the default spaceship and the selected color
	else
	{
		float minValue = 90.0 / 255.0;
		float maxValue = 200.0 / 255.0;
		if(pixel.r > minValue && pixel.r < maxValue)
		{
			if(pixel.g > minValue && pixel.g < maxValue)
			{
				if(pixel.b > minValue && pixel.b < maxValue)
				{
					pixel.r = (pixel.g * textureColor.r);
					pixel.g = (pixel.g * textureColor.g);
					pixel.b = (pixel.b * textureColor.b);
				}
			}
		}
	}

	if (seeThrough == 1)
	{
		pixel.a = pixel.a / 2.0;
	}

    gl_FragColor =  pixel;
}