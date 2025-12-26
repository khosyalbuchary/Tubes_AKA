#ifndef HEROAOE_H
#define HEROAOE_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <cmath>

// 1. Struktur Koordinat
struct Position {
    double x, y;
};

// 2. Struktur Unit (Sekarang punya NAME)
struct Unit {
    int id;
    std::string name; // <--- Ini penting agar nama musuh bisa muncul
    Position pos;
    bool is_ally; 
};

// 3. Struktur Skill
struct Skill {
    std::string skillName;
    double radius;
};

// 4. Struktur Hero
struct Hero {
    std::string heroName;
    std::vector<Skill> skills;
};

// --- DEKLARASI FUNGSI ---
// (Hanya janji fungsi, isinya di .cpp)

double getDistance(Position p1, Position p2);

// BFS mengembalikan daftar ID yang kena hit
std::vector<int> analyzeBFS(int startId, double radius, std::map<int, Unit>& units, std::map<int, std::vector<int> >& adj);

// DFS menyimpan hasil hit ke dalam parameter 'hitList'
void analyzeDFSRecursive(int currId, int startId, double radius, std::map<int, Unit>& units, 
                      std::map<int, std::vector<int> >& adj, std::set<int>& visited, std::vector<int>& hitList);

#endif