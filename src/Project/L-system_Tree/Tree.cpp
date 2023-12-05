#include "Tree.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <stack>
#include "core/helpers.hpp"
#include "core/Log.h"


Tree::Tree()



{	//Generate Tree using L-system -> generator
	//Empty until figured out if we want String or L-System-obj as param.
	std::string s = "";
	for (char c : s) {
		switch (c) {
		case "F":
			break;
		case "-":
			break;
		case "+":
			break;
		case "[":
				break;
		case "]":
				continue;
		default:
			break;

		}
	}
}


glm::vec3 Tree::get_pos() {
	return glm::vec3((transformed * glm::vec4(0.0f,0.0f,0.0f,1.0f)));
}
