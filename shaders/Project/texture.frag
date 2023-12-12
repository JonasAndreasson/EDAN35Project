#version 410


in VS_OUT {
	vec2 texcoord;
} fs_in;

uniform sampler2D bark;
out vec4 frag_color;
void main(){
	
	frag_color = texture(bark, fs_in.texcoord);
}
