uniform sampler2D texture;
uniform vec4 color;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	pixel.r = (pixel.r + pixel.g + pixel.b) / 3.0 + color.r;
	pixel.g = (pixel.r + pixel.g + pixel.b) / 3.0 + color.g;
	pixel.b = (pixel.r + pixel.g + pixel.b) / 3.0 + color.b;

    gl_FragColor = pixel;
}