#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <iomanip>
#include <string>
#include <chrono>

using namespace std;

// --- Struktur Data ---
struct Position { double x, y; };
struct Unit { 
    int id; 
    string name; 
    Position pos; 
    bool is_ally; 
};
struct Skill { 
    string skillName; 
    double radius; 
};
struct Hero { 
    string heroName; 
    vector<Skill> skills; 
};

// Fungsi Jarak Euclidean (O(1))
double getDistance(Position p1, Position p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// --- Metode BFS (Iteratif) ---
void analyzeBFS(int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    queue<int> q;
    set<int> visited;
    q.push(startId);
    visited.insert(startId);
    while (!q.empty()) {
        int currId = q.front(); q.pop();
        if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
            for (int neighbor : adj[currId]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }
}

// --- Metode DFS (Rekursif) ---
void analyzeDFSRecursive(int currId, int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj, set<int>& visited) {
    visited.insert(currId);
    if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
        for (int neighbor : adj[currId]) {
            if (visited.find(neighbor) == visited.end()) {
                analyzeDFSRecursive(neighbor, startId, radius, units, adj, visited);
            }
        }
    }
}

// Fungsi Generate Tabel Performa
void generatePerformanceTable(string heroName, Skill selectedSkill, map<int, Unit>& units, map<int, vector<int>>& adj) {
    int n_values[] = {10, 20, 100, 1000, 10000};
    double r = selectedSkill.radius;

    cout << "\n>>> PERFORMANCE COMPARISON: " << heroName << " <<<" << endl;
    cout << "Skill: " << selectedSkill.skillName << " (Radius: " << r << ")" << endl;
    cout << "+----------+--------------------------+--------------------------+" << endl;
    cout << "|    n     |  Metode DFS(Rekursif)(ms)|  Metode BFS(Iteratif)(ms)|" << endl;
    cout << "+==========+==========================+==========================+" << endl;

    for (int n : n_values) {
        // Test DFS Rekursif
        auto t1 = chrono::high_resolution_clock::now();
        for(int i=0; i<n; i++) {
            set<int> v;
            analyzeDFSRecursive(1, 1, r, units, adj, v);
        }
        auto t2 = chrono::high_resolution_clock::now();
        double timeRec = chrono::duration<double, milli>(t2-t1).count();

        // Test BFS Iteratif
        auto t3 = chrono::high_resolution_clock::now();
        for(int i=0; i<n; i++) analyzeBFS(1, r, units, adj);
        auto t4 = chrono::high_resolution_clock::now();
        double timeIter = chrono::duration<double, milli>(t4-t3).count();

        cout << "| " << setw(8) << left << n 
             << " | " << setw(24) << fixed << setprecision(6) << timeRec 
             << " | " << setw(24) << timeIter << " |" << endl;
        cout << "+----------+--------------------------+--------------------------+" << endl;
    }
}

int main() {
    map<int, Hero> heroList;
    // Inisialisasi Hero & Skill
    Hero h1; h1.heroName = "Belerick"; h1.skills.push_back({"Ancient Seed", 2.5}); h1.skills.push_back({"Nature's Wrath", 4.0}); heroList[1] = h1;
    Hero h2; h2.heroName = "Alice";    h2.skills.push_back({"Flowing Blood", 2.0}); h2.skills.push_back({"Blood Ode", 3.5});     heroList[2] = h2;
    Hero h3; h3.heroName = "Claude";   h3.skills.push_back({"Art of Thievery", 2.2}); h3.skills.push_back({"Blazing Duet", 3.8});   heroList[3] = h3;
    Hero h4; h4.heroName = "Odette";   h4.skills.push_back({"Avian Authority", 2.8}); h4.skills.push_back({"Swan Song", 4.5});      heroList[4] = h4;
    Hero h5; h5.heroName = "Atlas";    h5.skills.push_back({"Annihilate", 2.3}); h5.skills.push_back({"Final Justice", 3.5});  heroList[5] = h5;

    map<int, Unit> gameUnits;
    gameUnits[1] = {1, "Player", {0, 0}, true};
    gameUnits[2] = {2, "Enemy1", {1.5, 0.5}, false};
    gameUnits[3] = {3, "Enemy2", {2.8, 2.0}, false};
    gameUnits[6] = {6, "Minion", {2.0, 1.2}, false};

    map<int, vector<int>> adj;
    adj[1] = {2, 6}; adj[2] = {1, 3}; adj[3] = {2}; adj[6] = {1};
    int c[2], s[2];
    for(int j=0; j<2; j++) {
        cout << "\n--- PEMILIHAN HERO KE-" << j+1 << " ---" << endl;
        for (int i = 1; i <= 5; i++) cout << i << ". " << heroList[i].heroName << endl;
        cout << "Pilih Hero (1-5): "; cin >> c[j];
        cout << "Pilih Skill (1. Skill 1 / 2. Ultimate): "; cin >> s[j];
    }

    generatePerformanceTable(heroList[c[0]].heroName, heroList[c[0]].skills[s[0]-1], gameUnits, adj);
    generatePerformanceTable(heroList[c[1]].heroName, heroList[c[1]].skills[s[1]-1], gameUnits, adj);

    cout << "\n[ PENJELASAN ALGORITMA ]" << endl;
    cout << "1. BFS (Iteratif): Menelusuri unit secara radial (melingkar). Lebih stabil untuk Area of Effect." << endl;
    cout << "2. DFS (Rekursif): Menelusuri jalur hingga terdalam. Memanfaatkan stack sistem otomatis." << endl;
    cout << "\n[ KESIMPULAN ]" << endl;
    cout << "Keduanya memiliki kompleksitas waktu O(V + E). Pada n yang besar, pendekatan iteratif" << endl;
    cout << "cenderung lebih stabil dalam penggunaan memori dibandingkan rekursif." << endl;

    return 0;
}