#include <math.h>

// Konstanten
const int MAX_X = 2999;
const int MAX_Y = 1999;
const int BOT_WIDTH = 180;

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
    void add_obstacle(Position pos1, Position pos2, int value) {
        for (int y = pos1.y; y <= pos2.y; y++) {
            for (int x = pos1.x; x <= pos2.x; x++) {
                map[y][x] = value;
            }
        }
    }

    ParallelPoints calculate_parallel_points(Position pos1, Position pos2, float distance) {
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
        int distance = sqrt(pow(node2.x - node1.x, 2) + pow(node2.y - node1.y, 2));
        for (int i = 0; i <= distance; i++) {
            float t = (float)i / distance;
            int x = node1.x + t * (node2.x - node1.x);
            int y = node1.y + t * (node2.y - node1.y);
            if (map[y][x] != 0) return true;
        }
        return false;
    }

public:
    // Konstruktor
    Pathfinder(Position startPos, Position targetPos) : start(startPos), target(targetPos) {
        // Hindernisse hinzufügen
        add_obstacle(Position(650, 1800), Position(2350, 1999), 1);
        add_obstacle(Position(1050, 1500), Position(1950, 1800), 1);
        add_obstacle(Position(0, 1550), Position(150, 1999), 1);
        add_obstacle(Position(2850, 1550), Position(2999, 1999), 1);
    }

    // Pfad planen
    void plan() {
        if (!collission(start, target)) {
            Serial.println("Direkter Pfad möglich!");
            return;
        }

        Serial.println("Kein direkter Pfad gefunden.");
    }
};