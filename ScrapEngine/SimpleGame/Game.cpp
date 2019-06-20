#include "Engine/Manager/EngineManager.h"
#include "Engine/Input/Manager/InputManager.h"
#include "GameObjects/Camera/GameCamera.h"
#include <iostream>
#include "GameObjects/Terrain.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Crate.h"

int main() {
	short exit_value = EXIT_SUCCESS;
	ScrapEngine::Manager::EngineManager* ScrapEngineManager = nullptr;
	try {
		//init engine
		ScrapEngineManager = new ScrapEngine::Manager::EngineManager("ScrapEngine Simple Chess Game", 0, 1280, 720);
		ScrapEngine::Render::GameWindow* gameWindowRef = ScrapEngineManager->render_manager_view->get_game_window();
		gameWindowRef->set_window_icon("../assets/game_icon/chess_game_icon_png.png");
		//Create the input manager
		ScrapEngine::Input::InputManager* inputmanager = gameWindowRef->create_window_input_manager();
		inputmanager->set_cursor_input_mode(ScrapEngine::Input::cursor_mode::cursor_grabbed_mode);
		//Get the component manager
		ScrapEngine::Core::ComponentsManager* ComponentManagerRef = ScrapEngineManager->logic_manager_view->get_components_manager();
		//Set the world skybox
		ScrapEngine::Core::SceneManager* SceneManagerRef = ScrapEngineManager->logic_manager_view->get_scene_manager();
		SceneManagerRef->set_skybox(
			std::array<std::string, 6>{
				"../assets/skybox/stormydays_ft.png", //FRONT TEXTURE
				"../assets/skybox/stormydays_bk.png", //BACK TEXTURE
				"../assets/skybox/stormydays_up.png", //UP TEXTURE
				"../assets/skybox/stormydays_dn.png", //DOWN TEXTURE
				"../assets/skybox/stormydays_rt.png", //RIGHT TEXTURE
				"../assets/skybox/stormydays_lf.png", //LEFT TEXTURE
			}
		);
		SceneManagerRef->set_skybox_size(150);
		//Create the first game object
		//Ball
		Ball* BallGameObject = new Ball(ComponentManagerRef, inputmanager);
		ScrapEngineManager->logic_manager_view->register_game_object(BallGameObject);
		//Terrain
		Terrain* TerrainGameObject = new Terrain(ComponentManagerRef);
		ScrapEngineManager->logic_manager_view->register_game_object(TerrainGameObject);
		//Crate
		/*new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(0, 0, -50));
		new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(10, 0, -50));
		new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(-10, 0, -50));
		new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(5, 10, -50));
		new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(-5, 10, -50));
		new Crate(ComponentManagerRef, ScrapEngine::Core::SVector3(0, 20, -50));*/
		//Create the camera
		GameCamera* GameCameraRef = new GameCamera(inputmanager, ScrapEngineManager->render_manager_view->get_default_render_camera());
		GameCameraRef->set_game_window_ref(gameWindowRef);
		ScrapEngineManager->logic_manager_view->register_game_object(GameCameraRef);
		//Begin gameplay
		ScrapEngineManager->start_game_loop();
		//End gameplay
	}catch (const std::exception& e) {
		std::cerr << "EXCEPTION MESSAGE-->" << e.what() << std::endl;
		exit_value = EXIT_FAILURE;
	}
	//Delete
	delete ScrapEngineManager;
	//Display error message
	if (exit_value == EXIT_FAILURE) { //Exit failure, whait to close
		std::cout << std::endl << "EXIT_FAILURE - Press to exit..." << std::endl;
		fflush(stdin);
		getchar();
	}
	else {
		std::cout << std::endl << "EXIT_SUCCESS" << std::endl;
	}
	//End of program
	return exit_value;
}
