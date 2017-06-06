#include "GB.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>


extern "C" {
	int imgui(void *p_s_gb);
}

int IMGUI_debugger(void *p_s_gb)
{
	struct s_gb		*s_gb = (struct s_gb *)p_s_gb;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	int res = SDL_GetCurrentDisplayMode(0, &current);
	if (res != 0)
		SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());
	SDL_Window *window = SDL_CreateWindow("ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	// Setup ImGui binding
	ImGui_ImplSdl_Init(window);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	// Main loop
	bool done = false;
	while (s_gb->stopdbg != 1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdl_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}
		ImGui_ImplSdl_NewFrame(window);
		{
			ImGui::Begin("CPU register state");
			ImGui::Text("emgb debug window");
			ImGui::Text("a %x f %x af %x", s_gb->gb_register.a, s_gb->gb_register.f, s_gb->gb_register.af);
			ImGui::Text("b %x c %x bc %x", s_gb->gb_register.b, s_gb->gb_register.c, s_gb->gb_register.bc);
			ImGui::Text("d %x e %x de %x", s_gb->gb_register.d, s_gb->gb_register.e, s_gb->gb_register.de);
			ImGui::Text("h %x l %x hl %x", s_gb->gb_register.h, s_gb->gb_register.l, s_gb->gb_register.hl);
			ImGui::Text("pc %x sp %x", s_gb->gb_register.pc, s_gb->gb_register.sp);
			ImGui::End();
		}
		{
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("LCD state");
			ImGui::Text("Hello");
			ImGui::End();
		}


		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplSdl_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	return (0);
}

int imgui(void *s_gb)
{
	IMGUI_debugger(s_gb);
	return (0);
}
