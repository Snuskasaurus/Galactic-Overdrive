uniform sampler2D texture;
uniform int intensity;

void main() 
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	
	color.r = pow(color.r, intensity);
	color.g = pow(color.g, intensity);
	color.b = pow(color.b, intensity);

	gl_FragColor = color;
}