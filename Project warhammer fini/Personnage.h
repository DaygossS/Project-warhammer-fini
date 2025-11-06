#pragma once
#include <string>
#include "De.h"
using namespace std;

class Personnage {
protected:
    string nom;
    int mouvement;
    int attaque;
    int defense;
    int degats;
    int pointsDeVie;
    De de;
public:
    Personnage();
    virtual ~Personnage();

    string getNom() const;
    int getMouvement() const;
    int getAttaque() const;
    int getDefense() const;
    int getDegats() const;
    int getPointsDeVie() const;

    void setAttaque(int a);
    void setDegats(int d);

    bool estVivant() const;
    virtual void attaquer(Personnage& cible);
};
