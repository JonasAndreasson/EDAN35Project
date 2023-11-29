#include "LSystem.hpp"
// Defines the LSystem parameters and rule application. 

//Inspired by https://github.com/AidanMM/AMM-3DEngine/tree/master, https://www.csh.rit.edu/~aidan/portfolio/3DLSystems.shtml


LSystem::LSystem()
{
}

// the axiom parameter is a map that stores the variabels and corrosponding rules.

//!< Add variable and rule to the axioms parameter.
void LSystem::AddAxiom(char key, std::string rule)
{
	axioms[key] = rule;
}


void LSystem::ClearAxioms()
{
	axioms.clear()
}

//if no rule exists for a variable then the rule is " var -> var "

//!< Applies axiom rules to the input, returns a string representing the new system.
std::string LSystem::ApplyAxioms(std::string system)
{
	std::string newSystem = ""; //Create an empty string.

	for (int j=0; j < system.length())

	return newSystem;
}

//!< Applies axiom rules to the input for a certain amont of iterations, returns a string representing the new system.
std::string LSystem::ApplyAxioms(std::string system, int iterations)
{
	std::string newSystem = ""; //Create an empty string.


	return newSystem;
}




