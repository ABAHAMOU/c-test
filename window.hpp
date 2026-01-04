#ifndef _H_WINDOW
#define _H_WINDOW

#include <iostream>
#include <vector>

#include "SDL.h"

#define THRESHOLD_MIN 1e-8
#define POINT_RADIUS 1

/**
 * @file Window.h
 * @brief Classe pour l'affichage graphique avec SDL2
 */

/**
 * @class Window
 * @brief Fenêtre d'affichage graphique permettant de tracer jusqu'à 2 courbes
 * 
 * Cette classe gère la création d'une fenêtre SDL2 et le tracé de données
 * sous forme de nuages de points. Elle supporte l'affichage simultané de
 * deux séries de données (y1 et y2) sur le même graphique.
 */
class Window {
private:
    static int window_id;           ///< Identifiant unique de fenêtre
    const char* window_title;       ///< Titre de la fenêtre
    int window_width;               ///< Largeur de la fenêtre en pixels
    int window_height;              ///< Hauteur de la fenêtre en pixels
    float alpha;                    ///< Coefficient de dimensionnement du cadre
    double L;                       ///< Longueur du domaine spatial

public:
    SDL_Window* window;             ///< Pointeur vers la fenêtre SDL
    SDL_Renderer* renderer;         ///< Pointeur vers le renderer SDL

    std::vector<double> x;          ///< Coordonnées en abscisse
    std::vector<double> y1;         ///< Première série de données en ordonnée
    std::vector<double> y2;         ///< Deuxième série de données en ordonnée

    double y_min_1;                 ///< Valeur minimale de y1
    double y_min_2;                 ///< Valeur minimale de y2
    double y_max_1;                 ///< Valeur maximale de y1
    double y_max_2;                 ///< Valeur maximale de y2
    double Y_MIN;                   ///< Minimum global pour l'échelle
    double Y_MAX;                   ///< Maximum global pour l'échelle

public:
    /**
     * @brief Constructeur de la classe Window
     * @param L_ Longueur du domaine spatial
     * @param window_title_ Titre de la fenêtre
     * @param window_width_ Largeur de la fenêtre (défaut: 640)
     * @param window_height_ Hauteur de la fenêtre (défaut: 480)
     * @throws const char* Si la création de la fenêtre échoue
     */
    Window(double L_, const char* window_title_ = "Option européenne [PAP]", 
           int window_width_ = 640, int window_height_ = 480);
    
    /**
     * @brief Destructeur de la classe Window
     */
    ~Window();

    /**
     * @brief Efface le contenu de la fenêtre
     */
    void clear();

    /**
     * @brief Affiche le cadre du graphique
     */
    void plot_frame();
    
    /**
     * @brief Fonction de mapping pour le rescaling des coordonnées
     * @param x Valeur à convertir
     * @param in_min Minimum de l'intervalle d'entrée
     * @param in_max Maximum de l'intervalle d'entrée
     * @param out_min Minimum de l'intervalle de sortie
     * @param out_max Maximum de l'intervalle de sortie
     * @return Valeur convertie dans le nouvel intervalle
     */
    double map(double x, double in_min, double in_max, double out_min, double out_max);

    /**
     * @brief Définit les coordonnées en abscisse
     * @param x_ Vecteur des abscisses
     */
    void set_x(std::vector<double> x_);
    
    /**
     * @brief Définit la première série de données
     * @param y1_ Vecteur des ordonnées (série 1)
     */
    void set_y1(std::vector<double> y1_);
    
    /**
     * @brief Définit la deuxième série de données
     * @param y2_ Vecteur des ordonnées (série 2)
     */
    void set_y2(std::vector<double> y2_);
    
    /**
     * @brief Calcule les bornes du graphique (Y_MIN et Y_MAX)
     */
    void get_graph_boundaries();

    /**
     * @brief Vérifie si y1 contient des données
     * @return true si y1 n'est pas vide
     */
    bool has_y1();
    
    /**
     * @brief Vérifie si y2 contient des données
     * @return true si y2 n'est pas vide
     */
    bool has_y2();

    /**
     * @brief Trace la première série de données
     * @throws const char* Si les tailles de x et y1 ne correspondent pas
     */
    void plot_y1();
    
    /**
     * @brief Trace la deuxième série de données
     * @throws const char* Si les tailles de x et y2 ne correspondent pas
     */
    void plot_y2();
    
    /**
     * @brief Dessine un cercle aux coordonnées spécifiées
     * @param n_cx Coordonnée x du centre
     * @param n_cy Coordonnée y du centre
     * @param radius Rayon du cercle (défaut: POINT_RADIUS)
     */
    void draw_circle(int n_cx, int n_cy, int radius = POINT_RADIUS);
    
    /**
     * @brief Affiche le graphique complet avec toutes les données
     * 
     * Cette fonction trace le cadre, calcule les échelles et affiche
     * les séries de données disponibles (y1 en rouge, y2 en vert).
     */
    void plot();
};

#endif