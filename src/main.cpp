#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "Error inicializando SDL: " << SDL_GetError() << endl;
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Blade of the Forest", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    
        if (!window)
        {
            cout << "Error creando window: " << SDL_GetError() << endl;
            SDL_Quit();
            return 1;
        }
        SDL_Delay(2000);

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;


}