#include <math.h>
#include <vector>
#include <random>

// Konstanten
const int MAX_X = 2999;
const int MAX_Y = 1999;
const int BOT_WIDTH = 180;
const int MAX_ITERS = 1250;

// Hilfsfunktion, um Werte auf Limits zu begrenzen
int add_with_limits(int a, int b, int max_value) {
    int result = a + b;
    if (result > max_value) return max_value;
    if (result < 0) return 0;
    return result;
}

// Position-Klasse
class Position {
public:
    int x, y;

    Position(int xPos, int yPos) {
        x = xPos;
        y = yPos;
        limit();
    }

    void limit() {
        if (x > MAX_X) x = MAX_X;
        if (x < 0) x = 0;
        if (y > MAX_Y) y = MAX_Y;
        if (y < 0) y = 0;
    }
};

// Struct für parallele Punkte
struct ParallelPoints {
    Position point1;
    Position point2;

    ParallelPoints(Position p1, Position p2) : point1(p1), point2(p2) {}
};

// Pathfinder-Klasse
class Pathfinder {
private:
    Position start;
    Position target;
    int map[MAX_Y + 1][MAX_X + 1] = {0}; // Hindernis-Karte


    // Hindernis hinzufügen
    void add_obstacle(Position pos1, Position pos2) {
        for (int y = pos1.y; y <= pos2.y; y++) {
            for (int x = pos1.x; x <= pos2.x; x++) {
                map[y][x] = 1;
            }
        }
    }

    // Stack hinzufügen
    void add_stack(Position pos1, Position pos2) {
        for (int y = pos1.y; y <= pos2.y; y++) {
            for (int x = pos1.x; x <= pos2.x; x++) {
                map[y][x] = 2;
            }
        }
    }

    // Distanz zwischen zwei Punkten berechnen
    int distance(Position node1, Position node2){
        return sqrt(pow(node2.x - node1.x, 2) + pow(node2.y - node1.y, 2));
    }

    // Parallelen zu Pfad berechnen --> Dimensionen vom Bot miteinbeziehen
    ParallelPoints calculate_parallel_points(Position pos1, Position pos2, int distance) {
        // Richtungsvektor berechnen
        float dx = pos2.x - pos1.x;
        float dy = pos2.y - pos1.y;

        // Länge des Vektors
        float magnitude = sqrt(dx * dx + dy * dy);

        // Senkrechter Einheitsvektor
        float unit_nx = -dy / magnitude;
        float unit_ny = dx / magnitude;

        // Verschiebung um die gegebene Distanz
        float shift_x = distance * unit_nx;
        float shift_y = distance * unit_ny;

        // create new points
        Position parallel1(pos1.x + shift_x, pos1.y + shift_y);
        Position parallel2(pos2.x + shift_x, pos2.y + shift_y);

        return ParallelPoints(parallel1, parallel2);
    }

    // Kollisionsprüfung
    bool collission(Position node1, Position node2) {
        int dis = distance(node1, node2);
        for (int i = 0; i <= dis; i++) {
            float t = (float)i / dis;
            int x = node1.x + t * (node2.x - node1.x);
            int y = node1.y + t * (node2.y - node1.y);
            if (map[y][x] != 0) return true;
        }
        return false;
    }

    boolean collission_with_bot(Position node1, Position node2){
        // outer dimmension of bot 
        ParallelPoints left = calculate_parallel_points(node1, node2, BOT_WIDTH);
        ParallelPoints right = calculate_parallel_points(node1, node2, -BOT_WIDTH);
        
        // check if bot does not collide 
        if(collission(node1, node2) || collission(left.point1, left.point2) || collission(right.point1, right.point2)){
            return true;
        }        
        return false;
    }

public:
    // Konstruktor
    Pathfinder(Position startPos, Position targetPos) : start(startPos), target(targetPos) {
        // Hindernisse hinzufügen
        add_obstacle(Position(650, 1800), Position(2350, 1999));
        add_obstacle(Position(1050, 1500), Position(1950, 1800));
        add_obstacle(Position(0, 1550), Position(150, 1999));
        add_obstacle(Position(2850, 1550), Position(2999, 1999));
    }

    // Pfad planen
    void plan() {
        if (!collission_with_bot(start, target)) {
            Serial.println("Direkter Pfad möglich!");
            return;
        }

        std::vector<Position> positions;
        for(int i = 0; i < MAX_ITERS; i++){
            Position pos = Position(random(0, 2999), random(0, 1999));
            if(!collission_with_bot(start, pos) && !collission_with_bot(pos, target)){
                positions.push_back(pos);
            }
        }
        Serial.println(positions.size());
        if(positions.size() > 0) Serial.println("Kein Pfad gefunden!");

        int best_index = 0;
        int best_distance = 99999;
        for(size_t x = 0; x < positions.size(); x++){
            Serial.print("Position ");
            Serial.print(x);
            Serial.print(": (");
            Serial.print(positions[x].x);
            Serial.print(", ");
            Serial.print(positions[x].y);
            Serial.println(")");

            int dist = distance(start, positions[x]) + distance(positions[x], target);
            if(dist < best_distance){
                best_index = x;
                best_distance = dist;
            }
        }

        Serial.print("Best Pos: ");
        Serial.print(best_index);
        Serial.print(": (");
        Serial.print(positions[best_index].x);
        Serial.print(", ");
        Serial.print(positions[best_index].y);
        Serial.println(")");

    }
};