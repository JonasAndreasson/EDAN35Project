#include "Tree.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <stack>
#include "core/helpers.hpp"
#include "core/Log.h"

Tree::Tree(bonobo::mesh_data const& shape,
                             GLuint const* program,
                             GLuint diffuse_texture_id)



{	//Generate Tree using L-system -> generator

	_body.node.set_geometry(shape);
	_body.node.add_texture("diffuse_texture", diffuse_texture_id, GL_TEXTURE_2D);
	_body.node.set_program(program);
	
}

glm::mat4 Tree::render(std::chrono::microseconds elapsed_time,
                                glm::mat4 const& view_projection,
                                glm::mat4 const& parent_transform,
                                bool show_basis)
{
	// Convert the duration from microseconds to seconds.
	auto const elapsed_time_s = std::chrono::duration<float>(elapsed_time).count();
	// If a different ratio was needed, for example a duration in
	// milliseconds, the following would have been used:
	// auto const elapsed_time_ms = std::chrono::duration<float, std::milli>(elapsed_time).count();
	

	if (show_basis)
	{
		bonobo::renderBasis(1.0f, 2.0f, view_projection, world);
	}
	if (_ring.is_set) {
		
	}

	// Note: The second argument of `node::render()` is supposed to be the
	// parent transform of the node, not the whole world matrix, as the
	// node internally manages its local transforms. However in our case we
	// manage all the local transforms ourselves, so the internal transform
	// of the node is just the identity matrix and we can forward the whole
	// world matrix.
	_body.node.render(view_projection, world);

	return transformed;
}


void Tree::set_scale(glm::vec3 const& scale)
{
	_body.scale = scale;
}



glm::vec3 Tree::get_pos() {
	return glm::vec3((transformed * glm::vec4(0.0f,0.0f,0.0f,1.0f)));
}
