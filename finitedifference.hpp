#ifndef _FINITE_DIFFERENCE_HPP
#define _FINITE_DIFFERENCE_HPP

#include <vector>

#include "mesh.hpp"
#include "edp.hpp"
#include "math.h"
#define THRESHOLD_MIN 1e-8

/**
 * @file FiniteDifference.hpp
 * @brief Classes pour la résolution d'EDP par différences finies
 */

/**
 * @typedef Matrix
 * @brief Alias pour manipuler des matrices sous forme de double**
 */
typedef double** Matrix;

/**
 * @class FiniteDifference
 * @brief Classe abstraite définissant l'interface des méthodes de différences finies
 */
class FiniteDifference {
    virtual void set_mesh() = 0;
    virtual void set_matrix_coefficients() = 0;
    virtual void set_coefficients_M1() = 0;
    virtual void set_terminal_condition() = 0;
    virtual void compute_solution() = 0;
    virtual void compute_vector_k(int m) = 0;
    virtual void compute_RHS_member(Matrix M, std::vector<double> v) = 0;
    virtual std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, 
                                           std::vector<double> c_, std::vector<double> d_) = 0;
    virtual void safe_csv(const char* file_title) = 0;
};

/**
 * @class IMFD
 * @brief Méthode Implicite des Différences Finies pour l'EDP réduite
 * 
 * Cette classe implémente le schéma implicite pour résoudre l'EDP de
 * Black-Scholes réduite. Le schéma est inconditionnellement stable.
 */
class IMFD : public FiniteDifference {
public:
    ReducedPDE* pde;   // EDP réduite à résoudre
    int M;             // Nombre d'intervalles temporels
    int N;             // Nombre d'intervalles spatiaux
    double T;          // Largeur du domaine temporel
    double L;          // Largeur du domaine spatial

public:
    /**
     * @brief Constructeur du résolveur implicite
     * @param pde_ EDP réduite
     * @param M_ Nombre d'intervalles temporels
     * @param N_ Nombre d'intervalles spatiaux
     * @param L_ Longueur du domaine spatial
     * @param T_ Longueur du domaine temporel
     */
    IMFD(ReducedPDE* pde_, int M_, int N_, double L_, double T_);

public:
    double r;           // Taux sans risque
    double sigma;       // Volatilité

    Mesh* t;            // Discrétisation temporelle
    Mesh* s;            // Discrétisation spatiale

    double dt;          // Pas temporel
    double ds;          // Pas spatial
    double mu;          // Paramètre mu = -coeff_b

    std::vector<double> a;      // Diagonale inférieure
    std::vector<double> b;      // Diagonale principale
    std::vector<double> c;      // Diagonale supérieure
    std::vector<double> C;      // Vecteur solution
    Matrix M1;                  // Matrice du membre de droite

    std::vector<double> k;      // Vecteur des conditions aux bords
    std::vector<double> RHS;    // Membre de droite du système

public:
    /**
     * @brief Crée la discrétisation temporelle et spatiale
     */
    void set_mesh();
    
    /**
     * @brief Calcule et stocke les coefficients a, b, c
     */
    void set_matrix_coefficients();
    
    /**
     * @brief Remplit la matrice M1 avec les coefficients
     */
    void set_coefficients_M1();
    
    /**
     * @brief Applique la condition terminale sur le vecteur C
     */
    void set_terminal_condition();
    
    /**
     * @brief Calcule la solution numérique de l'EDP
     * 
     * Résout l'EDP en remontant dans le temps avec le schéma implicite.
     */
    void compute_solution();
    
    /**
     * @brief Calcule le vecteur des conditions aux bords à l'instant t_m
     * @param m Indice temporel
     */
    void compute_vector_k(int m);
    
    /**
     * @brief Calcule le membre de droite RHS = M * v + k
     * @param M Matrice
     * @param v Vecteur solution actuel
     */
    void compute_RHS_member(Matrix M, std::vector<double> v);
    
