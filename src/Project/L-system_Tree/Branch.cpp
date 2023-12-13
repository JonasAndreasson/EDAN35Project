#include "Branch.hpp"
#include "parametric_shapes.hpp"
#include <stdio.h>
Branch::Branch(float radius, float height, glm::vec3 position, float angle1, float angle2, glm::vec3 rotation, glm::vec3 rotation2, float down_scaling, Branch* b) : Node(){
	//setup mesh, rotate, move
	if (b != nullptr) {
		mesh = parametric_shapes::createBranch(radius, height, position, angle1, angle2, rotation, rotation2, down_scaling, &b->get_mesh());
	}
	else {
		mesh = parametric_shapes::createBranch(radius, height, position, angle1, angle2, rotation, rotation2, down_scaling);
	}
	set_geometry(mesh);
	//get_transform().SetRotate(angle, rotation);
	//get_transform().SetTranslate(position);
	//calculate the positions.
	start_pos = position;
	end_pos = position + glm::vec3((glm::rotate(glm::mat4(1.0f), angle1, rotation) * glm::rotate(glm::mat4(1.0f), angle2, rotation2))* glm::vec4(0, height, 0, 0)); 
	//std::cout << "Start pos:" << start_pos << '\n';
	//std::cout << "End pos:" << end_pos << '\n';
}
