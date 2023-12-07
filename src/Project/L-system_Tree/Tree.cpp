#include "Tree.hpp"
#include "Branch.hpp"
#include "core/node.hpp"
#include <string.h>
#include <stdio.h>
#include <stack>
Tree::Tree(const GLuint* program, const std::function<void(GLuint)>& set_uniforms) :Node()

{	//Generate Tree using L-system -> generator
	//Empty until figured out if we want String or L-System-obj as param.
	std::string s = "F-F-F++F+F";
	s = "F-F+F";
	float angle = 0;
	float height = 2;
	float radius = 0.5;
	float down_scaling = 0.5f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	std::stack<Branch*> stack;
	Branch *b = nullptr;
	for (auto c : s) {
		switch (c) {
		case 'F':
			//make branch
			if (true){//(b == nullptr) {
				b = new Branch(radius, height, position, angle, rotation, down_scaling);
				b->set_program(program, set_uniforms);
			}
			else {

				//Branch *newb = new Branch(radius, height, position, angle, rotation, down_scaling, b);
				//b = newb;
				//b->set_program(program, set_uniforms);
			}
			add_child(b);
			position = b->get_end();
			radius *= down_scaling;
			height *= down_scaling;
			break;
		case '-':
			angle -= glm::quarter_pi<float>();
			break;
		case '+':
			angle += glm::quarter_pi<float>();
			break;
		case '[':
			stack.push(b);
				break;
		case ']':
			b = stack.top();
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
