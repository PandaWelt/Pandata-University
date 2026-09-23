#define SDL_MAIN_HANDLED
#include "desktop/DesktopView.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace fs=std::filesystem;
namespace {
fs::path discover() {
    char* raw=SDL_GetBasePath();
    if(!raw) throw pu::Error("Cannot locate this executable. Use Start-Pandata-University-Desktop.cmd.");
    const std::string text=raw; SDL_free(raw);
    fs::path path=fs::path(std::u8string(text.begin(),text.end()));
    for(int n=0;n<8 && !path.empty();++n,path=path.parent_path())
        if(fs::is_regular_file(path/"course/university/pilot.puc") && fs::is_directory(path/"university")) return path;
    throw pu::Error("Repository not found beside this build. Use Start-Pandata-University-Desktop.cmd in your repository.");
}
int run(const std::vector<fs::path>& args) {
    SDL_Window* window=nullptr; SDL_GLContext gl=nullptr;
    bool context=false,platform=false,renderer=false;
    int result=1;
    try {
        fs::path root; bool smoke=false;
        for(std::size_t i=1;i<args.size();++i) {
            if(args[i]=="--repository" && root.empty() && i+1<args.size()) root=args[++i];
            else if(args[i]=="--smoke" && !smoke) smoke=true;
            else throw pu::Error("Use no arguments, or --repository <existing repository> [--smoke]. No other command or file argument is accepted.");
        }
        SDL_SetMainReady();
        if(root.empty()) root=discover();
        pu::rejectLinks(root); root=fs::canonical(root);
        pu::desktop::DeskController desk(root);
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0) throw pu::Error(std::string("SDL video unavailable: ")+SDL_GetError());
#ifdef SDL_HINT_WINDOWS_DPI_AWARENESS
        SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS,"permonitorv2");
#endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3); SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        window=SDL_CreateWindow("Pandata University | Desktop 1",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
            1440,900,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
        if(!window) throw pu::Error(std::string("Desktop window unavailable: ")+SDL_GetError());
        SDL_SetWindowMinimumSize(window,880,620);
        gl=SDL_GL_CreateContext(window);
        if(!gl || SDL_GL_MakeCurrent(window,gl)!=0) throw pu::Error(std::string("OpenGL 3.0 unavailable: ")+SDL_GetError()+". The console launcher remains available.");
        SDL_GL_SetSwapInterval(1);
        IMGUI_CHECKVERSION(); ImGui::CreateContext(); context=true;
        auto& io=ImGui::GetIO(); io.IniFilename=nullptr; io.LogFilename=nullptr;
        io.ConfigFlags|=ImGuiConfigFlags_NavEnableKeyboard;
        pu::desktop::DesktopView::theme(std::clamp(ImGui_ImplSDL2_GetContentScaleForDisplay(0),1.0f,2.0f));
        platform=ImGui_ImplSDL2_InitForOpenGL(window,gl);
        renderer=ImGui_ImplOpenGL3_Init("#version 130");
        if(!platform || !renderer) throw pu::Error("Graphics backend initialization failed. Use the console fallback.");
        pu::desktop::DesktopView view(desk);
        bool quit=false; int frames=0;
        while(!quit) {
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if(event.type==SDL_QUIT || (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE)) {
                    if(!desk.busy()) quit=true; // Do not abandon a bounded action without its result.
                }
            }
            if(SDL_GetWindowFlags(window)&SDL_WINDOW_MINIMIZED) { desk.poll(); SDL_Delay(30); continue; }
            ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL2_NewFrame(); ImGui::NewFrame();
            view.draw(); ImGui::Render();
            int w=0,h=0; SDL_GL_GetDrawableSize(window,&w,&h); glViewport(0,0,w,h);
            glClearColor(.055f,.072f,.09f,1); glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); SDL_GL_SwapWindow(window);
            ++frames;
            if(smoke && frames==20) {
                if(ImGui::GetDrawData()->TotalVtxCount==0) throw pu::Error("Smoke rendered no interface vertices.");
                std::cout<<"PASS: desktop graphics smoke; 20 frames, actual SDL/OpenGL window and dashboard; no learner writes.\n";
                quit=true;
            }
        }
        result=0;
    } catch(const std::exception& e) {
        std::cerr<<"Desktop stopped safely: "<<e.what()<<'\n';
        // During --smoke, report to captured stderr instead of blocking automation on a modal dialog.
        if(std::find(args.begin(),args.end(),fs::path("--smoke"))==args.end())
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Pandata University - safe stop",e.what(),window);
    }
    if(renderer) ImGui_ImplOpenGL3_Shutdown();
    if(platform) ImGui_ImplSDL2_Shutdown();
    if(context) ImGui::DestroyContext();
    if(gl) SDL_GL_DeleteContext(gl);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit(); return result;
}
}
#ifdef _WIN32
int wmain(int argc,wchar_t** argv) {
#else
int main(int argc,char** argv) {
#endif
    std::vector<fs::path> args; for(int n=0;n<argc;++n) args.emplace_back(argv[n]);
    return run(args);
}
