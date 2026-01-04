#ifndef _EDP_HPP_
#define _EDP_HPP_

#include "option.hpp"

/**
 * @file edp.hpp
 * @brief Classes pour la résolution de l'EDP de Black-Scholes
 */

/**
 * @class PDE
 * @brief Classe abstraite représentant une EDP
 */
class PDE {
protected:
    Option* option;     ///< Option associée à l'EDP

public:
    /**
     * @brief Constructeur
     * @param option_ Pointeur vers l'option
     */
    PDE(Option* option_) : option(option_) {}
    
    /**
     * @brief Retourne le pointeur vers l'option
     */
    Option* get_option() const { return option; }
    
    /**
     * @brief Coefficient a de l'EDP
     */
    virtual double get_coeff_a() const = 0;
    
    /**
     * @brief Condition au bord basse
     * @param t Instant temporel
     */
    virtual double get_cdt_bord_b(double t) const = 0;
    
    /**
     * @brief Condition au bord haute
     * @param t Instant temporel
     * @param s Position spatiale
     */
    virtual double get_cdt_bord_h(double t, double s) const = 0;
    
    /**
     * @brief Condition terminale (payoff)
     * @param s Prix du sous-jacent
     */
    virtual double get_cdt_term(double s) const = 0;
    
    virtual ~PDE() {}
};

/**
 * @class CompletePDE
 * @brief EDP de Black-Scholes complète
 */
class CompletePDE : public PDE {
public:
    /**
     * @brief Constructeur
     * @param option_ Pointeur vers l'option
     */
    CompletePDE(Option* option_) : PDE(option_) {}

    /**
     * @brief Coefficient a (= 1.0)
     */
    double get_coeff_a() const override;
    
    /**
     * @brief Coefficient b = 0.5 * σ² * s²
     * @param s Position spatiale
     */
    double get_coeff_b(double s) const;
    
    /**
     * @brief Coefficient c = r * s
     * @param s Position spatiale
     */
    double get_coeff_c(double s) const;
    
    /**
     * @brief Coefficient d = -r
     */
    double get_coeff_d() const;

    /**
     * @brief Condition au bord basse
     * @param t Instant temporel
     */
    double get_cdt_bord_b(double t) const override;
    
    /**
     * @brief Condition au bord haute
     * @param t Instant temporel
     * @param s Position spatiale
     */
    double get_cdt_bord_h(double t, double s) const override;
    
    /**
     * @brief Condition terminale
     * @param s Prix du sous-jacent
     */
    double get_cdt_term(double s) const override;
};

/**
 * @class ReducedPDE
 * @brief EDP de Black-Scholes réduite
 */
class ReducedPDE : public PDE {
public:
    /**
     * @brief Constructeur
     * @param option_ Pointeur vers l'option
     */
    ReducedPDE(Option* option_) : PDE(option_) {}

    /**
     * @brief Coefficient a (= 1.0)
     */
    double get_coeff_a() const override;
    
    /**
     * @brief Coefficient b = -0.5 * σ²
     */
    double get_coeff_b() const;

    /**
     * @brief Condition au bord basse
     * @param t Instant temporel
     */
    double get_cdt_bord_b(double t) const override;
    
    /**
     * @brief Condition au bord haute
     * @param t Instant temporel
     * @param s Position spatiale
     */
    double get_cdt_bord_h(double t, double s) const override;
    
    /**
     * @brief Condition terminale
     * @param s Prix du sous-jacent
     */
    double get_cdt_term(double s) const override;
};

#endif