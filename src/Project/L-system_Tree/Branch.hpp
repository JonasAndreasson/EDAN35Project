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


public:

	Branch(float, float, glm::vec3, float, glm::vec3);

};
