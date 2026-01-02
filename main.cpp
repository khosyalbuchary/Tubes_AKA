#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue> 
#include <iomanip>
#include <string>
#include <chrono> 
#include <cstdlib> 
#include <ctime>   
#include "HeroAoE.h" 

using namespace std;

// --- FUNGSI GENERATE GRAPH ---
void generateGraph(int n, string currentHeroName, map<int, Unit>& units, map<int, vector<int>>& adj) {
    units.clear(); adj.clear();
    units[1] = {1, currentHeroName + " (YOU)", {0.0, 0.0}, true};
    vector<string> mobTypes = {"Minion Melee", "Minion Range", "Siege Minion", "Lithowanderer", "Layla (Bot)", "Zilong (Bot)"};
    
    for(int i = 2; i <= n; i++) {
        double rX = ((rand() % 100) / 10.0) - 5.0; 
        double rY = ((rand() % 100) / 10.0) - 5.0;
        units[i] = {i, mobTypes[rand() % mobTypes.size()], {rX, rY}, false};
        
        adj[i].push_back(i-1); adj[i-1].push_back(i);
        if (i > 2) { int r = (rand() % (i-1)) + 1; adj[i].push_back(r); adj[r].push_back(i); }
    }
}

// --- HEADER TABEL ---
void printTableHeader() {
    cout << "+-------+----------+--------------+--------------+--------------+--------------+" << endl;
    cout << "| Unit  | Jml Kena | DFS Rekursif | DFS Iteratif | BFS Rekursif | BFS Iteratif |" << endl;
    cout << "|  (N)  |  (Unit)  |     (ms)     |     (ms)     |     (ms)     |     (ms)     |" << endl;
    cout << "+=======+==========+==============+==============+==============+==============+" << endl;
}

// --- ISI TABEL ---
void printTableRow(int n, int hitCount, double tDFS_Rec, double tDFS_Iter, double tBFS_Rec, double tBFS_Iter) {
    cout << "| " << setw(5) << left << n 
         << " | " << setw(8) << hitCount 
         << " | " << setw(12) << fixed << setprecision(4) << tDFS_Rec 
         << " | " << setw(12) << tDFS_Iter 
         << " | " << setw(12) << tBFS_Rec 
         << " | " << setw(12) << tBFS_Iter << " |" << endl;
}

// --- BENCHMARK ENGINE (Jalankan 4 Algo) ---
void runBenchmarkRow(int n, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    int loops = 10; 

    // Hitung unit kena (Pakai salah satu algo)
    vector<int> hits = analyzeBFSIterative(1, radius, units, adj);
    int hitCount = hits.size();

    // 1. DFS Rekursif
    auto t1 = chrono::high_resolution_clock::now();
    for(int k=0; k<loops; k++) { 
        set<int> v; vector<int> d; analyzeDFSRecursive(1, 1, radius, units, adj, v, d); 
    }
    double t1_val = chrono::duration<double, milli>(chrono::high_resolution_clock::now()-t1).count() / loops;

    // 2. DFS Iteratif
    auto t2 = chrono::high_resolution_clock::now();
    for(int k=0; k<loops; k++) { analyzeDFSIterative(1, radius, units, adj); }
    double t2_val = chrono::duration<double, milli>(chrono::high_resolution_clock::now()-t2).count() / loops;

    // 3. BFS Rekursif
    auto t3 = chrono::high_resolution_clock::now();
    for(int k=0; k<loops; k++) { 
        set<int> v; vector<int> d; queue<int> q; q.push(1); v.insert(1);
        analyzeBFSRecursive(q, 1, radius, units, adj, v, d); 
    }
    double t3_val = chrono::duration<double, milli>(chrono::high_resolution_clock::now()-t3).count() / loops;

    // 4. BFS Iteratif
    auto t4 = chrono::high_resolution_clock::now();
    for(int k=0; k<loops; k++) { analyzeBFSIterative(1, radius, units, adj); }
    double t4_val = chrono::duration<double, milli>(chrono::high_resolution_clock::now()-t4).count() / loops;

    printTableRow(n, hitCount, t1_val, t2_val, t3_val, t4_val);
}

vector<Hero> initializeHeroes() {
    vector<Hero> list;
    list.push_back({"Belerick", {{"Ancient Seed", 2.5}, {"Nature's Wrath", 4.0}}});
    list.push_back({"Alice", {{"Flowing Blood", 2.0}, {"Blood Ode", 3.5}}});
    list.push_back({"Odette", {{"Avian Authority", 2.2}, {"Swan Song", 5.0}}});
    list.push_back({"Yve", {{"Void Blast", 2.0}, {"Real World Manip.", 6.0}}});
    return list;
}

int main() {
    srand(time(0)); 
    vector<Hero> heroes = initializeHeroes();
    int standard_n[] = {10, 100, 1000, 10000}; 
    char ulang;

    do {
        cout << "\n==========================================================" << endl;
        cout << " [ ANALISIS AOE MOBILE LEGENDS ]" << endl;
        
        cout << "Pilih Hero:" << endl;
        for (size_t i = 0; i < heroes.size(); ++i) cout << i + 1 << ". " << heroes[i].heroName << endl;
        int heroChoice; cout << ">> Input: "; cin >> heroChoice;
        if (heroChoice < 1 || heroChoice > heroes.size()) continue;
        
        Hero selectedHero = heroes[heroChoice - 1];
        cout << "\nSkill " << selectedHero.heroName << ":" << endl;
        for (size_t i = 0; i < selectedHero.skills.size(); ++i) 
            cout << i + 1 << ". " << selectedHero.skills[i].skillName << " (R: " << selectedHero.skills[i].radius << ")" << endl;
        int skillChoice; cout << ">> Input: "; cin >> skillChoice;
        Skill activeSkill = selectedHero.skills[skillChoice - 1];

        cout << "\n[ PILIH SKENARIO PENGUJIAN ]" << endl;
        cout << "1. Default Benchmark (10, 100, 1000, 10000)" << endl;
        cout << "2. Manual Input (Input 4 Nilai Berbeda)" << endl; 
        int mode; cout << ">> Pilihan: "; cin >> mode;

        map<int, Unit> units;
        map<int, vector<int>> adj;
        vector<int> inputs;

        if (mode == 1) {
            for(int val : standard_n) inputs.push_back(val);
        } else {
            cout << "\nSilakan masukkan 4 variasi jumlah unit (N):" << endl;
            for(int i = 1; i <= 4; i++) {
                int val; cout << "   Masukkan N ke-" << i << ": "; cin >> val;
                inputs.push_back(val);
            }
        }

        cout << "\n>>> HASIL ANALISIS (" << selectedHero.heroName << " - Radius: " << activeSkill.radius << "m) <<<" << endl;
        
        printTableHeader();
        for (int n : inputs) {
            generateGraph(n, selectedHero.heroName, units, adj);
            runBenchmarkRow(n, activeSkill.radius, units, adj);
        }
        cout << "+-------+----------+--------------+--------------+--------------+--------------+" << endl;
        
        cout << "\nUlangi? (y/n): "; cin >> ulang;

    } while (ulang == 'y' || ulang == 'Y');

    return 0;
}