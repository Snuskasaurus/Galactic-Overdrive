uniform sampler2D texture;
uniform float width;
uniform float height;
uniform float intensity;

void main() 
{
	vec2 blurrRadius;
	vec2 position = gl_TexCoord[0].xy;
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);

	blurrRadius.x = width * intensity;
	blurrRadius.y = height * intensity;

	color += texture2D(texture, vec2(position.x, position.y - 4.0 * blurrRadius.y)) * 0.05;
	color += texture2D(texture, vec2(position.x, position.y - 3.0 * blurrRadius.y)) * 0.09;
	color += texture2D(texture, vec2(position.x, position.y - 2.0 * blurrRadius.y)) * 0.12;
	color += texture2D(texture, vec2(position.x, position.y - blurrRadius.y)) * 0.15;
								
	color += texture2D(texture, vec2(position.x - 4.0 * blurrRadius.x, position.y)) * 0.05;
	color += texture2D(texture, vec2(position.x - 3.0 * blurrRadius.x, position.y)) * 0.09;
	color += texture2D(texture, vec2(position.x - 2.0 * blurrRadius.x, position.y)) * 0.12;
	color += texture2D(texture, vec2(position.x - blurrRadius.x, position.y)) * 0.15;

	/*color += texture2D(texture, vec2(position.x - 4.0 * blurrRadius.x, position.y - 4.0 * blurrRadius.y)) * 0.05;
	color += texture2D(texture, vec2(position.x - 3.0 * blurrRadius.x, position.y - 3.0 * blurrRadius.y)) * 0.09;
	color += texture2D(texture, vec2(position.x - 2.0 * blurrRadius.x, position.y - 2.0 * blurrRadius.y)) * 0.12;
	color += texture2D(texture, vec2(position.x - blurrRadius.x, position.y - blurrRadius.y)) * 0.15;*/
								
	color += texture2D(texture, vec2(position.x, position.y)) * 0.16;
								
	color += texture2D(texture, vec2(position.x, position.y + blurrRadius)) * 0.15;
	color += texture2D(texture, vec2(position.x, position.y + 2.0 * blurrRadius.y)) * 0.12;
	color += texture2D(texture, vec2(position.x, position.y + 3.0 * blurrRadius.y)) * 0.09;
	color += texture2D(texture, vec2(position.x, position.y + 4.0 * blurrRadius.y)) * 0.05;
								
	color += texture2D(texture, vec2(position.x + blurrRadius.x, position.y)) * 0.15;
	color += texture2D(texture, vec2(position.x + 2.0 * blurrRadius.x, position.y)) * 0.12;
	color += texture2D(texture, vec2(position.x + 3.0 * blurrRadius.x, position.y)) * 0.09;
	color += texture2D(texture, vec2(position.x + 4.0 * blurrRadius.x, position.y)) * 0.05;

	/*color += texture2D(texture, vec2(position.x + blurrRadius.x, position.y + blurrRadius.y)) * 0.15;
	color += texture2D(texture, vec2(position.x + 2.0 * blurrRadius.x, position.y + 2.0 * blurrRadius.y)) * 0.12;
	color += texture2D(texture, vec2(position.x + 3.0 * blurrRadius.x, position.y + 3.0 * blurrRadius.y)) * 0.09;
	color += texture2D(texture, vec2(position.x + 4.0 * blurrRadius.x, position.y + 4.0 * blurrRadius.y)) * 0.05;*/

	gl_FragColor = color;
}