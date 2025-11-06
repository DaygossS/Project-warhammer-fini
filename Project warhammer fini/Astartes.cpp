#include "Astartes.h"
#include <iostream>
using namespace std;

Astartes::Astartes(int choix) {
    switch (choix) {
    case 1:
        nom = "Assault Intercessor Sergeant";
        mouvement = 6; attaque = 4; defense = 3; degats = 5; pointsDeVie = 15;
        arsenal = {
            Arme("Fusil a pompe", 1, -1, +2, 1, false),
            Arme("Epee tronconneuse", 1, +1, +1, 0, false),
            Arme("Pistolet plasma", 3, 0, +2, 1, false)
        };
        break;
    case 2:
        nom = "Intercessor Sergeant";
        mouvement = 6; attaque = 4; defense = 4; degats = 4; pointsDeVie = 15;
        arsenal = {
            Arme("Bolter", 4, 0, 0, 0, false),
            Arme("Lance-flammes", 2, +1, +2, 1, false),
            Arme("Lance-grenades", 5, -1, +2, 2, true)
        };
        break;
    case 3:
        nom = "Space Marine Captain";
        mouvement = 6; attaque = 5; defense = 4; degats = 6; pointsDeVie = 15;
        arsenal = {
            Arme("Bolter", 4, 0, 0, 0, false),
            Arme("Canon laser", 6, -2, +3, 2, true),
            Arme("Sniper", 7, -2, +1, 2, true)
        };
        break;
    case 4:
        nom = "Assault Intercessor Grenadier";
        mouvement = 6; attaque = 4; defense = 3; degats = 4; pointsDeVie = 14;
        arsenal = {
            Arme("Fusil a pompe", 1, -1, +2, 1, false),
            Arme("Lance-grenades", 5, -1, +2, 2, true),
            Arme("Bolter", 4, 0, 0, 0, false)
        };
        break;
    case 5:
        nom = "Assault Intercessor Warrior";
        mouvement = 6; attaque = 4; defense = 3; degats = 5; pointsDeVie = 14;
        arsenal = {
            Arme("Epee tronconneuse", 1, +1, +1, 0, false),
            Arme("Mitraillette lourde", 5, +2, -1, 0, false),
            Arme("Bolter", 4, 0, 0, 0, false)
        };
        break;
    case 6:
        nom = "Intercessor Warrior";
        mouvement = 6; attaque = 3; defense = 3; degats = 4; pointsDeVie = 14;
        arsenal = {
            Arme("Bolter", 4, 0, 0, 0, false),
            Arme("Lance-flammes", 2, +1, +2, 1, false),
            Arme("Sniper", 7, -2, +1, 2, true)
        };
        break;
    case 7:
        nom = "Intercessor Gunner";
        mouvement = 6; attaque = 3; defense = 3; degats = 4; pointsDeVie = 14;
        arsenal = {
            Arme("Mitraillette lourde", 5, +2, -1, 0, false),
            Arme("Lance-grenades", 5, -1, +2, 2, true),
            Arme("Fusil a pompe", 1, -1, +2, 1, false)
        };
        break;
    case 8:
        nom = "Eliminator Sniper";
        mouvement = 7; attaque = 5; defense = 2; degats = 6; pointsDeVie = 12;
        arsenal = {
            Arme("Sniper", 7, -2, +1, 2, true),
            Arme("Pistolet plasma", 3, 0, +2, 1, false),
            Arme("Bolter", 4, 0, 0, 0, false)
        };
        break;
    case 9:
        nom = "Heavy Intercessor Gunner";
        mouvement = 5; attaque = 4; defense = 4; degats = 5; pointsDeVie = 18;
        arsenal = {
            Arme("Canon laser", 6, -2, +3, 2, true),
            Arme("Mitraillette lourde", 5, +2, -1, 0, false),
            Arme("Marteau energetique", 1, -1, +3, 2, true)
        };
        break;
    default:
        nom = "Assault Intercessor Sergeant";
        mouvement = 6; attaque = 4; defense = 3; degats = 5; pointsDeVie = 15;
        arsenal = {
            Arme("Bolter", 4, 0, 0, 0, false),
            Arme("Epee tronconneuse", 1, +1, +1, 0, false),
            Arme("Pistolet plasma", 3, 0, +2, 1, false)
        };
        break;
    }
}

void Astartes::afficherArsenal() const {
    cout << endl << "Arsenal de " << nom << " :" << endl;
    for (size_t i = 0; i < arsenal.size(); ++i) {
        const Arme& arme = arsenal[i];
        if (arme.estDisponible()) cout << "\033[32m"; else cout << "\033[31m";
        cout << (i+1) << ". " << arme.nom
             << " (Portee " << arme.portee
             << ", Att " << (arme.modAttaque >= 0 ? "+" : "") << arme.modAttaque
             << ", Deg " << (arme.modDegats >= 0 ? "+" : "") << arme.modDegats
             << ", CD " << arme.cooldownMax << ")";
        if (!arme.estDisponible()) cout << " [rechargement: " << arme.cooldownActuel << " tours]";
        else cout << " [disponible]";
        cout << "\033[0m" << endl;
    }
    cout << endl;
}
