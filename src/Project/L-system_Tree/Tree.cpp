#include "Tree.hpp"
#include "Branch.hpp"
#include "core/node.hpp"
#include <string.h>
#include <stdio.h>
#include <stack>
#include <random>
#include "LSystem.hpp"

Tree::Tree(const std::string s, LSystem system, const glm::vec3 start_pos ,const GLuint* program, const std::function<void(GLuint)>& set_uniforms, const GLuint texture) :Node()

{	//Generate Tree using L-system -> generator
	//Empty until figured out if we want String or L-System-obj as param.
	// 
	//s = "F-F+F";
	if (s == "") return;


	float delta_angle = system.angle;
	float angle = 0;

	float height = system.height;
	float radius = system.radius;
	float down_scaling = system.down_scaling;
	float down_scaling_height = system.down_scaling_height;
	//float height = 2;
	//float radius = 0.5f;
	//float down_scaling = 0.6f;
	//float down_scaling_height = 0.8f;
	glm::vec3 position = start_pos;
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	std::stack<Branch*> stack;
	Branch *b = nullptr;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 3);
	auto randAngle = std::bind(distribution, generator);

	for (auto c : s) {

		switch (c) {
		case 'F':
			//make branch
			
			b = new Branch(radius, height, position, angle, rotation, down_scaling, b);
			if (program != 0u) {
				b->set_program(program, set_uniforms);
			}
			if (texture != 0u) {
			b->add_texture("diffuse_texture", texture, GL_TEXTURE_2D);
			bool could_insert = b->get_mesh().bindings.emplace("diffuse_texture", texture).second;
			std::cout << "Could insert: " << could_insert << '\n';
			std::cout << "Size is now: " << b->get_mesh().bindings.size() << '\n';
			
			}
			else {
				std::cout << "Texture not added" << '\n';
			}
			meshes.push_back(b->get_mesh());
			add_child(b);
			position = b->get_end()+glm::vec3(0,height*0.05,0);
			b->r = radius;
			b->h = height;
			b->a = angle;
			radius *= down_scaling;
			height *= down_scaling_height;
			break;
		case '-':
			angle -= delta_angle;

			//angle -= glm::quarter_pi<float>();

			//angle -= (float) (glm::quarter_pi<float>() / 2 * distribution(generator)); //pi/8 + rand 45deg gives range pi/8- 3pi/8
			//std::cout << "Angle:" << angle << '\n';
			break;
		case '+':
			angle += delta_angle;

			//angle += glm::quarter_pi<float>();
			
			//angle += (float) (glm::quarter_pi<float>() / 2 * distribution(generator)); //pi/8 + rand 45deg gives range pi/8- 3pi/8
			break;
		case '[':
			stack.push(b);
				break;
		case ']':
			//Branch done, here we can add leaves before jumping back to branch on stack
			b = stack.top();
			height = b->h;
			position = b->get_end() + glm::vec3(0, height * 0.05, 0);
			radius = b->r;
			angle = b->a;
			radius *= down_scaling;
			height *= down_scaling_height;
			stack.pop();
				continue;
		default:
			break;

		}
	}
}




glm::vec3 Tree::get_pos() {
	//return glm::vec3((transformed * glm::vec4(0.0f,0.0f,0.0f,1.0f)));
	return glm::vec3(0.0f);
}
