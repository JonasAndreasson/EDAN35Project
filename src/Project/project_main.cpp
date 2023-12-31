#include "project_main.hpp"

#include "L-system_Tree/parametric_shapes.hpp"
#include "L-system_Tree/LSystem.hpp"
#include "L-system_Tree/Branch.hpp"
#include "config.hpp"
#include "core/Bonobo.h"
#include "core/FPSCamera.h"
#include "core/node.hpp"
#include "core/ShaderProgramManager.hpp"
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cmath>
#include <clocale>
#include <cstdlib>
#include <stdexcept>
#include "L-system_Tree/Tree.hpp"
#include <random>



project::ProjectMain::ProjectMain(WindowManager& windowManager) :
	mCamera(0.5f * glm::half_pi<float>(),
	        static_cast<float>(config::resolution_x) / static_cast<float>(config::resolution_y),
	        0.01f, 1000.0f),
	inputHandler(), mWindowManager(windowManager), window(nullptr)
{
	WindowManager::WindowDatum window_datum{ inputHandler, mCamera, config::resolution_x, config::resolution_y, 0, 0, 0, 0};

	window = mWindowManager.CreateGLFWWindow("project: Assignment 2", window_datum, config::msaa_rate);
	if (window == nullptr) {
		throw std::runtime_error("Failed to get a window: aborting!");
	}

	bonobo::init();
}

project::ProjectMain::~ProjectMain()
{
	bonobo::deinit();
}




