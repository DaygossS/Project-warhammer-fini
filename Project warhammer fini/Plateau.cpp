#include "Plateau.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <cmath>
#include <queue>

using namespace std;

static void pauseMillis(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

static void nettoyerConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Plateau::Plateau() {
    grille.resize(TAILLE, vector<char>(TAILLE, '.'));
    genererObstacles();
}

void Plateau::genererObstacles() {
    srand((unsigned int)time(nullptr));
    int nbCases = TAILLE * TAILLE;
    int maxObstacles = static_cast<int>(nbCases * 0.2);
    int nbObstacles = rand() % (maxObstacles + 1);

    for (int i = 0; i < nbObstacles; ++i) {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if ((x == 0 && y == 0) || (x == TAILLE - 1 && y == TAILLE - 1)) {
            --i;
            continue;
        }
        if (grille[x][y] == '.') grille[x][y] = '/';
        else --i;
    }
}

void Plateau::afficher() const {
    cout << "   ";
    for (int c = 0; c < TAILLE; ++c)
        cout << c << " ";
    cout << endl;

    for (int i = 0; i < TAILLE; ++i) {
        cout << i << "  ";
        for (int j = 0; j < TAILLE; ++j) {
            char s = grille[i][j];
            if (s == 'A') cout << "\033[32mA\033[0m ";
            else if (s == 'D') cout << "\033[31mD\033[0m ";
            else if (s == '/') cout << "\033[90m/\033[0m ";
            else cout << s << " ";
        }
        cout << endl;
    }
}

bool Plateau::estLibre(int x, int y) const {
    return (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE && grille[x][y] == '.');
}

bool Plateau::estObstacle(int x, int y) const {
    return (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE && grille[x][y] == '/');
}

char Plateau::getCase(int x, int y) const {
    if (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE)
        return grille[x][y];
    return ' ';
}

void Plateau::supprimerObstacle(int x, int y) {
    if (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE && grille[x][y] == '/')
        grille[x][y] = '.';
}

void Plateau::deplacerPersonnage(int x1, int y1, int x2, int y2, char symbole) {
    if (x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0 &&
        x1 < TAILLE && y1 < TAILLE && x2 < TAILLE && y2 < TAILLE) {
        grille[x1][y1] = '.';
        grille[x2][y2] = symbole;
    }
}

void Plateau::afficherZoneMouvement(int x, int y, int portee) {
    nettoyerConsole();
    cout << "Zones accessibles (etoiles jaunes = portee de mouvement)\n" << endl;

    cout << "   ";
    for (int c = 0; c < TAILLE; ++c)
        cout << c << " ";
    cout << endl;

    for (int i = 0; i < TAILLE; ++i) {
        cout << i << "  ";
        for (int j = 0; j < TAILLE; ++j) {
            if (grille[i][j] == 'A' || grille[i][j] == 'D' || grille[i][j] == '/')
                cout << grille[i][j] << " ";
            else {
                int dist = abs(x - i) + abs(y - j);
                if (dist <= portee)
                    cout << "\033[33m*\033[0m ";
                else
                    cout << ". ";
            }
        }
        cout << endl;
    }
}

bool Plateau::deplacementValide(int x1, int y1, int x2, int y2, int mouv) {
    if (x2 < 0 || y2 < 0 || x2 >= TAILLE || y2 >= TAILLE) return false;
    if (grille[x2][y2] != '.') return false;
    int dist = abs(x2 - x1) + abs(y2 - y1);
    return (dist <= mouv);
}

void Plateau::deplacementAnimeAvecPath(int& x, int& y, const vector<Point>& chemin, char symbole, int delai) {
    for (size_t i = 1; i < chemin.size(); ++i) {
        int nx = chemin[i].x;
        int ny = chemin[i].y;
        if (!this->estLibre(nx, ny)) break;
        this->grille[x][y] = '.';
        this->grille[nx][ny] = symbole;
        x = nx; y = ny;
        nettoyerConsole();
        this->afficher();
        pauseMillis(delai * 2);
    }
}

bool Plateau::ligneDeVueCoupee(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int cx = x1, cy = y1;

    while (!(cx == x2 && cy == y2)) {
        if (grille[cx][cy] == '/') return true;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; cx += sx; }
        if (e2 < dx) { err += dx; cy += sy; }
    }
    return false;
}

void Plateau::afficherPorteeArme(int x, int y, int portee) {
    nettoyerConsole();
    cout << "Visualisation de la portee de l’arme (rouge = accessible, gris = bloque)\n" << endl;

    cout << "   ";
    for (int c = 0; c < TAILLE; ++c)
        cout << c << " ";
    cout << endl;

    for (int i = 0; i < TAILLE; ++i) {
        cout << i << "  ";
        for (int j = 0; j < TAILLE; ++j) {
            char s = grille[i][j];
            if (s == 'A' || s == 'D' || s == '/') {
                cout << s << " ";
            }
            else {
                int dist = abs(x - i) + abs(y - j);
                if (dist <= portee) {
                    if (ligneDeVueCoupee(x, y, i, j))
                        cout << "\033[90m*\033[0m ";
                    else
                        cout << "\033[31m*\033[0m ";
                }
                else {
                    cout << ". ";
