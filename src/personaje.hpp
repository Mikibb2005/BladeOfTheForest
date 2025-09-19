#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>

class Personaje {
private:
    std::string nombre;
    int vida;
    int ataque;
    SDL_Texture* textura;
    SDL_Rect rect; // posición y tamaño en pantalla
    bool mirandoIzquierda;

    // Animación
    std::vector<SDL_Rect> frames;
    int frameActual;
    Uint32 lastFrameTime;
    int frameDelay; // ms entre frames

public:
    // Constructor: numFrames = 0 => auto-detectar
    Personaje(const std::string& n, int v, int a, SDL_Renderer* renderer, const std::string& rutaImagen,
              int x, int y, int w, int h, int numFrames = 0, int frameDelayMs = 120)
        : nombre(n), vida(v), ataque(a), textura(nullptr),
          rect{ x, y, w, h }, mirandoIzquierda(false),
          frameActual(0), lastFrameTime(0), frameDelay(frameDelayMs)
    {
        SDL_Surface* surface = IMG_Load(rutaImagen.c_str());
        if (!surface) {
            std::cerr << "Error cargando imagen (" << rutaImagen << "): " << IMG_GetError() << std::endl;
            throw std::runtime_error("IMG_Load failed");
        }

        int detectedFrames = 1;
        // Auto-detección si numFrames = 0
        if (numFrames <= 0) {
            if (surface->w > surface->h) {
                // Si la imagen es más ancha que alta, asumimos spritesheet horizontal
                detectedFrames = surface->w / surface->h;
            }
        } else {
            detectedFrames = numFrames;
        }

        // Calculamos los clips de animación
        int frameWidth = surface->w / detectedFrames;
        int frameHeight = surface->h;
        for (int i = 0; i < detectedFrames; ++i) {
            SDL_Rect r = { i * frameWidth, 0, frameWidth, frameHeight };
            frames.push_back(r);
        }

        textura = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!textura) {
            std::cerr << "Error creando textura: " << SDL_GetError() << std::endl;
            throw std::runtime_error("SDL_CreateTextureFromSurface failed");
        }
    }

    ~Personaje() {
        if (textura) {
            SDL_DestroyTexture(textura);
            textura = nullptr;
        }
    }

    // Actualiza el frame de animación en base al tiempo
    void actualizarAnimacion() {
        if (frames.size() <= 1) return; // si no hay animación, no hace nada
        Uint32 ahora = SDL_GetTicks();
        if (ahora - lastFrameTime >= (Uint32)frameDelay) {
            frameActual = (frameActual + 1) % frames.size();
            lastFrameTime = ahora;
        }
    }

    // Dibuja usando el frame actual y aplicando flip si mira a la izquierda
    void render(SDL_Renderer* renderer) {
        SDL_RendererFlip flip = mirandoIzquierda ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_Rect src = frames[frameActual];
        SDL_RenderCopyEx(renderer, textura, &src, &rect, 0.0, nullptr, flip);
    }

    // Mueve el personaje y ajusta la dirección (flip)
    void mover(int dx, int dy) {
        rect.x += dx;
        rect.y += dy;
        if (dx < 0) mirandoIzquierda = true;
        if (dx > 0) mirandoIzquierda = false;
    }

    // Getters útiles
    const std::string& getNombre() const { return nombre; }
    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    bool estaVivo() const { return vida > 0; }
};
