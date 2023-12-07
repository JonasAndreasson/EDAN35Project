#include "LSystem.hpp"
// Defines the LSystem parameters and rule application. 

//Inspired by https://github.com/AidanMM/AMM-3DEngine/tree/master, https://www.csh.rit.edu/~aidan/portfolio/3DLSystems.shtml


//Iterator used to step through map. Our iterator is called 'ruleIter'
typedef std::map<std::string, std::string>::iterator ruleIter;

LSystem::LSystem()
{
}

// the axiom parameter is a map that stores the variabels and corrosponding rules.

//!< Add variable and its production rule to the axioms parameter.
//! Production rules are defined by  key -> rule
void LSystem::AddAxiom(std::string key, std::string rule)
{
	axioms[key] = rule;
}


void LSystem::ClearAxioms()
{
	axioms.clear();
}

//if no rule exists for a variable then the rule is " var -> var "

//!< Applies axiom rules to the input which defines the system state.
//!  returns a string representing the new system.
std::string LSystem::ApplyAxioms(std::string system)
{
	std::string newSystem = ""; //Create an empty string.

	for (int j = 0; j < system.length(); j++) { //run rules for each item from the input.

		bool noRule = true; // Used to see if we find a matching rule.

		// Use a custom iterator - defined at the top of file - to find fitting rule
		// The iterator goes over map pairs.(first is key, second is value)
		for (ruleIter rule = axioms.begin(); rule != axioms.end(); ++rule) { // map.end() happens if the key isn't in the map.

			if (false) { //(system[j] == rule->first) { //Compare input item to each key NOTE: char == string doesn't work

				noRule = false;
				newSystem += rule->second; //Put the rule value in place of input variabel.
				break;
			}
		}
		if (noRule) { //Looped over entire map, no rule found var -> var
			newSystem += system[j]; //keep same variable in its place.
		}
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




