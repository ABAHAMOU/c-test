#include <fstream>
#include <iostream>
#include <vector>

#include "finitedifference.hpp"
#include "mesh.hpp"
#include "option.hpp"
#include "edp.hpp"
#include "payoff.hpp"
#include "sdl.hpp"
#include "functions.hpp"
#include "math.h"

/**
 * @file main.cpp
 * @brief Programme main pour la résolution de l'EDP de Black-Scholes
 */

/**
 * @brief Fonction principale du programme
 * @return Code de sortie (0 si succès)
 */
int main()
{

    // Paramètres de discrétisation
    const int M = 1000;     // Intervalles temporels
    const int N = 1000;     // Intervalles spatiaux
    const double T = 1.0;   // Maturité
    const double L = 300.0; // Domaine spatial

    // Paramètres du modèle de Black-Scholes
    const double K = 100.0;   // Strike
    const double r = 0.1;     // Taux sans risque
    const double sigma = 0.1; // Volatilité

    // Création des payoffs et options
    Payoff *payoff_put = new Put(K);
    Payoff *payoff_call = new Call(K);
    Option *option_put = new Option(T, r, K, sigma, L, payoff_put);
    Option *option_call = new Option(T, r, K, sigma, L, payoff_call);

    // Initialisation des EDP
    ReducedPDE *pde_r_put = new ReducedPDE(option_put);
    CompletePDE *pde_c_put = new CompletePDE(option_put);
    ReducedPDE *pde_r_call = new ReducedPDE(option_call);
    CompletePDE *pde_c_call = new CompletePDE(option_call);

    // Création des solveurs
    std::cout << "Initialisation des solveurs..." << std::endl;
    IMFD imfd_put(pde_r_put, M, N, L, T);
    CrankNicholsonFD cnfd_put(pde_c_put, M, N, L, T);
    IMFD imfd_call(pde_r_call, M, N, L, T);
    CrankNicholsonFD cnfd_call(pde_c_call, M, N, L, T);

    // Résolution des EDP
    std::cout << "Calcul des solutions numériques..." << std::endl;
    imfd_put.compute_solution();
    cnfd_put.compute_solution();
    imfd_call.compute_solution();
    cnfd_call.compute_solution();

    // Export des résultats en CSV
    std::cout << "Sauvegarde des données..." << std::endl;
    imfd_put.safe_csv("data_imfd_put.csv");
    cnfd_put.safe_csv("data_cnfd_put.csv");
    imfd_call.safe_csv("data_imfd_call.csv");
    cnfd_call.safe_csv("data_cnfd_call.csv");

    // Calcul des erreurs entre méthodes
    std::vector<double> diff_put = compute_diff_vector(imfd_put.C, cnfd_put.C);
    std::vector<double> diff_call = compute_diff_vector(imfd_call.C, cnfd_call.C);
    // Calcul et affichage des statistiques d'erreur
    std::cout << "\n=== STATISTIQUES D'ERREUR ===" << std::endl;

    double mean_error_put = compute_mean_error(diff_put);

    double mean_error_call = compute_mean_error(diff_call);

    std::cout << "Option PUT :" << std::endl;
    std::cout << "  - Erreur moyenne  : " << mean_error_put << std::endl;

    std::cout << "\nOption CALL :" << std::endl;
    std::cout << "  - Erreur moyenne  : " << mean_error_call << std::endl;

    std::cout << "\nRésumé :" << std::endl;
    std::cout << "  - Moyenne globale : " << (mean_error_put + mean_error_call) / 2.0 << std::endl;
    // Configuration de l'affichage graphique
    std::cout << "Préparation de l'affichage graphique..." << std::endl;
    Sdl *display = new Sdl();

    // Fenêtre 1: Comparaison des solutions pour PUT
    display->w1 = new Window(L, "PUT - Superposition des solutions");
    display->w1->set_x(imfd_put.s->get_vector());
    display->w1->set_y1(cnfd_put.C); // Crank-Nicholson
    display->w1->set_y2(imfd_put.C); // Implicite 

    // Fenêtre 2: Erreur numérique pour PUT
    display->w2 = new Window(L, "PUT - Erreur numérique");
    display->w2->set_x(imfd_put.s->get_vector());
    display->w2->set_y2(diff_put);

    // Fenêtre 3: Comparaison des solutions pour CALL
    display->w3 = new Window(L, "CALL - Superposition des solutions");
    display->w3->set_x(imfd_call.s->get_vector());
    display->w3->set_y1(cnfd_call.C); 
    display->w3->set_y2(imfd_call.C); 

    // Fenêtre 4: Erreur numérique pour CALL
    display->w4 = new Window(L, "CALL - Erreur numérique");
    display->w4->set_x(imfd_call.s->get_vector());
    display->w4->set_y2(diff_call);

    // Affichage et attente de fermeture
    std::cout << "Affichage des résultats..." << std::endl;
    display->show();
    display->quit();

    // Nettoyage mémoire
    delete payoff_put;
    delete payoff_call;
    delete option_put;
    delete option_call;
    delete pde_r_put;
    delete pde_c_put;
    delete pde_r_call;
    delete pde_c_call;
    delete display;

    std::cout << "\nProgramme terminé avec succès." << std::endl;
    return 0;
}