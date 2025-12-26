#include "HeroAoE.h"
#include <cmath>        
#include <iostream>

using namespace std;

double getDistance(Position p1, Position p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void analyzeBFS(int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    queue<int> q;
    set<int> visited;
    
    q.push(startId);
    visited.insert(startId);
    
    while (!q.empty()) {
        int currId = q.front();
        q.pop();
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