#include "generate_world.hpp"
#include "parametric_shapes.hpp"
#include "core/helpers.hpp"
#include <config.hpp>
#include <core/Log.h>
#include "Tree.hpp"
#include <random>
std::vector<bonobo::mesh_data> gen_world::fetch_mesh(glm::vec3 sun_position) {
	//Create L -system
   /*
   * Using ´Fifo Stack.
	   "-" : turn left 45°
	   "+" : turn right 45°.
	   "F" : draw branch
	   "X" : Dummy command - do nothing, used for organic generation
	   "[" : push position
	   "]" : pop position  TODO:here we can add leaves?
   */


	LSystem fractalSys;
	fractalSys.AddAxiom('F', "F[-F][+F]");
	fractalSys.angle = 0.4485496;
	//using default values

	LSystem shrubby;
	shrubby.AddAxiom('F', "F[+F]F[-F][F]");
	shrubby.angle = 0.4485496;
	shrubby.radius = 0.005;
	shrubby.height = 0.06;
	shrubby.down_scaling = 0.9;
	shrubby.down_scaling_height = 0.9;

	LSystem treeSys1;
	treeSys1.AddAxiom('F', "F-[-F+F+F]+[+F-F-F]");
	treeSys1.angle = 0.3926991;
	//using default values

	LSystem treeSys2;
	treeSys2.AddAxiom('F', "F[-F+F+F][+F-F-F]");
	treeSys2.angle = 0.3926991;
	//using default values



// Load the geometry

	// Create the shader programs	
	GLuint ground_diff_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_diffuse_4k.jpg"));
	GLuint ground_disp_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_disp_4k.png"));
	GLuint ground_normal_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_nor_gl_4k.png"));

	GLuint sky_texture = bonobo::loadTexture2D(config::resources_path("textures/sky/industrial_sunset_puresky.jpg"));

	// --- Shaders done 

	float scene_radius = 100.0f;

	auto skybox_shape = parametric_shapes::createSphere(scene_radius, 100u, 100u, glm::vec3(0.0f));
	if (skybox_shape.vao == 0u) {
		LogError("Failed to retrieve the mesh for the skybox");

	}
	skybox_shape.bindings.emplace("diffuse_texture", sky_texture);
	skybox_shape.name = "Sky";


	//Node skybox;
	//skybox.set_geometry(skybox_shape);
	//skybox.add_texture("sky_texture", sky_texture, GL_TEXTURE_2D);
	//skybox.set_program(&skybox_shader);

	auto quad = parametric_shapes::createQuad(50, 50, 1000, 1000);
	quad.bindings.emplace("diffuse_texture", ground_diff_texture);
	quad.name = "Ground";

	//Node plane;
	//plane.set_geometry(quad);


	std::string s = treeSys1.ApplyAxioms("F", 3);
	GLuint tree_diff_texture = bonobo::loadTexture2D(config::resources_path("textures/BarkPoplar001_COL_4K.jpg"));
	if (tree_diff_texture == 0u) {
		std::cout << "Couldn't load tree_diff_texture";
	}
	//Tree tree = Tree(s, treeSys1, glm::vec3(10, 0, 10), 0u, [](GLuint) {}, tree_diff_texture);
	std::vector<Tree> trees;


	//
	// Creating meshes and adding to vector -----------------------------------------------------------------------------------------------------------------------------------------------------
	//

	std::vector<bonobo::mesh_data> scene_geometry;
	scene_geometry.push_back(skybox_shape);
	scene_geometry.push_back(quad);


	const int tree_count = 3;
	const int shrub_count = 2 * tree_count;



	auto sun = parametric_shapes::createSphere(10.0, 10u, 10u, sun_position);
	sun.material.opacity = 0;
	sun.name = "Sun";

	scene_geometry.push_back(sun);

	//Code base don https://stackoverflow.com/questions/26086128/generate-different-random-coordinates

	std::mt19937 eng; // using twister engine.

	std::random_device r;
	std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
	eng.seed(seed); // seed the bit generator, replaces srand()

	auto N = 50; //max distance (coordinate) from center..

	std::uniform_int_distribution<> dist(-N + 1, N - 1); // encapsulates the correct method of turning
	// random bits into random numbers in the range
	// [0, N)



//Create shrubs
	std::vector<glm::vec3> coordinates_shrubs; //Currently choosing coordinates seperately.. Makes it possible to maybe change trees but keep locations? idk why 
	for (int i = 0; i < shrub_count; i++) {
		int x = dist(eng); // the distribution internally used the engine like rand()
		int y = dist(eng);
		coordinates_shrubs.push_back(glm::vec3(x, 0, y));

	}

	std::string s_shrub = shrubby.ApplyAxioms("F", 5);

	for (int i = 0; i < shrub_count; i++) {
		//Tree t = Tree(s_shrub, shrubby, coordinates_shrubs[i], 0u, [](GLuint) {}, tree_diff_texture);
		Tree t = Tree(s_shrub, shrubby, glm::vec3(i * 10.0f, 0, i * 10.0f), 0u, [](GLuint) {}, tree_diff_texture);
		trees.push_back(t); //save to a vector for rendering here

		for (auto mesh : t.get_mesh()) {
			scene_geometry.push_back(mesh);

		}
	}
	//Create larger trees
	std::vector<glm::vec3> coordinates_trees;
	for (int i = 0; i < tree_count; i++) {
		int x = dist(eng); // the distribution internally used the engine like rand()
		int y = dist(eng);
		coordinates_trees.push_back(glm::vec3(x, 0, y));
	}

	std::string s_tree = treeSys1.ApplyAxioms("F", 3);
	for (int i = 0; i < tree_count; i++) {
		//Tree t = Tree(s_tree, treeSys1, coordinates_trees[i], 0u, [](GLuint) {}, tree_diff_texture);
		Tree t = Tree(s_tree, treeSys1, glm::vec3(3 + i * 10.0f, 0, 3 + i * 10.0f), 0u, [](GLuint) {}, tree_diff_texture);
		trees.push_back(t);//save to a vector for rendering here
		for (auto mesh : t.get_mesh()) {
			scene_geometry.push_back(mesh);
		}
	}

	return scene_geometry;
}
