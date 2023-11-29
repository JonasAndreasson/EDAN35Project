#pragma once

#include "core/helpers.hpp"
#include "core/node.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct GenerationConfiguration
{
	float delta{0.2f}; //!< Angle in radians between the trees branches.
	float length{1.0f};      //!< Starting length of the trunk.
	float length_proportions{0.5f} //!< Proportion of following branches length compared to startinng length. Set between 0.1 and 0.9.
};


//! \brief Represents a tree
class Tree
{
public:
	//! \brief Default constructor for a tree.
	//!
	//! @param [in] shape Information about the geometry used to model the
	//!             tree (use CreateBranch?)
	//! @param [in] program Shader program used to render the tree (more details about it in assignment~3)
	//! @param [in] diffuse_texture_id Identifier of the diffuse texture
	//!             used (more details about it also in assignment~3)
	Tree(bonobo::mesh_data const& shape, GLuint const* program,
	              GLuint diffuse_texture_id);

	//! \brief Render this tree.
	//!
	//! @param [in] elapsed_time Amount of time (in microseconds) between
	//!             two frames
	//! @param [in] view_projection Matrix transforming from world space to
	//!             clip space
	//! @param [in] parent_transform Matrix transforming from the parent’s
	//!             local space to world space
	//! @param [in] show_basis Show a 3D basis transformed by the world matrix
	//!             of this tree
	//! @return Matrix transforming from this tree’s local space
	//!         to world space
	glm::mat4 render(std::chrono::microseconds elapsed_time,
	                 glm::mat4 const& view_projection,
	                 glm::mat4 const& parent_transform = glm::mat4(1.0f),
	                 bool show_basis = false);





	//! \brief Configure the scale of this tree.
	void set_scale(glm::vec3 const& scale);


	glm::vec3 get_pos();

private:
	struct {
		Node node;
		struct {
			float delta{ 0.2f };				//!< Angle in radians between the trees branches.
			float length{ 1.0f };				//!< Starting length of the trunk.
			float length_proportions{ 0.5f };	//!< Proportion of following branches length compared to startinng length. Set between 0.1 and 0.9.
		} orbit;
		glm::vec3 scale{1.0f};
		
	} _body;

	struct {
		Node node;
		glm::vec2 scale{1.0f};
		bool is_set{false};
	} _ring;

	
	glm::mat4 transformed;
	glm::mat4 S;
	glm::mat4 R_1s;
	glm::mat4 R_2s;
	glm::mat4 T_o;
	glm::mat4 R_1o;
	glm::mat4 R_2o;
	glm::mat4 world;
};
