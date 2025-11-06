#include "Personnage.h"
#include <iostream>
using namespace std;

Personnage::Personnage()
    : nom("Inconnu"), mouvement(0), attaque(0), defense(0), degats(0), pointsDeVie(0) {}

Personnage::~Personnage() {}

string Personnage::getNom() const { return nom; }
int Personnage::getMouvement() const { return mouvement; }
int Personnage::getAttaque() const { return attaque; }
int Personnage::getDefense() const { return defense; }
int Personnage::getDegats() const { return degats; }
int Personnage::getPointsDeVie() const { return pointsDeVie; }

void Personnage::setAttaque(int a) { attaque = a; }
void Personnage::setDegats(int d) { degats = d; }

bool Personnage::estVivant() const { return pointsDeVie > 0; }

void Personnage::attaquer(Personnage& cible) {
    cout << nom << " attaque " << cible.nom << " !" << endl;

    vector<int> resAtt = de.lancerPlusieurs(attaque);
    cout << nom << " lance " << attaque << " des : ";
    for (int v : resAtt) cout << v << " ";
    cout << endl;
    int succAtt = de.compterSucces(resAtt, attaque);
    cout << "Des >= " << attaque << " : " << succAtt << " succes." << endl;
    int puissanceAtt = succAtt * degats;

    vector<int> resDef = de.lancerPlusieurs(cible.defense);
    cout << cible.nom << " lance " << cible.defense << " des de defense : ";
    for (int v : resDef) cout << v << " ";
    cout << endl;
    int succDef = de.compterSucces(resDef, cible.defense);
    cout << "Des >= " << cible.defense << " : " << succDef << " succes." << endl;
    int puissanceDef = succDef * cible.defense;

    int degatsInfliges = 0;
    if (puissanceAtt > puissanceDef) degatsInfliges = puissanceAtt - puissanceDef;
    cible.pointsDeVie -= degatsInfliges;
    if (cible.pointsDeVie < 0) cible.pointsDeVie = 0;

    cout << "Resultat : " << nom << " inflige " << degatsInfliges << " points de degats a " << cible.nom << "." << endl;
    cout << cible.nom << " a maintenant " << cible.pointsDeVie << " PV restants." << endl;
}
