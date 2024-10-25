uniform sampler2D texture;
uniform vec4 color;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	pixel.a = pixel.a / 2;

    gl_FragColor = pixel;
}