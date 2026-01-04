#include "finitedifference.hpp"

#include <fstream>

//shéma implicite

IMFD::IMFD(ReducedPDE* pde_, int M_, int N_, double L_, double T_) 
    : pde(pde_), M(M_), N(N_), T(T_), L(L_) {
    
    r = pde->get_option()->r;
    sigma = pde->get_option()->sigma;

    set_mesh();
    dt = t->get_step();
    ds = s->get_step();
    mu = -pde->get_coeff_b();

    a.resize(N - 1, 0.0);
    b.resize(N - 1, 0.0);
    c.resize(N - 1, 0.0);

    set_matrix_coefficients();

    // Création de la matrice M1
    M1 = new double*[N - 1];
    for (int i = 0; i < N - 1; i++)
        M1[i] = new double[N - 1];

    // Initialisation de M1
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            M1[i][j] = 0.0;
        }
    }

    set_coefficients_M1();

    C.resize(N - 1, 0.0);
    set_terminal_condition();

    k.resize(N - 1, 0.0);
    RHS.resize(N - 1, 0.0);
}

void IMFD::compute_solution() {
    // Boucle temporelle
    for (int m = 0; m < M; m++) {
        compute_vector_k(m);
        compute_RHS_member(M1, C);
        C = thomas_algo(a, b, c, RHS);
    }
    
    // Ajout des conditions aux bords
    C.insert(C.begin(), pde->get_cdt_bord_b((*t)[0]));
}

void IMFD::set_mesh() {
    t = new Mesh(T, M);
    s = new Mesh(L, N);
}

void IMFD::set_matrix_coefficients() {
    double alpha = (mu * dt) / (double)pow(ds, 2.0);
    for (int i = 0; i < N - 1; i++) {
        a[i] = -alpha;
        b[i] = 1 + 2 * alpha;
        c[i] = -alpha;
    }
}

void IMFD::set_coefficients_M1() {
    // Diagonale principale
    for (int i = 0; i < N - 1; i++)
        M1[i][i] = b[i];

    // Diagonales adjacentes
    for (int i = 0; i < N - 2; i++) {
        M1[i][i + 1] = c[i];
        M1[i + 1][i] = a[i + 1];
    }
}

void IMFD::set_terminal_condition() {
    for (int j = 0; j < N - 1; j++) {
        C[j] = pde->get_cdt_term((*s)[j + 1]);
    }
}

void IMFD::compute_vector_k(int m) {
    k[0] = a[0] * (pde->get_cdt_bord_b((*t)[m]));
    k[N - 2] = c[N - 2] * (pde->get_cdt_bord_h((*t)[m], (*s)[N - 2]));
}

void IMFD::compute_RHS_member(Matrix M, std::vector<double> v) {
    for (int i = 0; i < N - 1; i++) {
        double tmp = 0.0;
        for (int j = 0; j < N - 1; j++) {
            tmp = tmp + M[i][j] * v[j];
        }
        RHS[i] = tmp + k[i];
    }
}

std::vector<double> IMFD::thomas_algo(std::vector<double> a_, std::vector<double> b_, 
                                      std::vector<double> c_, std::vector<double> d_) {
    int n = d_.size();
    n--;
    c_[0] /= b_[0];
    d_[0] /= b_[0];

    for (int i = 1; i < n; i++) {
        c_[i] /= b_[i] - a_[i] * c_[i - 1];
        d_[i] = (d_[i] - a_[i] * d_[i - 1]) / (b_[i] - a_[i] * c_[i - 1]);
    }

    d_[n] = (d_[n] - a_[n] * d_[n - 1]) / (b_[n] - a_[n] * c_[n - 1]);

    for (int i = n; i-- > 0;) {
        d_[i] -= c_[i] * d_[i + 1];
    }
    return d_;
}

void IMFD::safe_csv(const char* file_title) {
    std::ofstream f_out(file_title);
    f_out << "s;c" << std::endl;
    for (int j = 0; j < N; j++) {
        f_out << (*s)[j] << ";" << C[j] << std::endl;
    }
    f_out.close();
}

//shéma CrankNicholsonFD

