#version 410

in vec2 texCoord0;

out vec4 color;

void main(void)
{
	//I think we wanna render everything black, except the sun. Which will be renedered as White.
	color = vec4(0,0,0,1);
	//The sun should be white.
}
