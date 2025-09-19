#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>

class Config {
public:
    int width;
    int height;
    int fps;
    bool vsync;

    Config() {
        // Valores por defecto
        width = 800;
        height = 600;
        fps = 60;
        vsync = false;
    }

    void loadAuto() {
        SDL_DisplayMode dm;
        if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
            width = dm.w;
            height = dm.h;
            fps = dm.refresh_rate > 0 ? dm.refresh_rate : 60;
            vsync = true;
            std::cout << "[Config] AUTO -> Resolución " << width << "x" << height 
                      << " @ " << fps << "fps\n";
        } else {
            std::cerr << "[Config] Error al detectar modo pantalla: " << SDL_GetError() << "\n";
        }
    }

    void showMenu() {
        std::cout << "===== Menu de Configuracion =====\n";
        std::cout << "1. 800x600  @ 60fps\n";
        std::cout << "2. 1280x720 @ 60fps\n";
        std::cout << "3. 1920x1080@ 60fps\n";
        std::cout << "4. Automático (monitor)\n";
        std::cout << "Selecciona opcion: ";
        int opt; std::cin >> opt;

        switch (opt) {
            case 1: width = 800; height = 600; fps = 60; vsync = false; break;
            case 2: width = 1280; height = 720; fps = 60; vsync = false; break;
            case 3: width = 1920; height = 1080; fps = 60; vsync = false; break;
            case 4: loadAuto(); break;
            default: std::cout << "Opcion invalida, usando defecto\n"; break;
        }
    }
};
