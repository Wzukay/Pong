internal void Clear_Screen(u32 color) {
	// SETS THE SCREEN TO A SPECIFIED COLOR, BASICALLY CLEARING IT IN THE PROCESS OF ANY OTHER 
	// DISPLAYED ELEMENTS

	u32* pixel = (u32*)render.memory;
	for (s32 y = 0; y < render.height; y++) {
		for (s32 x = 0; x < render.width; x++) {
			*pixel++ = color;
		}
	}	
}

internal void Draw_Rect_In_Pixels(s32 x0, s32 y0, s32 x1, s32 y1, u32 color) {
	// CLAMPS THE VALUES SO THAT THE GAME DOESN'T CRASHES WHEN CHANGING THE WINDOW TO A VERY SMALL SIZE

	x0 = clamp(0, x0, render.width);
	x1 = clamp(0, x1, render.width);

	y0 = clamp(0, y0, render.height);
	y1 = clamp(0, y1, render.height);

	// DRAWS EACH PIXEL WITHIN THE LIMITS OF THE FUNCTION
	for (s32 y = y0; y < y1; y++) {
		u32* pixel = (u32*)render.memory + x0 + y * render.width;
		for (s32 x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float renderScaleMultiplier = 0.01f;

internal void Draw_Rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	// SEPARATE FUNCTION IS NEEDED BECAUSE IF I WERE TO USE THE DRAW_RECT_IN_PIXELS ONE
	// THE RECTS DRAWN ON THE SCREEN WOULDN'T REMAIN THE SAME SIZE AFTER CHANGING
	// THE SIZE OF THE WINDOW, THIS FUNCTION MAKES SURE THAT THE RECTS STAY THE SAME

	x *= render.height * renderScaleMultiplier;
	y *= render.height * renderScaleMultiplier;
	half_size_x *= render.height * renderScaleMultiplier;
	half_size_y *= render.height * renderScaleMultiplier;
	
	x += render.width / 2.f;
	y += render.height / 2.f;
	
	// CHANGE TO PIXELS
	s32 x0 = x - half_size_x;
	s32 x1 = x + half_size_x;
	s32 y0 = y - half_size_y;
	s32 y1 = y + half_size_y;

	Draw_Rect_In_Pixels(x0, y0, x1, y1, color);
}