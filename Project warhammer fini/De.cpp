#include "De.h"

De::De() {
    static bool dejaInit = false;
    if (!dejaInit) {
        srand(static_cast<unsigned int>(time(nullptr)));
        dejaInit = true;
    }
}

int De::lancer() {
    return rand() % 6 + 1;
}

vector<int> De::lancerPlusieurs(int nbDes) {
    vector<int> res;
    for (int i = 0; i < nbDes; ++i) res.push_back(lancer());
    return res;
}

int De::compterSucces(const vector<int>& resultats, int seuil) {
    int c = 0;
    for (int v : resultats) if (v >= seuil) ++c;
    return c;
}
