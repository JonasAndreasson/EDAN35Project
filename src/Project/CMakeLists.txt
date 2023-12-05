add_executable (EDAN35_Project)

target_sources (
	EDAN35_Project
	PRIVATE
		[[project_main.hpp]]
		[[project_main.cpp]]
)

target_link_libraries (EDAN35_Project PRIVATE assignment_setup parametric_tree LSystem)

add_subdirectory ("${CMAKE_SOURCE_DIR}/src/Project/L-system_Tree")

install (TARGETS EDAN35_Project DESTINATION bin)

copy_dlls (EDAN35_Project "${CMAKE_CURRENT_BINARY_DIR}")
