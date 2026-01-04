#ifndef _PAYOFF_HPP_
#define _PAYOFF_HPP_

#include <algorithm>

/**
 * @file payoff.hpp
 * @brief Classes pour les payoffs d'options
 */

/**
 * @enum Payofftype
 * @brief Types de payoff disponibles
 */
enum class Payofftype
{
    put = 1, ///< Option de vente
    call = 0 ///< Option d'achat
};

/**
 * @class Payoff
 * @brief Classe abstraite représentant le payoff d'une option
 */
class Payoff
{
public:
    /**
     * @brief Retourne le type de payoff
     * @return Type de payoff (call ou put)
     */
    virtual Payofftype get_payofftype() const = 0;

    /**
     * @brief Calcule la valeur du payoff
     * @param s Prix du sous-jacent
     * @return Valeur du payoff
     */
    virtual double operator()(const double &s) const = 0;

    /**
     * @brief Destructeur virtuel
     */
    virtual ~Payoff() {}
};

/**
 * @class Call
 * @brief Payoff d'une option Call européenne
 *
 * Payoff: max(S - K, 0)
 */
class Call : public Payoff
{
private:
    double K;        ///< Prix d'exercice
    Payofftype type; ///< Type de payoff

public:
    /**
     * @brief Constructeur
     * @param K_ Prix d'exercice (défaut: 0.0)
     */
    Call(const double &K_ = 0.0)
    {
        K = K_;
        type = Payofftype::call;
    }

    /**
     * @brief Retourne le type de payoff
     * @return Payofftype::call
     */
    Payofftype get_payofftype() const override
    {
        return type;
    }

    /**
     * @brief Calcule le payoff du Call
     * @param s Prix du sous-jacent
     * @return max(s - K, 0)
     */
    virtual double operator()(const double &s) const override
    {
        return std::max(s - K, 0.0);
    }
};

/**
 * @class Put
 * @brief Payoff d'une option Put européenne
 *
 * Payoff: max(K - S, 0)
 */
class Put : public Payoff
{
private:
    double K;        ///< Prix d'exercice
    Payofftype type; ///< Type de payoff

public:
    /**
     * @brief Constructeur
     * @param K_ Prix d'exercice
     */
    Put(const double &K_)
    {
        K = K_;
        type = Payofftype::put;
    }

    /**
     * @brief Retourne le type de payoff
     * @return Payofftype::put
     */
    Payofftype get_payofftype() const override
    {
        return type;
    }

    /**
     * @brief Calcule le payoff du Put
     * @param s Prix du sous-jacent
     * @return max(K - s, 0)
     */
    virtual double operator()(const double &s) const override
    {
        return std::max(K - s, 0.0);
    }
};

#endif