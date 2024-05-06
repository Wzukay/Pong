#include "utils.cpp"
#include <windows.h>

struct Render_State {
	s32 height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable bool running = true;
global_variable Render_State render;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK Window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
		// IF THE WINDOW WAS CLOSED
		case WM_CLOSE:
			case WM_DESTROY: {
				running = false; 
			} break;
		
		// IF THE WINDOW'S SIZE CHANGED
		case WM_SIZE: {
			// GETS THE WIDTH AND HEIGHT FROM RECT
			RECT rect;
			GetClientRect(hwnd, &rect);

			// CALCULATE NEW WINDOW SIZE
			render.width = rect.right - rect.left;
			render.height = rect.bottom - rect.top;

			s32 size = render.width * render.height * sizeof(u32);

			// IF THE MEMORY WAS ALREADY FILLED WITHG WINDOW DATA, CLEAR IT 
			if (render.memory) VirtualFree(render.memory, 0, MEM_RELEASE);

			// ALLOCATE THE NEW WINDOW DATA TO THE MEMORY
			render.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render.bitmap_info.bmiHeader.biSize = sizeof(render.bitmap_info.bmiHeader);
			render.bitmap_info.bmiHeader.biWidth = render.width;
			render.bitmap_info.bmiHeader.biHeight = render.height;
			render.bitmap_info.bmiHeader.biPlanes = 1;
			render.bitmap_info.bmiHeader.biBitCount = 32;
			render.bitmap_info.bmiHeader.biCompression = BI_RGB;

			// SET THE NEW PROPERTIES FOR THE WINDOW
		} break;

		// SEND CALLBACK RESULTS 
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd) {
	// CREATE WINDOW CLASS
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Pong Window Class";
	window_class.lpfnWndProc = Window_callback;

	// REGISTER WINDOW
	RegisterClass(&window_class);

	// CREATE WINDOW
	HWND window = CreateWindowA(window_class.lpszClassName, "Pong Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); // FOR WINDOW DISPLAY

	Input input = {};

	float deltaTime = .016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time); // COUNTS IN CPU TIME


	float performance_frequency; {
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	// GAME LOOP, IF !RUNNING THE GAME CLOSES
	while (running) {	

		// GET MESSAGES FROM THE WINDOW
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

			switch (message.message) {
				case WM_KEYUP:
					case WM_KEYDOWN: {
						u32 vk_code = (u32)message.wParam;
						bool is_down = ((message.lParam & (1 << 31)) == 0);

						#define ProcessButton(b, vk)\
							case vk: {\
								input.buttons[b].changed = is_down != input.buttons[b].is_down;\
								input.buttons[b].is_down = is_down;\
							} break;

						switch (vk_code) {
							ProcessButton(BUTTON_UP, VK_UP);
							ProcessButton(BUTTON_DOWN, VK_DOWN);
							ProcessButton(BUTTON_W, 'W');
							ProcessButton(BUTTON_S, 'S');
						}
					} break; 

				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}		
		}

		SimulateGame(&input, deltaTime);

		// DISPLAY ON WINDOW
		StretchDIBits(hdc, 0, 0, render.width, render.height, 0, 0, render.width, render.height, render.memory, &render.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time); // COUNTS IN CPU TIME
		deltaTime = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
}