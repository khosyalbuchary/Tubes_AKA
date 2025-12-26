#ifndef HEROAOE_H
#define HEROAOE_H

#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>

struct Position {
    double x, y;
};

struct Unit {
    int id;
    std::string name;
    Position pos;
    bool is_ally; 
};

struct Skill {
    std::string skillName;
    double radius;
};

struct Hero {
    std::string heroName;
    std::vector<Skill> skills;
};

double getDistance(Position p1, Position p2);

void analyzeBFS(int startId, double radius, std::map<int, Unit>& units, std::map<int, std::vector<int> >& adj);

void analyzeDFSRecursive(int currId, int startId, double radius, std::map<int, Unit>& units, 
                      std::map<int, std::vector<int> >& adj, std::set<int>& visited);

#endif