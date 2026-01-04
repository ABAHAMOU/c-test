#include "window.hpp"
#include <algorithm>

int Window::window_id = 0;

Window::Window(double L_, const char* window_title_, int window_width_, int window_height_)
    : window_title(window_title_), window_width(window_width_),
      window_height(window_height_), L(L_) {

    window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        printf("Erreur de création de fenêtre : %s\n", SDL_GetError());
        throw "Erreur";
    }

    renderer = SDL_CreateRenderer(window, 0,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    alpha = 0.08;  // Slightly larger margins
    window_id++;
    clear();
}

Window::~Window() {
    window_id--;
}

void Window::clear() {
    // Dark blue-gray background instead of pure black
    SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x2E, 0xFF);
    SDL_RenderClear(renderer);
}

void Window::set_x(std::vector<double> x_) {
    x = x_;
}

void Window::set_y1(std::vector<double> y1_) {
    y1 = y1_;
    y_min_1 = *std::min_element(y1.begin(), y1.end());
    y_max_1 = *std::max_element(y1.begin(), y1.end());
}

void Window::set_y2(std::vector<double> y2_) {
    y2 = y2_;
    y_min_2 = *std::min_element(y2.begin(), y2.end());
    y_max_2 = *std::max_element(y2.begin(), y2.end());
}

double Window::map(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Window::get_graph_boundaries() {
    Y_MIN = std::min(y_min_1, y_min_2);
    if (Y_MIN < THRESHOLD_MIN)
        Y_MIN = 0;
    Y_MAX = std::max(y_max_1, y_max_2);
}

void Window::plot_y1() {
    std::cout << "Affichage de la courbe 1" << std::endl;
    if (x.size() - 2 != y1.size()) {
        std::cout << "Tailles différentes de vecteur: x=" << x.size()
                  << ", y1=" << y1.size() << std::endl;
        throw "mauvaise taille";
    }
    int n_points = y1.size();
    for (int i = 0; i < n_points; i++) {
        int p_x = map(x[i], 0.0, L, alpha * window_width, (1 - alpha) * window_width);
        int p_y = map(y1[i], Y_MIN, Y_MAX, (1 - alpha) * window_height, alpha * window_height);
        draw_circle(p_x, p_y, 2);  // Larger points
    }
}

void Window::plot_y2() {
    std::cout << "Affichage de la courbe 2" << std::endl;
    if (x.size() - 2 != y2.size()) {
        std::cout << "Tailles différentes de vecteur: x=" << x.size()
                  << ", y2=" << y2.size() << std::endl;
        throw "mauvaise taille";
    }
    int n_points = y2.size();
    for (int i = 0; i < n_points; i++) {
        int p_x = map(x[i], 0.0, L, alpha * window_width, (1 - alpha) * window_width);
        int p_y = map(y2[i], Y_MIN, Y_MAX, (1 - alpha) * window_height, alpha * window_height);
        draw_circle(p_x, p_y, 2);  // Larger points
    }
}

void Window::plot_frame() {
    const int n_points = 5;
    SDL_Point points[n_points] = {
        {static_cast<int>(alpha * window_width), static_cast<int>(alpha * window_height)},
        {static_cast<int>(alpha * window_width), static_cast<int>((1 - alpha) * window_height)},
        {static_cast<int>((1 - alpha) * window_width), static_cast<int>((1 - alpha) * window_height)},
        {static_cast<int>((1 - alpha) * window_width), static_cast<int>(alpha * window_height)},
        {static_cast<int>(alpha * window_width), static_cast<int>(alpha * window_height)},
    };
    // Brighter white frame
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLines(renderer, points, n_points);
}

void Window::draw_circle(int n_cx, int n_cy, int radius) {
    double error = (double)-radius;
    double x = (double)radius - 0.5;
    double y = (double)0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;
    while (x >= y) {
        SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy + y));
        SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy + x));
        if (x != 0) {
            SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy + y));
            SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy - x));
        }
        if (y != 0) {
            SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy - y));
            SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy + x));
        }
        if (x != 0 && y != 0) {
            SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy - y));
            SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy - x));
        }
        error += y;
        ++y;
        error += y;
        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
}

bool Window::has_y1() {
    return !y1.empty();
}

bool Window::has_y2() {
    return !y2.empty();
}

void Window::plot() {
    clear();
    plot_frame();
    get_graph_boundaries();

    if (has_y1() && has_y2()) {
        // Cyan for curve 1 (better visibility on dark background)
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
        plot_y1();
        // Gold/Yellow for curve 2
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xD7, 0x00, 0xFF);
        plot_y2();
    }
    else if (has_y1() && !has_y2()) {
        // Bright green
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x88, 0xFF);
        plot_y1();
    }
    else {
        // Orange
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xA5, 0x00, 0xFF);
        plot_y2();
    }
}