#include "sdl.hpp"


Sdl::Sdl() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("L'initialisation a échoué.");
    }
}

void Sdl::wait_for_close() {
    SDL_Event e;
    bool displayed = true;
    
    while (displayed) {
        while (SDL_PollEvent(&e)) {
            // Détection de la fermeture individuelle des fenêtres
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (SDL_GetWindowID(w1->window) == e.window.windowID) {
                    close(w1);
                }
                if (SDL_GetWindowID(w2->window) == e.window.windowID) {
                    close(w2);
                }
                if (SDL_GetWindowID(w3->window) == e.window.windowID) {
                    close(w3);
                }
                if (SDL_GetWindowID(w4->window) == e.window.windowID) {
                    close(w4);
                }
            }
            // Événement de fermeture globale
            if (e.type == SDL_QUIT)
                displayed = false;
        }
    }
}

void Sdl::show() {
    // Affichage des 4 fenêtres
    w1->plot();
    SDL_RenderPresent(w1->renderer);

    w2->plot();
    SDL_RenderPresent(w2->renderer);

    w3->plot();
    SDL_RenderPresent(w3->renderer);

    w4->plot();
    SDL_RenderPresent(w4->renderer);

    wait_for_close();
}

void Sdl::close(Window* w_) {
    SDL_DestroyWindow(w_->window);
    SDL_DestroyRenderer(w_->renderer);
}

void Sdl::quit() {
    SDL_Quit();
    std::cout << "Fermeture de tous les affichages" << std::endl;
}