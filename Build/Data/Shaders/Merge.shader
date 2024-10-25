uniform sampler2D texture;
uniform sampler2D copy;

void main() 
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	vec4 colorCopy = texture2D(copy, gl_TexCoord[0].xy);

	color.r = color.r + colorCopy.r;
	color.g = color.g + colorCopy.g;
	color.b = color.b + colorCopy.b;

	gl_FragColor = color;
}