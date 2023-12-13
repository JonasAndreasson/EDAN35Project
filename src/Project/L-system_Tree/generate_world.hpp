#pragma once

#include "core/helpers.hpp"
#include "core/node.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "LSystem.hpp"


//! \brief Represents a tree
namespace gen_world
{
	std::vector<bonobo::mesh_data> fetch_mesh(glm::vec3 sun_position);
}
