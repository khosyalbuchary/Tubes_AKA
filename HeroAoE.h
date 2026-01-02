#ifndef HEROAOE_H
#define HEROAOE_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack> // Wajib untuk DFS Iteratif
#include <map>
#include <set>
#include <string>
#include <cmath>

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


std::vector<int> analyzeBFSIterative(int startId, double radius, std::map<int, Unit>& units, std::map<int, std::vector<int> >& adj);

void analyzeBFSRecursive(std::queue<int>& q, int startId, double radius, std::map<int, Unit>& units, 
                         std::map<int, std::vector<int> >& adj, std::set<int>& visited, std::vector<int>& hitList);

std::vector<int> analyzeDFSIterative(int startId, double radius, std::map<int, Unit>& units, std::map<int, std::vector<int> >& adj);

void analyzeDFSRecursive(int currId, int startId, double radius, std::map<int, Unit>& units, 
                      std::map<int, std::vector<int> >& adj, std::set<int>& visited, std::vector<int>& hitList);

#endif