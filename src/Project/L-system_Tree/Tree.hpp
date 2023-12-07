#pragma once

#include "core/helpers.hpp"
#include "core/node.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


//! \brief Represents a tree
class Tree : public Node
{

private:



public:
	//! \brief Default constructor for a tree.
	//!
	//! @param [in] shape Information about the geometry used to model the
	//!             tree (use CreateBranch?)
	//! @param [in] program Shader program used to render the tree (more details about it in assignment~3)
	//! @param [in] diffuse_texture_id Identifier of the diffuse texture
	//!             used (more details about it also in assignment~3)
	Tree(const std::string s, const GLuint* program, const std::function<void(GLuint)>& set_uniforms = [](GLuint) {});

	//! \brief Configure the scale of this tree. ??

	glm::vec3 get_pos();

};
