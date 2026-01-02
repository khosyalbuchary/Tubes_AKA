#include "HeroAoE.h"
#include <cmath>
#include <iostream>

using namespace std;

double getDistance(Position p1, Position p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<int> analyzeBFSIterative(int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    vector<int> hitList; 
    queue<int> q;         
    set<int> visited;
    
    q.push(startId);
    visited.insert(startId);
    
    while (!q.empty()) {    
        int currId = q.front();
        q.pop();

        if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
            if (currId != startId) hitList.push_back(currId);

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

void analyzeBFSRecursive(queue<int>& q, int startId, double radius, map<int, Unit>& units, 
                         map<int, vector<int>>& adj, set<int>& visited, vector<int>& hitList) {
    if (q.empty()) return; 

    int currId = q.front();
    q.pop();

    if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
        if (currId != startId) hitList.push_back(currId);

        for (int neighbor : adj[currId]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    analyzeBFSRecursive(q, startId, radius, units, adj, visited, hitList);
}

vector<int> analyzeDFSIterative(int startId, double radius, map<int, Unit>& units, map<int, vector<int>>& adj) {
    vector<int> hitList;
    stack<int> s;         
    set<int> visited;

    s.push(startId);
    
    while (!s.empty()) {   
        int currId = s.top();
        s.pop();

        if (visited.find(currId) == visited.end()) {
            visited.insert(currId); 

            if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
                if (currId != startId) hitList.push_back(currId);
                for (auto it = adj[currId].rbegin(); it != adj[currId].rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        s.push(*it);
                    }
                }
            }
        }
    }
    return hitList;
}

void analyzeDFSRecursive(int currId, int startId, double radius, map<int, Unit>& units, 
                         map<int, vector<int>>& adj, set<int>& visited, vector<int>& hitList) {
    visited.insert(currId); 
    
    if (getDistance(units[startId].pos, units[currId].pos) <= radius) {
        if (currId != startId) hitList.push_back(currId);

        for (int neighbor : adj[currId]) {
            if (visited.find(neighbor) == visited.end()) {
                analyzeDFSRecursive(neighbor, startId, radius, units, adj, visited, hitList);
            }
        }
    }
}