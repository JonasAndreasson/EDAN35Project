#include "Branch.hpp"
#include "parametric_shapes.hpp"
#include <stdio.h>
Branch::Branch(float radius, float height, glm::vec3 position, float angle, glm::vec3 rotation, float down_scaling) : Node(){
	//setup mesh, rotate, move
	auto const mesh = parametric_shapes::createBranch(radius, height,angle,rotation,down_scaling);
	set_geometry(mesh);
	//get_transform().SetRotate(angle, rotation);
	get_transform().SetTranslate(position);
	//calculate the positions.
	start_pos = position;
	end_pos = position+glm::vec3((glm::vec4(0,height,0,0)*glm::rotate(glm::mat4(1.0f), angle, rotation))); //multiply with
	std::cout << "Start pos:" << start_pos << '\n';
	std::cout << "End pos:" << end_pos << '\n';
}
