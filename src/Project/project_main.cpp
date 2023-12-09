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


void
project::ProjectMain::run()
{

	
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

		LSystem treeSys2;
		treeSys2.AddAxiom('F', "F[-F+F+F][+F-F-F]");
		treeSys2.angle = 0.3926991;
	


	// Load the geometry
	auto const shape = parametric_shapes::createBranch(0.05f, 0.3f);
	auto const branch2 = parametric_shapes::createBranch(0.025f, 0.15f);
	if (shape.vao == 0u)
		return;

	// Set up the camera
	mCamera.mWorld.SetTranslate(glm::vec3(0.0f, 1.0f, 1.0f));
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
	

		//Diffuse, Normal, Tangent, Binormal, teexcoord - These we can maybe remove??
	GLuint diffuse_shader = 0u;
	program_manager.CreateAndRegisterProgram("Diffuse",
	                                         { { ShaderType::vertex, "project/diffuse.vert" },
	                                           { ShaderType::fragment, "project/diffuse.frag" } },
	                                         diffuse_shader);
	if (diffuse_shader == 0u)
		LogError("Failed to load diffuse shader");

	GLuint normal_shader = 0u;
	program_manager.CreateAndRegisterProgram("Normal",
	                                         { { ShaderType::vertex, "project/normal.vert" },
	                                           { ShaderType::fragment, "project/normal.frag" } },
	                                         normal_shader);
	if (normal_shader == 0u)
		LogError("Failed to load normal shader");

	GLuint tangent_shader = 0u;
	program_manager.CreateAndRegisterProgram("Tangent",
	                                         { { ShaderType::vertex, "project/tangent.vert" },
	                                           { ShaderType::fragment, "project/tangent.frag" } },
	                                         tangent_shader);
	if (tangent_shader == 0u)
		LogError("Failed to load tangent shader");

	GLuint binormal_shader = 0u;
	program_manager.CreateAndRegisterProgram("Bitangent",
	                                         { { ShaderType::vertex, "project/binormal.vert" },
	                                           { ShaderType::fragment, "project/binormal.frag" } },
	                                         binormal_shader);
	if (binormal_shader == 0u)
		LogError("Failed to load binormal shader");

	GLuint texcoord_shader = 0u;
	program_manager.CreateAndRegisterProgram("Texture coords",
	                                         { { ShaderType::vertex, "project/texcoord.vert" },
	                                           { ShaderType::fragment, "project/texcoord.frag" } },
	                                         texcoord_shader);
	if (texcoord_shader == 0u)
		LogError("Failed to load texcoord shader");

	auto const light_position = glm::vec3(-2.0f, 4.0f, 2.0f);
	auto const set_uniforms = [&light_position](GLuint program){
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
	};
	// Set the default tensions value; it can always be changed at runtime
	// through the "Scene Controls" window.
	float catmull_rom_tension = 0.5f;
	// Set whether the default interpolation algorithm should be the linear one;
	// it can always be changed at runtime through the "Scene Controls" window.
	bool use_linear = true;
	// Set whether to interpolate the position of an object or not; it can
	// always be changed at runtime through the "Scene Controls" window.
	bool interpolate = true;
	// Set whether to show the control points or not; it can always be changed
	// at runtime through the "Scene Controls" window.
	bool show_control_points = true;

		// Tree Bark shader
	GLuint bark = 0u;
	program_manager.CreateAndRegisterProgram("Tree_Bark",
		{ { ShaderType::vertex, "project/texture.vert" },
		  { ShaderType::fragment, "project/texture.frag" } },
		bark);
	if (bark == 0u)
		LogError("Failed to load treebark shader");


	auto const bark_uniforms = [&light_position](GLuint program) {
		glUniform3fv(glGetUniformLocation(program, "light_position"), 1, glm::value_ptr(light_position));
		};
	//auto circle_rings = Node();
	//auto second_branch = Branch(0.025f, 0.15f, glm::vec3(0,0,0), 0, glm::vec3(1.0),0.5f);


	GLuint cubemap = bonobo::loadTextureCubeMap(
		config::resources_path("cubemaps/SemiSky/sky-right.png"),
		config::resources_path("cubemaps/SemiSky/sky-left.png"),
		config::resources_path("cubemaps/SemiSky/sky-top.png"),
		config::resources_path("cubemaps/SemiSky/sky-bottom.png"),
		config::resources_path("cubemaps/SemiSky/sky-front.png"),
		config::resources_path("cubemaps/SemiSky/sky-back.png")
	);


	// --- Shaders done 

	auto skybox_shape = parametric_shapes::createSphere(100.0f, 100u, 100u);
	if (skybox_shape.vao == 0u) {
		LogError("Failed to retrieve the mesh for the skybox");
		return;
	}
	Node skybox, surface;
	skybox.set_geometry(skybox_shape);
	skybox.add_texture("cubemap", cubemap, GL_TEXTURE_CUBE_MAP);
	skybox.set_program(&skybox_shader);


	std::string s = shrubby.ApplyAxioms("F", 5); //TODO: This could be done in tree...?
	//std::cout << s << '\n';
	GLuint diff_texture = bonobo::loadTexture2D(config::resources_path("textures/BarkPoplar001_COL_4K.jpg"));
	Tree tree = Tree(s, shrubby, glm::vec3(0, 0, 0),&bark, bark_uniforms, diff_texture);
	
	//circle_rings.set_geometry(shape);
	//circle_rings.set_program(&normal_shader, set_uniforms);
	//TRSTransformf& circle_rings_transform_ref = circle_rings.get_transform();
	//second_branch.set_program(&normal_shader, set_uniforms);
	
	


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



		//circle_rings.render(mCamera.GetWorldToClipMatrix());
		//second_branch.render(mCamera.GetWorldToClipMatrix());
		//tree.render(mCamera.GetWorldToClipMatrix());
		for (size_t i = 0; i < tree.get_children_nb(); i += 1) {
			tree.get_child(i)->render(mCamera.GetWorldToClipMatrix());
		}

		skybox.get_transform().SetTranslate(glm::vec3(camera_position));
		skybox.render(mCamera.GetWorldToClipMatrix());

		

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
			ImGui::Checkbox("Show control points", &show_control_points);
			//ImGui::Checkbox("Enable interpolation", &interpolate);
			ImGui::Checkbox("Use linear interpolation", &use_linear);
			ImGui::SliderFloat("Catmull-Rom tension", &catmull_rom_tension, 0.0f, 1.0f);
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
