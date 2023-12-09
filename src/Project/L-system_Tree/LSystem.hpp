#pragma once
#include "core/helpers.hpp"
#include "core/node.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <map>

//! \brief Represents the L-system.
class LSystem
{
public:
	//! \brief Default constructor for L-system.
	LSystem();

	//! map storing axiom data.
	std::map<char, std::string> axioms;

	//! best angle for this system
	float angle = glm::quarter_pi<float>();
	//! best starting branch height for this system
	float height = 2;
	//! best angle for this system
	float radius = 0.5f;
	//! best angle for this system
	float down_scaling = 0.6f;
	//! best angle for this system
	float down_scaling_height = 0.8f;

	//! \brief Add rules to the Axiom
	//! 
	void AddAxiom(char key, std::string rule);


	//! \brief Clear the axiom.
	void ClearAxioms();


	//! \brief Apply the rules to the system.
	std::string ApplyAxioms(std::string system);


	//! \brief Apply the rules to the system for up to *iterations*.
	std::string ApplyAxioms(std::string system, int iterations);
};
