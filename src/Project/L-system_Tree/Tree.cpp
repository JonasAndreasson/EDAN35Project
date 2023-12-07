#include "Tree.hpp"
#include "Branch.hpp"
#include "core/node.hpp"
#include <string.h>
#include <stdio.h>
#include <stack>
Tree::Tree(const std::string s, const GLuint* program, const std::function<void(GLuint)>& set_uniforms) :Node()

{	//Generate Tree using L-system -> generator
	//Empty until figured out if we want String or L-System-obj as param.
	// 
	//s = "F-F+F";
	if (s == "") return;
	float angle = 0;
	float height = 2;
	float radius = 0.5;
	float down_scaling = 0.7f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	std::stack<Branch*> stack;
	Branch *b = nullptr;
	for (auto c : s) {
		switch (c) {
		case 'F':
			//make branch
			
			b = new Branch(radius, height, position, angle, rotation, down_scaling,b);
			b->set_program(program, set_uniforms);
			add_child(b);
			position = b->get_end()+glm::vec3(0,height*0.05,0);
			b->r = radius;
			b->h = height;
			b->a = angle;
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
			height = b->h;
			position = b->get_end() + glm::vec3(0, height * 0.05, 0);
			radius = b->r;
			angle = b->a;
			radius *= down_scaling;
			height *= down_scaling;
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
