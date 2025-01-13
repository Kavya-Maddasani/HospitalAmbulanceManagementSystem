#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
using namespace std;

// Ambulance Struct
struct Ambulance {
    int id;
    int currentLocation;
    bool isAvailable;
    Ambulance(int id, int location) : id(id), currentLocation(location), isAvailable(true) {}
};

// Hospital Struct
struct Hospital {
    int id;
    int location;
    int capacity;
    Hospital(int id, int location, int capacity) : id(id), location(location), capacity(capacity) {}
};

// Emergency Request Struct
struct Request {
    int id;
    int emergencyLocation;
    int priority;
    Request(int id, int location, int priority) : id(id), emergencyLocation(location), priority(priority) {}
};

// Comparator for Priority Queue
struct ComparePriority {
    bool operator()(const Request& r1, const Request& r2) {
        return r1.priority < r2.priority;
    }
};

// Dijkstra's Algorithm for Shortest Path
int dijkstra(const vector<vector<pair<int, int>>>& graph, int source, int target) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    vector<int> distances(graph.size(), INT_MAX);
    distances[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int distance = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (node == target) return distance;

        for (const auto& neighbor : graph[node]) {
            int newDistance = distance + neighbor.second;
            if (newDistance < distances[neighbor.first]) {
                distances[neighbor.first] = newDistance;
                pq.push({newDistance, neighbor.first});
            }
        }
    }
    return INT_MAX;
}

// Find Nearest Available Ambulance
int findNearestAmbulance(vector<Ambulance>& ambulances, int emergencyLocation, const vector<vector<pair<int, int>>>& graph) {
    int nearestAmbulance = -1;
    int minDistance = INT_MAX;

    for (auto& ambulance : ambulances) {
        if (ambulance.isAvailable) {
            int distance = dijkstra(graph, ambulance.currentLocation, emergencyLocation);
            if (distance < minDistance) {
                minDistance = distance;
                nearestAmbulance = ambulance.id;
            }
        }
    }
    return nearestAmbulance;
}

// Handle Emergency Request
void handleRequest(priority_queue<Request, vector<Request>, ComparePriority>& requests, vector<Ambulance>& ambulances, const vector<vector<pair<int, int>>>& graph) {
    while (!requests.empty()) {
        Request currentRequest = requests.top();
        requests.pop();

        int ambulanceId = findNearestAmbulance(ambulances, currentRequest.emergencyLocation, graph);
        if (ambulanceId != -1) {
            cout << "Dispatching Ambulance " << ambulanceId << " to Emergency at Location " << currentRequest.emergencyLocation << endl;
            ambulances[ambulanceId].isAvailable = false;
        } else {
            cout << "No available ambulance for Request " << currentRequest.id << endl;
        }
    }
}

// Main Function
int main() {
    vector<Ambulance> ambulances = {
        Ambulance(0, 1), Ambulance(1, 2), Ambulance(2, 3)
    };
    vector<Hospital> hospitals = {
        Hospital(0, 4, 10), Hospital(1, 5, 8)
    };

    vector<vector<pair<int, int>>> graph = {
        {{1, 2}, {2, 4}}, // Node 0
        {{0, 2}, {3, 1}}, // Node 1
        {{0, 4}, {3, 3}}, // Node 2
        {{1, 1}, {2, 3}}, // Node 3
        {{5, 1}},         // Node 4
        {{4, 1}}          // Node 5
    };

    priority_queue<Request, vector<Request>, ComparePriority> requests;
    requests.push(Request(0, 3, 1)); // Request with ID 0, Location 3, Priority 1
    requests.push(Request(1, 2, 2)); // Request with ID 1, Location 2, Priority 2

    handleRequest(requests, ambulances, graph);
    return 0;
}

