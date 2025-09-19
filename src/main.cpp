#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include "personaje.hpp"
#include "gamestate.hpp"

using namespace std;

int window_LARGO = 800;
int window_ALTO = 600;
int targetFPS = 60;

void DetectarResolucion()
{
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "Error al obtener resolución: " << SDL_GetError() << std::endl;
    }
    window_ALTO = displayMode.h;
    window_LARGO = displayMode.w;
}

// Guardar partida simple (ejemplo: vida + posición del héroe)
void GuardarPartida(const Personaje& p) {
    ofstream save("savegame.sav");
    if (!save.is_open()) return;
    save << p.getNombre() << " "
         << p.getVida() << " "
         << p.getAtaque() << "\n";
    save.close();
    cout << "Partida guardada en savegame.sav" << endl;
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

    //     INICIALIZAR SDL_TTF (para textos del menú)
    if (TTF_Init() == -1) {
        cerr << "Error inicializando SDL_ttf: " << TTF_GetError() << endl;
        IMG_Quit();
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
        cerr << "Error creando renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Fuente para el menú
    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 32);
    if (!font) {
        cerr << "Error cargando fuente: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
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
    GameState estado = GameState::JUGANDO;

    int menuSeleccion = 0; // índice del menú

    while(running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (estado == GameState::JUGANDO && event.key.keysym.sym == SDLK_ESCAPE) {
                    estado = GameState::MENU_PAUSA;
                } 
                else if (estado == GameState::MENU_PAUSA) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        estado = GameState::JUGANDO;
                    }
                    else if (event.key.keysym.sym == SDLK_UP) {
                        menuSeleccion = (menuSeleccion - 1 + 4) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN) {
                        menuSeleccion = (menuSeleccion + 1) % 4;
                    }
                    else if (event.key.keysym.sym == SDLK_RETURN) {
                        if (menuSeleccion == 0) estado = GameState::JUGANDO;
                        else if (menuSeleccion == 1) estado = GameState::MENU_CONFIG;
                        else if (menuSeleccion == 2) GuardarPartida(heroe);
                        else if (menuSeleccion == 3) running = false;
                    }
                }
                else if (estado == GameState::MENU_CONFIG) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        estado = GameState::MENU_PAUSA;
                    }
                }
            }
        }

        //  LIMPIAR PANTALLA
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        if (estado == GameState::JUGANDO) {
            // Movimiento continuo
            int velocidad = 5;
            if (teclas[SDL_SCANCODE_W] || teclas[SDL_SCANCODE_UP])    heroe.mover(0, -velocidad);
            if (teclas[SDL_SCANCODE_S] || teclas[SDL_SCANCODE_DOWN])  heroe.mover(0, velocidad);
            if (teclas[SDL_SCANCODE_A] || teclas[SDL_SCANCODE_LEFT])  heroe.mover(-velocidad, 0);
            if (teclas[SDL_SCANCODE_D] || teclas[SDL_SCANCODE_RIGHT]) heroe.mover(velocidad, 0);

            // Dibujar personaje
            heroe.actualizarAnimacion();
            heroe.render(renderer);
        }
        else if (estado == GameState::MENU_PAUSA) {
            // Renderizar opciones del menú
            const char* opciones[4] = { "Reanudar Juego", "Configuracion", "Guardar Partida", "Salir" };
            for (int i = 0; i < 4; i++) {
                SDL_Color color = (i == menuSeleccion) ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, opciones[i], color);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect dstRect = { window_LARGO/2 - 150, 200 + i*60, 300, 50 };
                SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
        else if (estado == GameState::MENU_CONFIG) {
            const char* opciones[3] = { "Resolucion: Auto", "FPS: 60", "Volver" };
            for (int i = 0; i < 3; i++) {
                SDL_Color color = {200, 200, 255, 255};
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, opciones[i], color);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect dstRect = { window_LARGO/2 - 150, 200 + i*60, 300, 50 };
                SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }

        // ACTUALIZACION PANTALLA
        SDL_RenderPresent(renderer);

        // Delay según FPS
        SDL_Delay(1000 / targetFPS);
    }

    //     LIBERACION DE RECURSOS
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
