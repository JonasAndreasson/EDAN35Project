#include "Branch.hpp"
#include "parametric_shapes.hpp"
Branch::Branch(float radius, float height, glm::vec3 position, float angle, glm::vec3 rotation) : Node(){
	//setup mesh, rotate, move
	auto const mesh = parametric_shapes::createBranch(radius, height);
	set_geometry(mesh);
	get_transform().SetRotate(angle, rotation);
	get_transform().SetTranslate(position);
	//calculate the positions.
	start_pos = position;
	end_pos = position+glm::vec3(0,height,0)*get_transform().GetRotation(); //multiply with
}
