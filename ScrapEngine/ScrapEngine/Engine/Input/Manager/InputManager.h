#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace ScrapEngine {

	struct MouseLocation {
		double xpos, ypos;

		MouseLocation() {
			xpos = 0;
			ypos = 0;
		}
		MouseLocation(double x, double y) {
			xpos = x;
			ypos = y;
		}
	};

	enum CursorMode {
		cursor_normal_mode, //makes the cursor visible and behaving normally.
		cursor_hidden_mode, //makes the cursor invisible when it is over the client area of the window but does not restrict the cursor from leaving.
		cursor_grabbed_mode //hides and grabs the cursor, providing virtual and unlimited cursor movement.
	};

	enum SystemCursorShapes {
		cursor_regular_arrow,
		cursor_crosshair_shape,
		cursor_hand_shape,
		cursor_horizontal_resize_arrow_shape,
		cursor_vertical_resize_arrow_shape,
		cursor_input_beam_shape
	};

	enum ButtonState {
		released, pressed
	};

	class InputManager
	{
	private:
		GLFWwindow* windowRef;
		GLFWcursor* cursor = nullptr;
	public:
		InputManager(GLFWwindow* window);
		~InputManager();

		MouseLocation getLastMouseLocation();
		void SetCursorInputMode(ScrapEngine::CursorMode NewMode);

		void LoadNewCursor(const std::string& path_to_file, int xhot = 0, int yhot = 0);
		void LoadSystemCursor(ScrapEngine::SystemCursorShapes NewShape);
		void ResetCursorToSystemDefault();

		int getKeyboardKeyStatus(int key_to_check);
		int getKeyboardKeyPressed(int key_to_check);
		int getKeyboardKeyReleased(int key_to_check);

		int getMouseButtonStatus(int button_to_check);
		int getMouseButtonPressed(int button_to_check);
		int getMouseButtonReleased(int button_to_check);
	};

}

