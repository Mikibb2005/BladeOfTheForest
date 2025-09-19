#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

class Menu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> opciones;
    int opcionSeleccionada;
    SDL_Color colorNormal;
    SDL_Color colorSeleccion;
    bool activo;

public:
    Menu(SDL_Renderer* r, const std::vector<std::string>& ops, const std::string& rutaFuente, int tamFuente)
        : renderer(r), opciones(ops), opcionSeleccionada(0), activo(false)
    {
        colorNormal = {255, 255, 255, 255};
        colorSeleccion = {255, 255, 0, 255};

        font = TTF_OpenFont(rutaFuente.c_str(), tamFuente);
        if (!font) {
            std::cerr << "Error cargando fuente: " << TTF_GetError() << std::endl;
            exit(-1);
        }
    }

    ~Menu() {
        if (font) TTF_CloseFont(font);
    }

    void activar() { activo = true; }
    void desactivar() { activo = false; }
    bool estaActivo() const { return activo; }

    void moverArriba() {
        opcionSeleccionada--;
        if (opcionSeleccionada < 0) opcionSeleccionada = opciones.size() - 1;
    }

    void moverAbajo() {
        opcionSeleccionada++;
        if (opcionSeleccionada >= (int)opciones.size()) opcionSeleccionada = 0;
    }

    int getOpcionSeleccionada() const { return opcionSeleccionada; }

    void render() {
        if (!activo) return;

        // Fondo semitransparente
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect fondo = {0, 0, 800, 600};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &fondo);

        // Dibujar opciones
        int y = 200;
        for (size_t i = 0; i < opciones.size(); i++) {
            SDL_Color color = (i == opcionSeleccionada) ? colorSeleccion : colorNormal;

            SDL_Surface* surface = TTF_RenderText_Solid(font, opciones[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dst = {300, y, surface->w, surface->h};
            SDL_FreeSurface(surface);

            SDL_RenderCopy(renderer, texture, nullptr, &dst);
            SDL_DestroyTexture(texture);

            y += 60;
        }
    }
};
