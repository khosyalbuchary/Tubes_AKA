#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <string>
#include <chrono>  
#include <cstdlib> 
#include <ctime>   
#include "HeroAoE.h" 

using namespace std;

void generateGraph(int n, string currentHeroName, map<int, Unit>& units, map<int, vector<int>>& adj) {
    units.clear();
    adj.clear();
    
    units[1] = {1, currentHeroName, {0.0, 0.0}, true};
    
    for(int i = 2; i <= n; i++) {
        double rX = (rand() % 100) / 10.0; 
        double rY = (rand() % 100) / 10.0;
        units[i] = {i, "Enemy", {rX, rY}, false};
        
        adj[i].push_back(i-1);
        adj[i-1].push_back(i);
        
        if (i > 2) {
            int randomNode = (rand() % (i-1)) + 1;
            adj[i].push_back(randomNode);
            adj[randomNode].push_back(i);
        }
    }
}

vector<Hero> initializeHeroes() {
    vector<Hero> list;

    Hero h1; h1.heroName = "Belerick";
    h1.skills.push_back({"Ancient Seed (S1)", 2.5}); 
    h1.skills.push_back({"Nature's Wrath (Ult)", 4.0});
    list.push_back(h1);

    Hero h2; h2.heroName = "Alice";
    h2.skills.push_back({"Flowing Blood (S1)", 2.0});
    h2.skills.push_back({"Blood Ode (Ult)", 3.5});
    list.push_back(h2);

    Hero h3; h3.heroName = "Tigreal";
    h3.skills.push_back({"Attack Wave (S1)", 2.8});
    h3.skills.push_back({"Implosion (Ult)", 3.2});
    list.push_back(h3);
  
    Hero h4; h4.heroName = "Odette";
    h4.skills.push_back({"Avian Authority (S1)", 2.2});
    h4.skills.push_back({"Swan Song (Ult)", 5.0});
    list.push_back(h4);

    Hero h5; h5.heroName = "Yve";
    h5.skills.push_back({"Void Blast (S1)", 2.0});
    h5.skills.push_back({"Real World Manipulation (Ult)", 5.5});
    list.push_back(h5);

    return list;
}

int main() {
    srand(time(0)); 
    vector<Hero> heroes = initializeHeroes();
    int n_values[] = {10, 100, 1000, 10000}; 
    char ulang;

    do {
        cout << "\n==========================================" << endl;
        cout << "   MOBA AoE ALGORITHM SIMULATOR (BFS vs DFS)" << endl;
        cout << "==========================================" << endl;
        cout << "Pilih Hero:" << endl;
        
        for (size_t i = 0; i < heroes.size(); ++i) {
            cout << i + 1 << ". " << heroes[i].heroName << endl;
        }
        
        int heroChoice;
        cout << "\n>> Masukan Nomor Hero (1-" << heroes.size() << "): ";
        cin >> heroChoice;

        if (heroChoice < 1 || heroChoice > heroes.size()) {
            cout << "Pilihan tidak valid!" << endl;
            continue;
        }

        Hero selectedHero = heroes[heroChoice - 1];
        
        cout << "\n>>> Hero: " << selectedHero.heroName << " <<<" << endl;
        cout << "Pilih Skill:" << endl;
        for (size_t i = 0; i < selectedHero.skills.size(); ++i) {
            cout << i + 1 << ". " << selectedHero.skills[i].skillName 
                 << " (Radius: " << selectedHero.skills[i].radius << ")" << endl;
        }

        int skillChoice;
        cout << ">> Masukan Nomor Skill: ";
        cin >> skillChoice;

        if (skillChoice < 1 || skillChoice > selectedHero.skills.size()) {
            cout << "Skill tidak valid!" << endl;
            continue;
        }

        Skill activeSkill = selectedHero.skills[skillChoice - 1];
        cout << "\n==========================================================" << endl;
        cout << "[ START BENCHMARK ]" << endl;
        cout << "Hero   : " << selectedHero.heroName << endl;
        cout << "Skill  : " << activeSkill.skillName << endl;
        cout << "Radius : " << activeSkill.radius << endl;
        cout << "==========================================================" << endl;
        
        cout << "+----------+--------------------------+--------------------------+" << endl;
        cout << "| Unit (n) |  Metode DFS(Rekursif)(ms)|  Metode BFS(Iteratif)(ms)|" << endl;
        cout << "+==========+==========================+==========================+" << endl;

        map<int, Unit> units;
        map<int, vector<int>> adj;

        for (int n : n_values) {
            generateGraph(n, selectedHero.heroName, units, adj);
            double r = activeSkill.radius;

            auto startDFS = chrono::high_resolution_clock::now();
            for(int k=0; k<50; k++) { 
                set<int> visited;
                analyzeDFSRecursive(1, 1, r, units, adj, visited);
            }
            auto endDFS = chrono::high_resolution_clock::now();
            double timeDFS = chrono::duration<double, milli>(endDFS - startDFS).count();

           
            auto startBFS = chrono::high_resolution_clock::now();
            for(int k=0; k<50; k++) { 
                analyzeBFS(1, r, units, adj);
            }
            auto endBFS = chrono::high_resolution_clock::now();
            double timeBFS = chrono::duration<double, milli>(endBFS - startBFS).count();

            cout << "| " << setw(8) << left << n 
                 << " | " << setw(24) << fixed << setprecision(6) << timeDFS 
                 << " | " << setw(24) << timeBFS << " |" << endl;
        }
        cout << "+----------+--------------------------+--------------------------+" << endl;

        cout << "\nUlangi? (y/n): ";
        cin >> ulang;

    } while (ulang == 'y' || ulang == 'Y');

    return 0;
}