uniform sampler2D texture;

void main()
{
	// Récupère le pixel dans la texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (pixel.a > 0)
	{
		pixel = vec4(30.0 / 255.0, 30.0 / 255.0, 30.0 / 255.0, 0.26);
	}

	gl_FragColor = pixel;
}