#include "mesh.hpp"

#include <stdexcept>

Mesh::Mesh(double a_, int size_) {
    if (size_ <= 0)
        throw std::invalid_argument("Taille invalide");
    
    size = size_ + 1;
    a = a_;
    data = new double[size];

    if (!data)
        throw "Echec de l'allocation de mémoire";

    for (int i = 0; i < size; i++)
        data[i] = i * ((double)a / (double)size);
}

Mesh::~Mesh() {
    delete[] data;
}

double Mesh::operator[](int i) const {
    if ((i < 0) || (i > size))
        throw std::invalid_argument("Index invalide");
    return data[i];
}

std::ostream& operator<<(std::ostream& st, const Mesh& m) {
    st << "[";
    for (int i = 0; i < m.get_size(); i++) {
        st << m[i];
        if (i != (m.get_size() - 1))
            st << ";";
    }
    st << "]";
    return st;
}

std::vector<double> Mesh::get_vector() {
    int n = size;
    std::vector<double> res(n + 1, 0.0);
    for (int i = 0; i < n; i++)
        res[i] = data[i];
    return res;
}