void project::ProjectMain::run()
{

	
	 //Create L -system
		/*
		* Using �Fifo Stack.
			"-" : turn left 45�
			"+" : turn right 45�.
			"F" : draw branch
			"X" : Dummy command - do nothing, used for organic generation
			"[" : push position
			"]" : pop position  TODO:here we can add leaves?
		*/

		glm::vec3 sun_position = glm::vec3(-62.003f, 11.122f, -78.046f);
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
	auto const shape = parametric_shapes::createBranch(0.05f, 0.3f);
	auto const branch2 = parametric_shapes::createBranch(0.025f, 0.15f);
	if (shape.vao == 0u)
		return;

	// Set up the camera
	mCamera.mWorld.SetTranslate(glm::vec3(0.0f, 2.0f, 0.0f));
	mCamera.mMouseSensitivity = glm::vec2(0.003f);
	mCamera.mMovementSpeed = glm::vec3(3.0f); // 3 m/s => 10.8 km/h
	auto camera_position = mCamera.mWorld.GetTranslation();

	
	// Create the shader programs
	ShaderProgramManager program_manager;
	GLuint fallback_shader = 0u;
	program_manager.CreateAndRegisterProgram("Fallback",
	                                         { { ShaderType::vertex, "common/fallback.vert" },
	                                           { ShaderType::fragment, "common/fallback.frag" } },
	                                         fallback_shader);
	if (fallback_shader == 0u) {
		LogError("Failed to load fallback shader");
		return;
	}

		//Sampler
	auto sampl = [](GLuint a) {
		glSamplerParameteri(a, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	};

	GLuint sampler = bonobo::createSampler(sampl);
		

		// Skybox 
	GLuint skybox_shader = 0u;
	program_manager.CreateAndRegisterProgram("Skybox",
		{ { ShaderType::vertex, "project/skybox.vert" },
		  { ShaderType::fragment, "project/skybox.frag" } },
		skybox_shader);
	if ((skybox_shader) == 0u)
		LogError("Failed to load skybox shader");
	

		//Diffuse, Normal, Tangent, Binormal, teexcoord - These we can maybe remove/Replace with whatever from deffered shading..

	GLuint diffuse_shader = 0u;
	program_manager.CreateAndRegisterProgram("Diffuse",
	                                         { { ShaderType::vertex, "EDAF80/diffuse.vert" },
	                                           { ShaderType::fragment, "EDAF80/diffuse.frag" } },
	                                         diffuse_shader);
	if (diffuse_shader == 0u)
		LogError("Failed to load diffuse shader");

	GLuint normal_shader = 0u;
	program_manager.CreateAndRegisterProgram("Normal",
	                                         { { ShaderType::vertex, "EDAF80/normal.vert" },
	                                           { ShaderType::fragment, "EDAF80/normal.frag" } },
	                                         normal_shader);
	if (normal_shader == 0u)
		LogError("Failed to load normal shader");

	GLuint tangent_shader = 0u;
	program_manager.CreateAndRegisterProgram("Tangent",
	                                         { { ShaderType::vertex, "EDAF80/tangent.vert" },
	                                           { ShaderType::fragment, "EDAF80/tangent.frag" } },
	                                         tangent_shader);
	if (tangent_shader == 0u)
		LogError("Failed to load tangent shader");

	GLuint binormal_shader = 0u;
	program_manager.CreateAndRegisterProgram("Bitangent",
	                                         { { ShaderType::vertex, "EDAF80/binormal.vert" },
	                                           { ShaderType::fragment, "EDAF80/binormal.frag" } },
	                                         binormal_shader);
	if (binormal_shader == 0u)
		LogError("Failed to load binormal shader");

	GLuint texcoord_shader = 0u;
	program_manager.CreateAndRegisterProgram("Texture coords",
	                                         { { ShaderType::vertex, "EDAF80/texcoord.vert" },
	                                           { ShaderType::fragment, "EDAF80/texcoord.frag" } },
	                                         texcoord_shader);
	if (texcoord_shader == 0u)
		LogError("Failed to load texcoord shader");



	GLuint phong_shader = 0u;
	program_manager.CreateAndRegisterProgram("Phong",
		{ { ShaderType::vertex, "project/phong.vert" },
		  { ShaderType::fragment, "project/phong.frag" } },
		phong_shader);

	if ((phong_shader) == 0u)
		LogError("Failed to load phong shader");


	
	auto const light_position = glm::vec3(-2.0f, 4.0f, 2.0f);
	auto const set_uniforms = [&light_position](GLuint program){
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
	};
	bool use_normal_mapping = false;
	auto const phong_set_uniforms = [&use_normal_mapping, &light_position, &camera_position](GLuint program) {
		glUniform1i(glGetUniformLocation(program, "use_normal_mapping"), use_normal_mapping ? 1 : 0);
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
		glUniform3fv(glGetUniformLocation(program, "camera_position"), 1, glm::value_ptr(camera_position));
	};
	
		// Texture shader used for tree-bark, sky, any objects...
	GLuint texture_shader = 0u;
	program_manager.CreateAndRegisterProgram("texture",
		{ { ShaderType::vertex, "project/texture.vert" },
		  { ShaderType::fragment, "project/texture.frag" } },
		texture_shader);
	if (texture_shader == 0u)
		LogError("Failed to load texture shader");


	
	auto const bark_uniforms = [&light_position](GLuint program) {
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
		};


	/*GLuint cubemap = bonobo::loadTextureCubeMap(
		config::resources_path("cubemaps/NissiBeach2/posx.jpg"),
		config::resources_path("cubemaps/NissiBeach2/negx.jpg"),
		config::resources_path("cubemaps/NissiBeach2/posy.jpg"),
		config::resources_path("cubemaps/NissiBeach2/negy.jpg"),
		config::resources_path("cubemaps/NissiBeach2/posz.jpg"),
		config::resources_path("cubemaps/NissiBeach2/negz.jpg"));*/
	GLuint ground_diff_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_diffuse_4k.jpg"));
	GLuint ground_disp_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_disp_4k.png"));
	GLuint ground_normal_texture = bonobo::loadTexture2D(config::resources_path("textures/Ground/forest_leaves_02_nor_gl_4k.png"));

	GLuint sky_texture = bonobo::loadTexture2D(config::resources_path("textures/sky/industrial_sunset_puresky.jpg"));
	
	// --- Shaders done 

	float scene_radius = 100.0f;

	auto skybox_shape = parametric_shapes::createSphere(scene_radius, 100u, 100u,glm::vec3(0.0f));
	if (skybox_shape.vao == 0u) {
		LogError("Failed to retrieve the mesh for the skybox");
		return;
	}
	skybox_shape.bindings.emplace("diffuse_texture", sky_texture);



	Node skybox;
	skybox.set_geometry(skybox_shape);
	//skybox.add_texture("sky_texture", sky_texture, GL_TEXTURE_2D);
	skybox.set_program(&skybox_shader);

	auto quad = parametric_shapes::createQuad(50, 50, 1000, 1000);
	quad.bindings.emplace("diffuse_texture", ground_diff_texture);


	Node plane;
	plane.set_geometry(quad);
	plane.get_transform().SetTranslate(glm::vec3(0.0, 0, 0.0));
	plane.set_program(&texture_shader);
	//plane.add_texture("diff_texture", ground_diff_texture, GL_TEXTURE_2D);
	//plane.add_texture("normal_texture", ground_normal_texture, GL_TEXTURE_2D);


	std::string s = treeSys1.ApplyAxioms("F", 3); 
	//std::cout << s << '\n';
	GLuint tree_diff_texture = bonobo::loadTexture2D(config::resources_path("textures/BarkPoplar001_COL_4K.jpg"));
	if (tree_diff_texture == 0u) {
		std::cout << "Couldn't load tree_diff_texture";
	}
	Tree tree = Tree(s, treeSys1, glm::vec3(10, 0, 10),&texture_shader, bark_uniforms, tree_diff_texture);
	std::vector<Tree> trees;
	//circle_rings.set_geometry(shape);
	//circle_rings.set_program(&normal_shader, set_uniforms);
	//TRSTransformf& circle_rings_transform_ref = circle_rings.get_transform();
	//second_branch.set_program(&normal_shader, set_uniforms);
	

	//
	// Creating meshes and adding to vector -----------------------------------------------------------------------------------------------------------------------------------------------------
	//

	std::vector<bonobo::mesh_data> scene_geometry;
	scene_geometry.push_back(skybox_shape);
	scene_geometry.push_back(quad);


	const int tree_count = 3;
	const int shrub_count = 2 * tree_count;



	auto sun = parametric_shapes::createSphere(100.0, 10u, 10u, sun_position);
	sun.material.opacity = 0;
	sun.name = "Sun";

	scene_geometry.push_back(sun);

	//Code base don https://stackoverflow.com/questions/26086128/generate-different-random-coordinates

	std::mt19937 eng; // using twister engine.

	std::random_device r;
	std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
	eng.seed(seed); // seed the bit generator, replaces srand()

	auto N = 50; //max distance (coordinate) from center..

	std::uniform_int_distribution<> dist(-N+1 , N - 1); // encapsulates the correct method of turning
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
		Tree t = Tree(s_shrub, shrubby, glm::vec3(i*10.0f, 0, i*10.0f), &texture_shader, bark_uniforms, tree_diff_texture);
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
		Tree t = Tree(s_tree, treeSys1, glm::vec3(3 + i * 10.0f, 0, 3 + i *10.0f), &texture_shader, bark_uniforms, tree_diff_texture);
		trees.push_back(t);//save to a vector for rendering here
		for (auto mesh : t.get_mesh()) {
			scene_geometry.push_back(mesh);
		}
	}
	
	//return scene_geometry; //TODO: Fick inte till att �ndra void vi kanske vill ha detta n�n annastans?


	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	glClearDepthf(1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);




	auto lastTime = std::chrono::high_resolution_clock::now();

	std::int32_t program_index = 0;
	float elapsed_time_s = 0.0f;
	auto cull_mode = bonobo::cull_mode_t::disabled;
	auto polygon_mode = bonobo::polygon_mode_t::fill;
	bool show_logs = true;
	bool show_gui = true;
	bool show_basis = false;
	float basis_thickness_scale = 1.0f;
	float basis_length_scale = 1.0f;
	float* res = new float(0);
	float last_second = 0;
	changeCullMode(cull_mode);

	while (!glfwWindowShouldClose(window)) {
		//Used to find current location
		//std::cout << mCamera.mWorld.GetTranslation() << '\n';
		auto const nowTime = std::chrono::high_resolution_clock::now();
		auto const deltaTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(nowTime - lastTime);
		lastTime = nowTime;

		auto& io = ImGui::GetIO();
		inputHandler.SetUICapture(io.WantCaptureMouse, io.WantCaptureKeyboard);

		glfwPollEvents();
		inputHandler.Advance();
		mCamera.Update(deltaTimeUs, inputHandler);
		elapsed_time_s += std::chrono::duration<float>(deltaTimeUs).count();

		if (inputHandler.GetKeycodeState(GLFW_KEY_F3) & JUST_RELEASED)
			show_logs = !show_logs;
		if (inputHandler.GetKeycodeState(GLFW_KEY_F2) & JUST_RELEASED)
			show_gui = !show_gui;
		if (inputHandler.GetKeycodeState(GLFW_KEY_F11) & JUST_RELEASED)
			mWindowManager.ToggleFullscreenStatusForWindow(window);


		// Retrieve the actual framebuffer size: for HiDPI monitors,
		// you might end up with a framebuffer larger than what you
		// actually asked for. For example, if you ask for a 1920x1080
		// framebuffer, you might get a 3840x2160 one instead.
		// Also it might change as the user drags the window between
		// monitors with different DPIs, or if the fullscreen status is
		// being toggled.
		int framebuffer_width, framebuffer_height;
		glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
		glViewport(0, 0, framebuffer_width, framebuffer_height);

		mWindowManager.NewImGuiFrame();


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		bonobo::changePolygonMode(polygon_mode);

		// Render background scene
		glDisable(GL_DEPTH_TEST);
		skybox.get_transform().SetTranslate(camera_position);
		skybox.render(mCamera.GetWorldToClipMatrix());
		glEnable(GL_DEPTH_TEST);
		plane.render(mCamera.GetWorldToClipMatrix());

	

		//circle_rings.render(mCamera.GetWorldToClipMatrix());
		//second_branch.render(mCamera.GetWorldToClipMatrix());
		//tree.render(mCamera.GetWorldToClipMatrix());
		for(auto& tree: trees){
			
			for (size_t i = 0; i < tree.get_children_nb(); i += 1) {
				tree.get_child(i)->render(mCamera.GetWorldToClipMatrix());
			}
		}

		

		

		bool const opened = ImGui::Begin("Scene Controls", nullptr, ImGuiWindowFlags_None);
		if (opened) {
			auto const cull_mode_changed = bonobo::uiSelectCullMode("Cull mode", cull_mode);
			if (cull_mode_changed) {
				changeCullMode(cull_mode);
			}
			bonobo::uiSelectPolygonMode("Polygon mode", polygon_mode);

			auto selection_result = program_manager.SelectProgram("Shader", program_index);
			if (selection_result.was_selection_changed) {
				//circle_rings.set_program(selection_result.program, set_uniforms);
				//second_branch.set_program(selection_result.program, set_uniforms);
			}
			ImGui::Separator();
			ImGui::Separator();
			ImGui::Checkbox("Show basis", &show_basis);
			ImGui::SliderFloat("Basis thickness scale", &basis_thickness_scale, 0.0f, 100.0f);
			ImGui::SliderFloat("Basis length scale", &basis_length_scale, 0.0f, 100.0f);
		}
		ImGui::End();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (show_basis)
			bonobo::renderBasis(basis_thickness_scale, basis_length_scale, mCamera.GetWorldToClipMatrix());
		if (show_logs)
			Log::View::Render();
		mWindowManager.RenderImGuiFrame(show_gui);

		glfwSwapBuffers(window);
	}
}

int main()
{
	std::setlocale(LC_ALL, "");

	Bonobo framework;


	try {
		project::ProjectMain projectMain(framework.GetWindowManager());
		projectMain.run();
	} catch (std::runtime_error const& e) {
		LogError(e.what());
	}
}
