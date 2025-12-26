#include "HeroAoE.h"
#include <cmath>
#include <iostream>

using namespace std;

// Rumus Jarak Euclidean
double getDistance(Position p1, Position p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Implementasi BFS (Iteratif)
vector<int> analyzeBFS(int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    vector<int> hitList; // Penampung korban
    queue<int> q;
    set<int> visited;
    
    q.push(startId);
    visited.insert(startId);
    
    while (!q.empty()) {
        int currId = q.front();
        q.pop();
        
        // Logika AoE: Cek Jarak
        if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
            
            // Jika unit ini bukan Hero itu sendiri, catat sebagai korban
            if (currId != startId) {
                hitList.push_back(currId);
            }

            // Lanjut cek tetangga
            for (int neighbor : adj[currId]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }
    return hitList;
}

// Implementasi DFS (Rekursif)
void analyzeDFSRecursive(int currId, int startId, double radius, map<int, Unit>& units, 
                         map<int, vector<int>>& adj, set<int>& visited, vector<int>& hitList) {
    visited.insert(currId);
    
    // Logika AoE: Cek Jarak
    if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
        
        // Catat korban
        if (currId != startId) {
            hitList.push_back(currId);
        }

        for (int neighbor : adj[currId]) {
            if (visited.find(neighbor) == visited.end()) {
                analyzeDFSRecursive(neighbor, startId, radius, units, adj, visited, hitList);
            }
        }
    }
}