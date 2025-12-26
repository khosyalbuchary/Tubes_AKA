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

// --- Helper: Generate Musuh dengan Nama & Posisi ---
void generateGraph(int n, string currentHeroName, map<int, Unit>& units, map<int, vector<int>>& adj) {
    units.clear();
    adj.clear();
    
    // HERO SELALU DI TENGAH (0.0, 0.0)
    units[1] = {1, currentHeroName + " (YOU)", {0.0, 0.0}, true};
    
    // Database Nama Unit
    vector<string> mobTypes;
    mobTypes.push_back("Minion Melee");
    mobTypes.push_back("Minion Range");
    mobTypes.push_back("Siege Minion");
    mobTypes.push_back("Lithowanderer");
    mobTypes.push_back("Crimson Lizard");
    mobTypes.push_back("Purple Buff");
    mobTypes.push_back("Orange Buff");
    mobTypes.push_back("Turtle");
    mobTypes.push_back("Layla (Bot)");
    mobTypes.push_back("Zilong (Bot)");

    // Generate musuh di area sekitar (-5.0 sampai 5.0 meter)
    for(int i = 2; i <= n; i++) {
        // Koordinat Random
        double rX = ((rand() % 100) / 10.0) - 5.0; 
        double rY = ((rand() % 100) / 10.0) - 5.0;
        
        string randomName = mobTypes[rand() % mobTypes.size()];
        units[i] = {i, randomName, {rX, rY}, false};
        
        // Koneksi Graf
        adj[i].push_back(i-1);
        adj[i-1].push_back(i);
        if (i > 2) {
            int randomNode = (rand() % (i-1)) + 1;
            adj[i].push_back(randomNode);
            adj[randomNode].push_back(i);
        }
    }
}

// --- FUNGSI TAMPILAN BARU: CETAK KOORDINAT & JARAK ---
void printHitResult(int n, const vector<int>& hits, const map<int, Unit>& units) {
    cout << "   -> Hasil Deteksi (N=" << n << "): ";
    
    if (hits.empty()) {
        cout << "Meleset! Musuh terlalu jauh." << endl;
    } else if (n <= 20) {
        // Tampilkan Detail
        cout << hits.size() << " unit terkena skill." << endl;
        cout << "      -----------------------------------------------------------" << endl;
        cout << "      | ID  | Nama Unit        | Posisi (x, y)  | Jarak ke Hero |" << endl;
        cout << "      -----------------------------------------------------------" << endl;
        
        for (int id : hits) {
            Unit musuh = units.at(id);
            Unit hero = units.at(1); // Hero selalu ID 1
            
            // Hitung jarak lagi cuma buat ditampilkan ke layar
            double jarak = getDistance(hero.pos, musuh.pos);

            cout << "      | " << setw(3) << left << id 
                 << " | " << setw(16) << musuh.name
                 << " | (" << setw(4) << musuh.pos.x << ", " << setw(4) << musuh.pos.y << ") "
                 << " | " << setw(5) << fixed << setprecision(2) << jarak << "m      |" << endl;
        }
        cout << "      -----------------------------------------------------------" << endl;
    } else {
        cout << "Total " << hits.size() << " unit terkena hit." << endl;
    }
}

vector<Hero> initializeHeroes() {
    vector<Hero> list;
    // 1. Belerick
    Hero h1; h1.heroName = "Belerick";
    h1.skills.push_back({"Ancient Seed (S1)", 2.5}); 
    h1.skills.push_back({"Nature's Wrath (Ult)", 4.0});
    list.push_back(h1);
    // 2. Alice
    Hero h2; h2.heroName = "Alice";
    h2.skills.push_back({"Flowing Blood (S1)", 2.0});
    h2.skills.push_back({"Blood Ode (Ult)", 3.5}); 
    list.push_back(h2);
    // 3. Tigreal
    Hero h3; h3.heroName = "Tigreal";
    h3.skills.push_back({"Attack Wave (S1)", 2.8});
    h3.skills.push_back({"Implosion (Ult)", 3.2}); 
    list.push_back(h3);
    // 4. Odette
    Hero h4; h4.heroName = "Odette";
    h4.skills.push_back({"Avian Authority (S1)", 2.2});
    h4.skills.push_back({"Swan Song (Ult)", 5.0});
    list.push_back(h4);
    // 5. Yve
    Hero h5; h5.heroName = "Yve";
    h5.skills.push_back({"Void Blast (S1)", 2.0});
    h5.skills.push_back({"Real World Manipulation", 6.0});
    list.push_back(h5);
    return list;
}

