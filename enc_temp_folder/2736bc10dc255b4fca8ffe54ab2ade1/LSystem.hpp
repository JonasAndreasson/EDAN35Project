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
	std::map<std::string, std::string> axioms;


	//! \brief Add rules to the Axiom
	//! 
	void AddAxiom(std::string key, std::string rule);


	//! \brief Clear the axiom.
	void ClearAxioms();


	//! \brief Apply the rules to the system.
	std::string ApplyAxioms(std::string system);


	//! \brief Apply the rules to the system for up to *iterations*.
	std::string ApplyAxioms(std::string system, int iterations);
};
