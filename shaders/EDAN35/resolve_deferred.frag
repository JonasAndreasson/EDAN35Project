#version 410

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D light_d_texture;
uniform sampler2D light_s_texture;
uniform sampler2D godray_texture;
layout (pixel_center_integer) in vec4 gl_FragCoord;
uniform vec2 inverse_screen_resolution;
uniform vec3 sun_position;
out vec4 frag_color;
const float exposure = 0.3f;
const float decay = 0.96815;
const float density  = 0.926;
const float weight  = 0.587;
void main()
{
	ivec2 pixel_coord = ivec2(gl_FragCoord.xy);
	vec2 texcoords;
	texcoords.x = gl_FragCoord.x * inverse_screen_resolution.x;
	texcoords.y = gl_FragCoord.y * inverse_screen_resolution.y;
	vec3 diffuse  = texelFetch(diffuse_texture,  pixel_coord, 0).rgb;
	vec3 specular = texelFetch(specular_texture, pixel_coord, 0).rgb;

	vec3 light_d  = texelFetch(light_d_texture,  pixel_coord, 0).rgb;
	vec3 light_s  = texelFetch(light_s_texture,  pixel_coord, 0).rgb;
	const vec3 ambient = vec3(0.15);

	frag_color =  vec4((ambient + light_d) * diffuse + light_s * specular, 1.0);
	int NUM_SAMPLES = 40;
	vec2 tc = texcoords;
	vec2 deltatexCoord = (tc - (sun_position.xy*0.5 + 0.5));
	deltatexCoord *= 1.0/ float(NUM_SAMPLES);
	float illuminationDecay = 1.0f;

	vec4 godRayColor = texture(godray_texture , tc.xy)*0.4;
	for(int i = 0 ; i< NUM_SAMPLES ; i++)
	{
		tc-= deltatexCoord;
		vec4 samp = texture(godray_texture , tc )*0.4;
		samp *= illuminationDecay*weight;
		godRayColor += samp;
		illuminationDecay *= decay;
	}

	//frag_color = ((vec4((vec3(godRayColor.r, godRayColor.g, godRayColor.b) * exposure), 1)) + (frag_color*(1.0)));
}
