#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Astartes.h"
#include "Demon.h"
#include "Plateau.h"
#include <queue>

using namespace std;

static void activerSeqANSIWindows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

static void pauseMillis(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

int manhattan(int x1,int y1,int x2,int y2){ return abs(x1-x2)+abs(y1-y2); }

bool trouverChemin(const Plateau& plateau, int sx, int sy, int tx, int ty, vector<Point>& path) {
    path.clear();
    if (sx==tx && sy==ty) { path.push_back({sx,sy}); return true; }
    const int n = TAILLE;
    vector<vector<bool>> vis(n, vector<bool>(n,false));
    vector<vector<Point>> parent(n, vector<Point>(n, {-1,-1}));
    queue<Point> q;
    q.push({sx,sy}); vis[sx][sy]=true;
    const int dx[4]={1,-1,0,0}, dy[4]={0,0,1,-1};
    bool trouve=false;
    while(!q.empty()) {
        Point p=q.front(); q.pop();
        if (p.x==tx && p.y==ty) { trouve=true; break;}
        for(int k=0;k<4;++k){
            int nx=p.x+dx[k], ny=p.y+dy[k];
            if(nx<0||ny<0||nx>=n||ny>=n) continue;
            if(vis[nx][ny]) continue;
            char c = plateau.getCase(nx,ny);
            if (c == '/') continue;
            if ((c == 'A' || c == 'D') && !(nx==tx && ny==ty)) continue;
            vis[nx][ny]=true;
            parent[nx][ny]=p;
            q.push({nx,ny});
        }
    }
    if (!trouve) return false;
    Point cur={tx,ty};
    while(!(cur.x== -1 && cur.y== -1)){
        path.push_back(cur);
        Point par = parent[cur.x][cur.y];
        if (par.x== -1 && par.y== -1) break;
        cur = par;
    }
    reverse(path.begin(), path.end());
    return true;
}

int main() {
    srand((unsigned)time(nullptr));
    activerSeqANSIWindows();

    cout << "=== CHOIX DE VOTRE ASTARTES ===\n";
    cout << "1. Assault Intercessor Sergeant (Mvt6 Att4 Def3 Dgt5 PV15)\n";
    cout << "2. Intercessor Sergeant (Mvt6 Att4 Def4 Dgt4 PV15)\n";
    cout << "3. Space Marine Captain (Mvt6 Att5 Def4 Dgt6 PV15)\n";
    cout << "4. Assault Intercessor Grenadier (Mvt6 Att4 Def3 Dgt4 PV14)\n";
    cout << "5. Assault Intercessor Warrior (Mvt6 Att4 Def3 Dgt5 PV14)\n";
    cout << "6. Intercessor Warrior (Mvt6 Att3 Def3 Dgt4 PV14)\n";
    cout << "7. Intercessor Gunner (Mvt6 Att3 Def3 Dgt4 PV14)\n";
    cout << "8. Eliminator Sniper (Mvt7 Att5 Def2 Dgt6 PV12)\n";
    cout << "9. Heavy Intercessor Gunner (Mvt5 Att4 Def4 Dgt5 PV18)\n";
    cout << "Votre choix : ";
    int choix=1;
    if (!(cin>>choix) || choix<1 || choix>9) { choix=1; cin.clear(); string tmp; getline(cin,tmp); }

    Astartes a(choix);
    Demon d;
    Plateau plateau;

    int ax=0, ay=0, dx_pos=TAILLE-1, dy_pos=TAILLE-1;
    plateau.deplacerPersonnage(ax,ay,ax,ay,'A');
    plateau.deplacerPersonnage(dx_pos,dy_pos,dx_pos,dy_pos,'D');

    const int animDelay = 300;

    bool quitter=false;
    while(!quitter) {
        plateau.afficher();
        cout << "\nPV A: " << a.getPointsDeVie() << "   PV D: " << d.getPointsDeVie() << "\n";
        cout << "\n1. Se deplacer\n2. Attaquer\n3. Abandonner\nVotre choix : ";
        int action=0; if (!(cin>>action)) { cin.clear(); string tmp; getline(cin,tmp); continue; }

        if (action==3) { cout << "Abandon...\n"; break; }

        if (action==1) {
            plateau.afficherZoneMouvement(ax,ay,a.getMouvement());
            cout << "Entrez X : "; int nx; if(!(cin>>nx)){ cin.clear(); string tmp; getline(cin,tmp); continue;}
            cout << "Entrez Y : "; int ny; if(!(cin>>ny)){ cin.clear(); string tmp; getline(cin,tmp); continue;}
            if (!plateau.deplacementValide(ax,ay,nx,ny,a.getMouvement())) { cout << "Deplacement invalide.\n"; pauseMillis(800); continue; }
            vector<Point> path;
            if (!trouverChemin(plateau,ax,ay,nx,ny,path)) { cout << "Chemin introuvable.\n"; pauseMillis(800); continue; }
            for(size_t i=1;i<path.size();++i){
                int fromx=ax, fromy=ay;
                int tox=path[i].x, toy=path[i].y;
                plateau.deplacerPersonnage(fromx,fromy,tox,toy,'A');
                ax=tox; ay=toy;
                plateau.afficher();
                pauseMillis(animDelay);
            }
        }
        else if (action==2) {
            a.afficherArsenal();
            cout << "Choisissez arme (numero) ou 0 pour annuler : ";
            int idx; if(!(cin>>idx)){ cin.clear(); string tmp; getline(cin,tmp); continue; }
            if (idx==0) continue;
            if (idx<1 || idx> (int)a.arsenal.size()) { cout << "Choix invalide.\n"; pauseMillis(600); continue; }
            Arme &arme = a.arsenal[idx-1];
            if (!arme.estDisponible()) { cout << "Arme en rechargement (" << arme.cooldownActuel << " tours" << "\n";
            pauseMillis(800);
            continue; }
            plateau.afficherPorteeArme(ax,ay,arme.portee);
            cout << "1. Tirer\n2. Revenir\nVotre choix : ";
            int choixAction; if(!(cin>>choixAction)){ cin.clear(); string tmp; getline(cin,tmp); continue; }
            if (choixAction==2) continue;
            cout << "Entrez X cible : "; int tx; if(!(cin>>tx)){ cin.clear(); string tmp; getline(cin,tmp); continue; }
            cout << "Entrez Y cible : "; int ty; if(!(cin>>ty)){ cin.clear(); string tmp; getline(cin,tmp); continue; }
            if (manhattan(ax,ay,tx,ty) > arme.portee) { cout << "Hors portee.\n"; pauseMillis(800); continue; }
            if (plateau.ligneDeVueCoupee(ax,ay,tx,ty)) {
                if (plateau.getCase(tx,ty) == '/' && arme.detruitObstacle) {
                    cout << "Obstacle detruit par " << arme.nom << "!\\n";
                    plateau.supprimerObstacle(tx,ty);
                    arme.declencherCooldown();
                } else {
                    cout << "Tir bloque par obstacle.\n"; pauseMillis(800); continue;
                }
            } else {
                if (tx==dx_pos && ty==dy_pos) {
                    int oldAtt = a.getAttaque();
                    int oldDeg = a.getDegats();
                    a.setAttaque(oldAtt + arme.modAttaque);
                    a.setDegats(oldDeg + arme.modDegats);
                    a.attaquer(d);
                    a.setAttaque(oldAtt);
                    a.setDegats(oldDeg);
                    arme.declencherCooldown();
                } else if (plateau.getCase(tx,ty) == '/') {
                    if (arme.detruitObstacle) {
                        plateau.supprimerObstacle(tx,ty);
                        cout << "Obstacle detruit !\\n";
                        arme.declencherCooldown();
                    } else {
                        cout << "Tir sur obstacle sans effet.\n";
                    }
                } else {
                    cout << "Tir sur case vide.\n";
                }
            }
        }

        for (auto &ar : a.arsenal) ar.reduireCooldown();

        if (!d.estVivant()) { cout << "Demon mort. Vous gagnez!\\n"; break; }

        cout << "\n-- Tour Demon --\n"; pauseMillis(600);
        int dist = manhattan(ax,ay,dx_pos,dy_pos);
        if (dist > 1) {
            vector<Point> path;
            if (trouverChemin(plateau, dx_pos, dy_pos, ax, ay, path) && path.size()>1) {
                int stepsToA = (int)path.size()-1;
                int move = min(d.getMouvement(), stepsToA-1);
                if (move > 0) {
                    for (int i=1;i<=move;++i) {
                        int fromx=dx_pos, fromy=dy_pos;
                        int tox=path[i].x, toy=path[i].y;
                        plateau.deplacerPersonnage(fromx,fromy,tox,toy,'D');
                        dx_pos=tox; dy_pos=toy;
                        plateau.afficher();
                        pauseMillis(animDelay);
                    }
                }
            } else {
                cout << "Demon ne peut pas atteindre Astartes.\n";
            }
        } else {
            int chance = rand()%100;
            if (chance < 80) {
                cout << "Demon attaque!\n"; d.attaquer(a); pauseMillis(600);
            } else {
                cout << "Demon se repositionne.\n";
                bool moved=false;
                int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
                for (int k=0;k<4 && !moved;k++){
                    int nx=dx_pos+dirs[k][0], ny=dy_pos+dirs[k][1];
                    if (nx<0||ny<0||nx>=TAILLE||ny>=TAILLE) continue;
                    if (!plateau.estLibre(nx,ny)) continue;
                    plateau.deplacerPersonnage(dx_pos,dy_pos,nx,ny,'D');
                    dx_pos=nx; dy_pos=ny; moved=true;
                }
                if (!moved) cout << "Demon ne peut pas se repositionner.\n";
            }
        }

        if (!a.estVivant()) { cout << "Astartes mort. Defaite.\n"; break; }

        pauseMillis(600);
    }

    cout << "Partie terminee." << endl;
    return 0;
}