CrankNicholsonFD::CrankNicholsonFD(CompletePDE* pde_, int M_, int N_, double L_, double T_) 
    : pde(pde_), M(M_), N(N_), T(T_), L(L_) {
    
    r = pde->get_option()->r;
    sigma = pde->get_option()->sigma;

    set_mesh();
    dt = t->get_step();
    ds = s->get_step();

    a.resize(N - 1, 0.0);
    b.resize(N - 1, 0.0);
    c.resize(N - 1, 0.0);
    d.resize(N - 1, 0.0);
    e.resize(N - 1, 0.0);
    f.resize(N - 1, 0.0);

    set_matrix_coefficients();

    // Création de M1
    M1 = new double*[N - 1];
    for (int i = 0; i < N - 1; i++)
        M1[i] = new double[N - 1];

    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            M1[i][j] = 0.0;
        }
    }

    // Création de M2
    M2 = new double*[N - 1];
    for (int i = 0; i < N - 1; i++)
        M2[i] = new double[N - 1];

    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            M2[i][j] = 0.0;
        }
    }

    set_coefficients_M1();
    set_coefficients_M2();

    C.resize(N - 1, 0.0);
    set_terminal_condition();

    k.resize(N - 1, 0.0);
    RHS.resize(N - 1, 0.0);
}

void CrankNicholsonFD::compute_solution() {
    // Boucle temporelle
    for (int m = M; m > 0; m--) {
        compute_vector_k(m);
        compute_RHS_member(M1, C);
        C = thomas_algo(e, d, f, RHS);
    }
    
    // Ajout des conditions aux bords
    C.insert(C.begin(), pde->get_cdt_bord_b((*t)[0]));
}

void CrankNicholsonFD::set_mesh() {
    t = new Mesh(T, M);
    s = new Mesh(L, N);
}

void CrankNicholsonFD::set_matrix_coefficients() {
    for (int i = 0; i < N - 1; i++) {
        int j = i + 1;
        a[i] = 0.25 * j * dt * (pow(sigma, 2) * j - r);
        b[i] = 1 - 0.5 * (pow(sigma, 2) * pow(j, 2) * dt);
        c[i] = 0.25 * j * dt * (pow(sigma, 2) * j + r);
        d[i] = 1 + 0.5 * (pow(sigma, 2) * pow(j, 2) * dt) + r * dt;
        e[i] = -a[i];
        f[i] = -c[i];
    }
}

void CrankNicholsonFD::set_coefficients_M1() {
    // Diagonale principale
    for (int i = 0; i < N - 1; i++)
        M1[i][i] = b[i];

    // Diagonales adjacentes
    for (int i = 0; i < N - 2; i++) {
        M1[i][i + 1] = c[i];
        M1[i + 1][i] = a[i + 1];
    }
}

void CrankNicholsonFD::set_coefficients_M2() {
    // Diagonale principale
    for (int i = 0; i < N - 1; i++)
        M2[i][i] = d[i];

    // Diagonales adjacentes
    for (int i = 0; i < N - 2; i++) {
        M2[i][i + 1] = -c[i];
        M2[i + 1][i] = -a[i + 1];
    }
}

void CrankNicholsonFD::set_terminal_condition() {
    for (int j = 0; j < N - 1; j++) {
        C[j] = pde->get_cdt_term((*s)[j + 1]);
    }
}

void CrankNicholsonFD::compute_vector_k(int m) {
    k[0] = a[0] * (pde->get_cdt_bord_b((*t)[m]) + pde->get_cdt_bord_b((*t)[m + 1]));
    k[N - 2] = c[N - 2] * (pde->get_cdt_bord_h((*t)[m], (*s)[N - 2]) + 
                           pde->get_cdt_bord_h((*t)[m + 1], (*s)[N - 2]));
}

void CrankNicholsonFD::compute_RHS_member(Matrix M, std::vector<double> v) {
    for (int i = 0; i < N - 1; i++) {
        double tmp = 0.0;
        for (int j = 0; j < N - 1; j++) {
            tmp = tmp + M[i][j] * v[j];
        }
        RHS[i] = tmp + k[i];
    }
}

std::vector<double> CrankNicholsonFD::thomas_algo(std::vector<double> a_, std::vector<double> b_, 
                                                  std::vector<double> c_, std::vector<double> d_) {
    int n = d_.size();
    n--;
    c_[0] /= b_[0];
    d_[0] /= b_[0];

    for (int i = 1; i < n; i++) {
        c_[i] /= b_[i] - a_[i] * c_[i - 1];
        d_[i] = (d_[i] - a_[i] * d_[i - 1]) / (b_[i] - a_[i] * c_[i - 1]);
    }

    d_[n] = (d_[n] - a_[n] * d_[n - 1]) / (b_[n] - a_[n] * c_[n - 1]);

    for (int i = n; i-- > 0;) {
        d_[i] -= c_[i] * d_[i + 1];
    }
    return d_;
}

void CrankNicholsonFD::safe_csv(const char* file_title) {
    std::ofstream f_out(file_title);
    f_out << "s;c" << std::endl;
    for (int j = 0; j < N; j++) {
        f_out << (*s)[j] << ";" << C[j] << std::endl;
    }
    f_out.close();
}