int main() {
    srand(time(0)); 
    vector<Hero> heroes = initializeHeroes();
    int n_values[] = {10, 20, 100, 1000, 10000}; 
    char ulang;

    do {
        // system("cls"); // Hapus komentar jika Windows
        cout << "\n==========================================" << endl;
        cout << "   MOBA AoE ALGORITHM SIMULATOR (BFS vs DFS)" << endl;
        cout << "==========================================" << endl;
        
        // Pilih Hero
        cout << "Pilih Hero:" << endl;
        for (size_t i = 0; i < heroes.size(); ++i) cout << i + 1 << ". " << heroes[i].heroName << endl;
        int heroChoice; cout << "\n>> Nomor Hero: "; cin >> heroChoice;
        if (heroChoice < 1 || heroChoice > heroes.size()) continue;
        Hero selectedHero = heroes[heroChoice - 1];
        
        // Pilih Skill
        cout << "\n>>> Hero: " << selectedHero.heroName << " <<<" << endl;
        for (size_t i = 0; i < selectedHero.skills.size(); ++i) 
            cout << i + 1 << ". " << selectedHero.skills[i].skillName << " (Radius: " << selectedHero.skills[i].radius << ")" << endl;
        int skillChoice; cout << ">> Nomor Skill: "; cin >> skillChoice;
        if (skillChoice < 1 || skillChoice > selectedHero.skills.size()) continue;
        Skill activeSkill = selectedHero.skills[skillChoice - 1];

        // Header Info
        cout << "\n==========================================================" << endl;
        cout << "[ SIMULASI POSISI & JARAK ]" << endl;
        cout << "Hero Posisi : (0.0, 0.0) -> PUSAT" << endl;
        cout << "Skill Radius: " << activeSkill.radius << " meter" << endl;
        cout << "==========================================================" << endl;
        cout << "\n[1] LOGIKA DETEKSI (Siapa yang kena?)" << endl;
        
        map<int, Unit> units;
        map<int, vector<int>> adj;

        // Loop 1: Validasi
        for (int n : n_values) {
            generateGraph(n, selectedHero.heroName, units, adj);
            vector<int> hits = analyzeBFS(1, activeSkill.radius, units, adj); 
            printHitResult(n, hits, units);
        }

        cout << "\n[2] BENCHMARK PERFORMA" << endl;
        cout << "+----------+--------------------------+--------------------------+" << endl;
        cout << "| Unit (n) |  Metode DFS(Rekursif)(ms)|  Metode BFS(Iteratif)(ms)|" << endl;
        cout << "+==========+==========================+==========================+" << endl;

        // Loop 2: Benchmark
        for (int n : n_values) {
            generateGraph(n, selectedHero.heroName, units, adj);
            double r = activeSkill.radius;
            
            // DFS
            auto t1 = chrono::high_resolution_clock::now();
            for(int k=0; k<50; k++) { 
                set<int> visited; vector<int> d; 
                analyzeDFSRecursive(1, 1, r, units, adj, visited, d);
            }
            auto t2 = chrono::high_resolution_clock::now();

            // BFS
            auto t3 = chrono::high_resolution_clock::now();
            for(int k=0; k<50; k++) { analyzeBFS(1, r, units, adj); }
            auto t4 = chrono::high_resolution_clock::now();

            cout << "| " << setw(8) << left << n 
                 << " | " << setw(24) << fixed << setprecision(6) << chrono::duration<double, milli>(t2-t1).count()
                 << " | " << setw(24) << chrono::duration<double, milli>(t4-t3).count() << " |" << endl;
        }
        cout << "+----------+--------------------------+--------------------------+" << endl;
        cout << "\nUlangi? (y/n): "; cin >> ulang;

    } while (ulang == 'y' || ulang == 'Y');

    return 0;
}