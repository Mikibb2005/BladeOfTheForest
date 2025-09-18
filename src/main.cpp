#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "personaje.hpp"

using namespace std;

int window_LARGO = 800;
int window_ALTO = 600;

void DetectarResolucion()
{
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "Error al obtener resolución: " << SDL_GetError() << std::endl;
    }
    window_ALTO = displayMode.h;
    window_LARGO = displayMode.w;
}

int main(int argc, char* argv[])
{
    //     INICIALIZAR SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "Error inicializando SDL: " << SDL_GetError() << endl;
        return 1;
    }
    
    //     INICIALIZAR SDL_IMAGE (para los png)
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cerr << "Error inicializando SDL_image: " << IMG_GetError() << endl;
        SDL_Quit();
        return -1;
    }
    
    // ***  DETECTAR RESOLUCION
    DetectarResolucion();

    //     Crear ventana
    SDL_Window* window = SDL_CreateWindow("Blade of the Forest", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_LARGO, window_ALTO, SDL_WINDOW_SHOWN);
    
    if (!window)
    {
        cout << "Error creando window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
        
    //        CREAR RENDER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        cerr << "Error crenado renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    //          CREAR PERSONAJE
    Personaje heroe("Caballero", 100, 20, renderer, "assets/Sprite-0001.png",
                    (window_LARGO / 2) - 64, (window_ALTO / 2) - 64, 128, 128);

    //  ***BUCLE PRINCIPAL***
    bool running = true;
    SDL_Event event;
    const Uint8* teclas = SDL_GetKeyboardState(NULL); // estado del teclado

    while(running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Movimiento continuo
        int velocidad = 5;
        if (teclas[SDL_SCANCODE_W] || teclas[SDL_SCANCODE_UP])    heroe.mover(0, -velocidad);
        if (teclas[SDL_SCANCODE_S] || teclas[SDL_SCANCODE_DOWN])  heroe.mover(0, velocidad);
        if (teclas[SDL_SCANCODE_A] || teclas[SDL_SCANCODE_LEFT])  heroe.mover(-velocidad, 0);
        if (teclas[SDL_SCANCODE_D] || teclas[SDL_SCANCODE_RIGHT]) heroe.mover(velocidad, 0);

        //  LIMPIAR PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Dibujar personaje
        heroe.render(renderer);

        // ACTUALIZACION PANTALLA
        SDL_RenderPresent(renderer);

        // Pequeño delay para controlar CPU
        SDL_Delay(16); // ~60 FPS
    }

    //     LIBERACION DE RECURSOS
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
