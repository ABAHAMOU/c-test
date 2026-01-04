#ifndef _SDL_HPP_
#define _SDL_HPP_

#include <iostream>
#include <vector>

#include "SDL.h"
#include "window.hpp"

/**
 * @file Sdl.h
 * @brief Gestionnaire d'affichage multiple avec SDL2
 */

/**
 * @class Sdl
 * @brief Classe gérant l'affichage simultané de 4 fenêtres graphiques
 * 
 * Cette classe permet de créer, afficher et gérer plusieurs fenêtres
 * SDL2 simultanément pour visualiser différents résultats numériques.
 */
class Sdl {
public:
    Window* w1;     ///< Première fenêtre d'affichage
    Window* w2;     ///< Deuxième fenêtre d'affichage
    Window* w3;     ///< Troisième fenêtre d'affichage
    Window* w4;     ///< Quatrième fenêtre d'affichage

public:
    /**
     * @brief Constructeur de la classe Sdl
     * 
     * Initialise la bibliothèque SDL2 pour l'affichage graphique.
     */
    Sdl();
    
    /**
     * @brief Attend la fermeture des fenêtres par l'utilisateur
     * 
     * Gère les événements SDL pour détecter la fermeture de chaque
     * fenêtre individuellement ou la fermeture globale.
     */
    void wait_for_close();
    
    /**
     * @brief Affiche toutes les fenêtres et leurs données
     * 
     * Trace les graphiques dans chaque fenêtre et attend que
     * l'utilisateur les ferme avant de continuer.
     */
    void show();
    
    /**
     * @brief Ferme une fenêtre spécifique
     * @param w_ Pointeur vers la fenêtre à fermer
     * 
     * Détruit la fenêtre SDL et son renderer associé.
     */
    void close(Window* w_);
    
    /**
     * @brief Quitte le processus d'affichage
     * 
     * Libère toutes les ressources SDL et termine proprement
     * le système d'affichage graphique.
     */
    void quit();
};

#endif