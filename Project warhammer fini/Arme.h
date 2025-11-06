#pragma once
#include <string>
using namespace std;

class Arme {
public:
    string nom;
    int portee;
    int modAttaque;
    int modDegats;
    int cooldownMax;
    int cooldownActuel;
    bool detruitObstacle;

    Arme(string n = "Aucune", int p = 1, int att = 0, int deg = 0, int cd = 0, bool det = false)
        : nom(n), portee(p), modAttaque(att), modDegats(deg),
          cooldownMax(cd), cooldownActuel(0), detruitObstacle(det) {}

    bool estDisponible() const { return cooldownActuel == 0; }
    void declencherCooldown() { cooldownActuel = cooldownMax; }
    void reduireCooldown() { if (cooldownActuel > 0) --cooldownActuel; }
};
