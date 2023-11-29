#include "LSystem.hpp"
// Defines the LSystem parameters and rule application. 

//Inspired by https://github.com/AidanMM/AMM-3DEngine/tree/master, https://www.csh.rit.edu/~aidan/portfolio/3DLSystems.shtml


LSystem::LSystem()
{
}

// the axiom parameter is a map that stores the variabels and corrosponding rules.

//!< Add variable and its production rule to the axioms parameter.
//! Production rules are defined by  key -> rule
void LSystem::AddAxiom(char key, std::string rule)
{
	axioms[key] = rule;
}


void LSystem::ClearAxioms()
{
	axioms.clear()
}

//if no rule exists for a variable then the rule is " var -> var "

//!< Applies axiom rules to the input which defines the system state.
//!  returns a string representing the new system.
std::string LSystem::ApplyAxioms(std::string system)
{
	std::string newSystem = ""; //Create an empty string.

	for (int j = 0; j < system.length(), j++) { //run rules for each item from the input.


	}

	return newSystem;
}

//!< Applies axiom rules to the input and then the result... for *iterations, returns a string representing the new system.
std::string LSystem::ApplyAxioms(std::string system, int iter)
{
	//Check basecase
	if (iter <= 0) { return system; }


	std::string newSystem = system; //

	//Apply rules *iter* many times
	for (int k = 0; k < iter; k++) {

		newSystem = ApplyAxioms(newSystem);
	}

	return newSystem;
}




