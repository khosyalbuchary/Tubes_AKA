#ifndef HEROAOE_H
#define HEROAOE_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>

// Struktur Data
struct Position { double x, y; };

struct Unit {
    int id;
    std::string name;
    Position pos;
    bool is_ally; 
};

struct Skill { std::string skillName; double radius; };

struct Hero {
    std::string heroName;
    std::vector<Skill> skills;
};

// Deklarasi Fungsi
double getDistance(Position p1, Position p2);
void analyzeIterative(int startId, double radius, std::map<int, Unit>& units, std::map<int, std::vector<int> >& adj);
void analyzeRecursive(int currId, int startId, double radius, std::map<int, Unit>& units, 
                      std::map<int, std::vector<int> >& adj, std::set<int>& visited);

#endif