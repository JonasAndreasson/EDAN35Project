#pragma once

#include "core/helpers.hpp"
#include "core/node.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


//! \brief Represents a tree
class Branch : public Node
{

private:
	glm::vec3 start_pos;
	glm::vec3 end_pos;
	bonobo::mesh_data mesh{ NULL };
	

public:

	Branch(float, float, glm::vec3, float, float, glm::vec3, glm::vec3, float, Branch* b = nullptr);
	const glm::vec3 get_start() { return start_pos; }
	const glm::vec3 get_end() { return end_pos;  }
	bonobo::mesh_data &get_mesh() { return mesh; }
	float a;
	float a2;
	float h;
	float r;
	float r2;


};