    /**
     * @brief Résout le système tridiagonal par l'algorithme de Thomas
     * @param a_ Diagonale inférieure
     * @param b_ Diagonale principale
     * @param c_ Diagonale supérieure
     * @param d_ Membre de droite
     * @return Vecteur solution
     */
    std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, 
                                   std::vector<double> c_, std::vector<double> d_);
    
    /**
     * @brief Enregistre les résultats dans un fichier CSV
     * @param file_title Nom du fichier de sortie
     */
    void safe_csv(const char* file_title);
};

/**
 * @class CrankNicholsonFD
 * @brief Méthode de Crank-Nicholson pour l'EDP complète
 * 
 * Cette classe implémente le schéma de Crank-Nicholson pour résoudre
 * l'EDP de Black-Scholes complète. C'est un schéma d'ordre 2 en temps.
 */
class CrankNicholsonFD : public FiniteDifference {
public:
    CompletePDE* pde;  // EDP complète à résoudre
    int M;             // Nombre d'intervalles temporels
    int N;             // Nombre d'intervalles spatiaux
    double T;          // Largeur du domaine temporel
    double L;          // Largeur du domaine spatial

public:
    /**
     * @brief Constructeur du résolveur Crank-Nicholson
     * @param pde_ EDP complète
     * @param M_ Nombre d'intervalles temporels
     * @param N_ Nombre d'intervalles spatiaux
     * @param L_ Longueur du domaine spatial
     * @param T_ Longueur du domaine temporel
     */
    CrankNicholsonFD(CompletePDE* pde_, int M_, int N_, double L_, double T_);

public:
    double r;           // Taux sans risque
    double sigma;       // Volatilité

    Mesh* t;            // Discrétisation temporelle
    Mesh* s;            // Discrétisation spatiale

    double dt;          // Pas temporel
    double ds;          // Pas spatial

    std::vector<double> a;      // Coefficients pour M1
    std::vector<double> b;      // Coefficients pour M1
    std::vector<double> c;      // Coefficients pour M1
    std::vector<double> d;      // Coefficients pour M2
    std::vector<double> e;      // Coefficients pour M2 (= -a)
    std::vector<double> f;      // Coefficients pour M2 (= -c)
    std::vector<double> C;      // Vecteur solution
    Matrix M1;                  // Matrice du membre de droite
    Matrix M2;                  // Matrice du membre de gauche

    std::vector<double> k;      // Vecteur des conditions aux bords
    std::vector<double> RHS;    // Membre de droite du système

public:
    /**
     * @brief Crée la discrétisation temporelle et spatiale
     */
    void set_mesh();
    
    /**
     * @brief Calcule et stocke les coefficients a, b, c, d, e, f
     */
    void set_matrix_coefficients();
    
    /**
     * @brief Remplit la matrice M1 avec les coefficients a, b, c
     */
    void set_coefficients_M1();
    
    /**
     * @brief Remplit la matrice M2 avec les coefficients d, e, f
     */
    void set_coefficients_M2();
    
    /**
     * @brief Applique la condition terminale sur le vecteur C
     */
    void set_terminal_condition();
    
    /**
     * @brief Calcule la solution numérique de l'EDP
     * 
     * Résout l'EDP en remontant dans le temps avec le schéma de Crank-Nicholson.
     */
    void compute_solution();
    
    /**
     * @brief Calcule le vecteur des conditions aux bords à l'instant t_m
     * @param m Indice temporel
     */
    void compute_vector_k(int m);
    
    /**
     * @brief Calcule le membre de droite RHS = M * v + k
     * @param M Matrice
     * @param v Vecteur solution actuel
     */
    void compute_RHS_member(Matrix M, std::vector<double> v);
    
    /**
     * @brief Résout le système tridiagonal par l'algorithme de Thomas
     * @param a_ Diagonale inférieure
     * @param b_ Diagonale principale
     * @param c_ Diagonale supérieure
     * @param d_ Membre de droite
     * @return Vecteur solution
     */
    std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, 
                                   std::vector<double> c_, std::vector<double> d_);
    
    /**
     * @brief Enregistre les résultats dans un fichier CSV
     * @param file_title Nom du fichier de sortie
     */
    void safe_csv(const char* file_title);
};

#endif