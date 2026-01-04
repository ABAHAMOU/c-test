#ifndef _OPTION_HPP_
#define _OPTION_HPP_

#include "payoff.hpp"

/**
 * @class Option
 * @brief Classe représentant une option financière européenne
 * 
 * Cette classe encapsule tous les paramètres nécessaires pour définir
 * une option européenne (Call ou Put) dans le modèle de Black-Scholes.
 */
class Option {
public:
    double L;           ///< Longueur du domaine spatial
    double r;           ///< Taux d'intérêt sans risque
    double K;           ///< Prix d'exercice (strike)
    double T;           ///< Maturité de l'option
    double sigma;       ///< Volatilité du sous-jacent
    Payoff* payoff;     ///< Pointeur vers le payoff de l'option

public:
    /**
     * @brief Constructeur de la classe Option
     * @param T_ Maturité de l'option
     * @param r_ Taux d'intérêt sans risque
     * @param K_ Prix d'exercice (strike)
     * @param sigma_ Volatilité du sous-jacent
     * @param L_ Longueur du domaine spatial
     * @param payoff_ Pointeur vers l'objet Payoff
     */
    Option(const double T_, const double r_, const double K_, const double sigma_, 
           const double L_, Payoff* payoff_) {
        T = T_;
        r = r_;
        K = K_;
        payoff = payoff_;
        L = L_;
        sigma = sigma_;
    }

    /**
     * @brief Retourne la maturité de l'option
     * @return Maturité T
     */
    double get_T() const { return T; }
    
    /**
     * @brief Retourne le taux d'intérêt sans risque
     * @return Taux r
     */
    double get_r() const { return r; }
    
    /**
     * @brief Retourne le prix d'exercice
     * @return Strike K
     */
    double get_K() const { return K; }
    
    /**
     * @brief Retourne la volatilité
     * @return Volatilité sigma
     */
    double get_sigma() const { return sigma; }
    
    /**
     * @brief Retourne la longueur du domaine spatial
     * @return Longueur L
     */
    double get_L() const { return L; }
    
    /**
     * @brief Retourne le pointeur vers le payoff
     * @return Pointeur vers l'objet Payoff
     */
    Payoff* get_payoff() const { return payoff; }
};

#endif