#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class De {
public:
    De();
    int lancer();
    vector<int> lancerPlusieurs(int nbDes);
    int compterSucces(const vector<int>& resultats, int seuil);
};
