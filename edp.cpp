#include "edp.hpp"

#include "math.h"

/**
 * @brief Fonction retournant le coefficient a de l'EDP complète
 * @return double 
 */
double CompletePDE::get_coeff_a() const { return 1.0; }

/**
 * @brief Fonction retournant le coefficient a de l'EDP complète
 * @param s Position s
 * @return double 
 */
double CompletePDE::get_coeff_b(double s) const { return 0.5 * pow((option->sigma), 2.0) * pow(s, 2.0); }

/**
 * @brief Fonction retournant le coefficient c de l'EDP complète
 * @param s Position s
 * @return double 
 */
double CompletePDE::get_coeff_c(double s) const { return (option->r) * s; }

/**
 * @brief Fonction retournant le coefficient d de l'EDP complète
 * @return double 
 */
double CompletePDE::get_coeff_d() const { return -(option->r); }

/**
 * @brief Fonction retournant le coefficient a de l'EDP réduite
 * @return double 
 */
double ReducedPDE::get_coeff_a() const { return 1.0; }

/**
 * @brief Fonction retournant le coefficient a de l'EDP réduite
 * @return double 
 */
double ReducedPDE::get_coeff_b() const { return -0.5 * pow(option->sigma, 2); }


/**
 * @brief Fonction retournant la condition au bord basse pour l'EDP complète
 * @param t Instant t
 * @return double 
 */
double CompletePDE::get_cdt_bord_b(double t) const {
    if (option->payoff->get_payofftype() == Payofftype::call)
        return 0.0;   // La payoff implémenté est un CALL
    else
        return (option->K) * exp(-(option->r) * (t - option->T));   // Le payoff implémenté est un PUT
}

/**
 * @brief Fonction retournant la condition au bord haute pour l'EDP complète
 * @param t Instant t
 * @param s Position s
 * @return double 
 */
double CompletePDE::get_cdt_bord_h(double t, double s) const {
    if (option->payoff->get_payofftype() == Payofftype::call)
        return s - (option->K) * exp(-(option->r) * (t - option->T));   // La payoff implémenté est un CALL
    else
        return 0.0;   // Le payoff implémenté est un PUT
}

/**
 * @brief Fonction retournant le payoff pour l'EDP complète
 * @param s Position s
 * @return double 
 */
double CompletePDE::get_cdt_term(double s) const { 
    return option->payoff->operator()(s); 
}

/**
 * @brief Fonction retournant la condition au bord basse pour l'EDP réduite
 * @param t Instant t
 * @return double 
 */
double ReducedPDE::get_cdt_bord_b(double t) const {
    if (option->payoff->get_payofftype() == Payofftype::call)
        return 0.0;   // La payoff implémenté est un CALL
    else
        return (option->K) * exp(-(option->r) * (t - option->T));   // Le payoff implémenté est un PUT
}

/**
 * @brief Fonction retournant la condition au bord haute pour l'EDP réduite
 * @param t Instant t
 * @param s Position s
 * @return double 
 */
double ReducedPDE::get_cdt_bord_h(double t, double s) const {
    if (option->payoff->get_payofftype() == Payofftype::call)
        return s - (option->K) * exp(-(option->r) * (t - option->T));   // La payoff implémenté est un CALL
    else
        return 0.0;   // Le payoff implémenté est un PUT
}

/**
 * @brief Fonction retournant le payoff pour l'EDP réduite
 * @param s Position s
 * @return double 
 */
double ReducedPDE::get_cdt_term(double s) const {
    return option->payoff->operator()(s);
}