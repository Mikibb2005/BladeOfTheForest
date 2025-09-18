#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>

class Personaje {
private:
    std::string nombre;
    int vida;
    int ataque;
    SDL_Texture* textura;
    SDL_Rect rect; // posición y tamaño
    bool mirandoIzquierda;

public:
    Personaje(const std::string& n, int v, int a, SDL_Renderer* renderer, const std::string& rutaImagen,
              int x, int y, int w, int h)
        : nombre(n), vida(v), ataque(a), textura(nullptr), mirandoIzquierda(false)
    {
        SDL_Surface* surface = IMG_Load(rutaImagen.c_str());
        if (!surface) {
            throw std::runtime_error("No se pudo cargar la imagen: " + rutaImagen);
        }
        textura = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }

    ~Personaje() {
        if (textura) SDL_DestroyTexture(textura);
    }

    void render(SDL_Renderer* renderer) {
        SDL_RendererFlip flip = mirandoIzquierda ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, textura, nullptr, &rect, 0, nullptr, flip);
    }

    void mover(int dx, int dy) {
        rect.x += dx;
        rect.y += dy;
        if (dx < 0) mirandoIzquierda = true;
        if (dx > 0) mirandoIzquierda = false;
    }

    // Getters básicos
    const std::string& getNombre() const { return nombre; }
    int getVida() const { return vida; }
    bool estaVivo() const { return vida > 0; }
};
