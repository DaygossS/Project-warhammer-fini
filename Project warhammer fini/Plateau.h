#pragma once
#include <iostream>
#include <vector>
using namespace std;

const int TAILLE = 8;
struct Point { int x, y; };

class Plateau {
private:
    vector<vector<char>> grille;
public:
    Plateau();
    void genererObstacles();
    void afficher() const;
    bool estLibre(int x, int y) const;
    bool estObstacle(int x, int y) const;
    char getCase(int x, int y) const;
    void supprimerObstacle(int x, int y);
    void afficherZoneMouvement(int x, int y, int portee);
    bool deplacementValide(int x1, int y1, int x2, int y2, int mouv);
    void deplacerPersonnage(int x1, int y1, int x2, int y2, char symbole);
    void deplacementAnimeAvecPath(int& x, int& y, const vector<Point>& chemin, char symbole, int delai);
    void afficherPorteeArme(int x, int y, int portee);
    bool ligneDeVueCoupee(int x1, int y1, int x2, int y2);
};
