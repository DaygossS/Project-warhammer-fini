#pragma once
#include "Personnage.h"
#include "Arme.h"
#include <vector>
using namespace std;

class Astartes : public Personnage {
public:
    vector<Arme> arsenal;
    Astartes(int choix = 1);
    void afficherArsenal() const;
};
