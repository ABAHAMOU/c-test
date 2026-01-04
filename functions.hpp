#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <iostream>
#include <vector>

/**
 * @file functions.h
 * @brief Déclaration des fonctions auxiliaires pour le projet
 */

/**
 * @brief Affiche le contenu d'un vecteur sur la sortie standard
 * @param v Vecteur de doubles à afficher
 */
void print_vector(std::vector<double> v);

/**
 * @brief Calcule le vecteur de différences absolues entre deux vecteurs
 * 
 * Cette fonction est utilisée pour calculer l'erreur entre la résolution
 * de l'EDP complète et de l'EDP réduite.
 * 
 * @param a Premier vecteur de doubles
 * @param b Second vecteur de doubles
 * @return Vecteur contenant les différences absolues |a[i] - b[i]|
 * @throws const char* Si les vecteurs n'ont pas la même taille
 */
std::vector<double> compute_diff_vector(std::vector<double> a, std::vector<double> b);


/**
 * @brief Calcule l'erreur moyenne dans un vecteur d'erreurs
 * @param error_vec Vecteur d'erreurs absolues
 * @return Erreur moyenne
 */
double compute_mean_error(const std::vector<double>& error_vec);  // DÉCLARATION seulement

#endif