#include "functions.hpp"

/**
 * @brief Calcule l'erreur moyenne dans un vecteur d'erreurs
 * @param error_vec Vecteur d'erreurs absolues
 * @return Erreur moyenne
 */
double compute_mean_error(const std::vector<double>& error_vec) {
    if (error_vec.empty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (double error : error_vec) {
        sum += error;
    }
    return sum / error_vec.size();
}




void print_vector(std::vector<double> v) {
        int size = v.size();
        for (int i = 0; i < size; i++)
                std::cout << v[i] << ' ';
        std::cout << "fin" << std::endl;
}

std::vector<double> compute_diff_vector(std::vector<double> a, std::vector<double> b) {
        if (a.size() != b.size())
                throw "Taille invalide";
        int n = b.size();
        std::vector<double> diff(n, 0.0);  
        for (int i = 0; i < n; i++)
                diff[i] = abs(a[i] - b[i]);
        return diff;
}