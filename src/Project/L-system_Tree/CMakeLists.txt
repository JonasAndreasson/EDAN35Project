add_library (parametric_tree STATIC)
target_sources (
       parametric_tree
       PUBLIC [[parametric_shapes.hpp]]
       PRIVATE [[parametric_shapes.cpp]])
target_link_libraries (parametric_tree PRIVATE bonobo CG_Labs_options)

add_library (LSystem STATIC)
target_sources (
       LSystem
       PUBLIC [[LSystem.hpp]]
       PRIVATE [[LSystem.cpp]])
target_link_libraries (LSystem PRIVATE bonobo CG_Labs_options)
