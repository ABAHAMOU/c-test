#ifndef _MESH_HPP
#define _MESH_HPP

#include <iostream>
#include <vector>

/**
 * @file mesh.hpp
 * @brief Classe pour la discrétisation spatiale et temporelle
 */

/**
 * @class Mesh
 * @brief Discrétisation d'un domaine spatial ou temporel
 */
class Mesh
{
private:
    double *data; ///< Tableau contenant la discrétisation
    double a;     ///< Largeur de l'intervalle
    int size;     ///< Nombre de subdivisions

public:
    /**
     * @brief Constructeur créant une discrétisation uniforme
     * @param a_ Largeur de l'intervalle [0, a_]
     * @param size_ Nombre d'intervalles de discrétisation
     * @throws std::invalid_argument Si size_ <= 0
     * @throws const char* Si l'allocation mémoire échoue
     */
    Mesh(double a_, int size_);

    /**
     * @brief Destructeur libérant la mémoire allouée
     */
    ~Mesh();

    /**
     * @brief Retourne la largeur de l'intervalle
     * @return Largeur a
     */
    double get_interval() const { return a; }

    /**
     * @brief Retourne le nombre total de points
     * @return Nombre de points (size)
     */
    int get_size() const { return size; }

    /**
     * @brief Retourne le pas de discrétisation
     * @return Pas = a / size
     */
    double get_step() const { return (double)(a / size); }

    /**
     * @brief Convertit la discrétisation en std::vector
     * @return Vecteur contenant tous les points de discrétisation
     */
    std::vector<double> get_vector();

    /**
     * @brief Accès en lecture à un élément par index
     * @param i Index de l'élément (0 <= i < size)
     * @return Valeur du point à l'index i
     * @throws std::invalid_argument Si l'index est invalide
     */
    double operator[](int i) const;

    /**
     * @brief Surcharge de l'opérateur << pour l'affichage
     * @param st Flux de sortie
     * @param m Objet Mesh à afficher
     * @return Référence vers le flux de sortie
     */
    friend std::ostream &operator<<(std::ostream &st, const Mesh &m);
};

#endif