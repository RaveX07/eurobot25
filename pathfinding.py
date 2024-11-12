from math import sqrt
import random
import numpy as np
import matplotlib.pyplot as plt

class Position():
    def __init__(self, x, y, theta):
        self.x = x
        self.y = y
        self.theta = theta

class Pathfinder():
    def __init__(self, start, goal, map_size, obstacle_map):
        self.start = start
        self.goal = goal
        self.map_size = map_size
        self.obstacle_map = obstacle_map
        
    def distance(self, node1: Position, node2: Position):
        return int(sqrt((node1.x - node2.x)**2+(node1.y - node2.y)**2))
        

    def collission(self, node1: Position, node2: Position):
        distance = self.distance(node1, node2)
        print(distance)
        for i in range(distance):
            t = i / distance
            x = int(start.x + t * (node2.x - node1.x))
            y = int(start.y + t * (node2.y - node1.y))
            # print(str(x) + "," + str(y))
            if obstacle_map[y, x] != 0:
                return True
            
        return False
    
        
    def plan(self):
        pos = self.goal
        
        while(self.collission(self.start, pos) or self.collission(pos, self.goal)):
            pos = Position(random.randint(0, 3000), random.randint(0, 2000), 0)
            
        print("Position: x=" + str(pos.x) + ", y=" + str(pos.y))
        return pos




start = Position(100, 100, 0)
goal = Position(2900, 1900, 0)
map_size = [3000, 2000]

# 2D obstacle map (1 = obstacle, 0 = free space)
obstacle_map = np.zeros((map_size[1], map_size[0]), dtype='int32')
obstacle_map[900:1100, 1300:1700] = 1  # Beispiel-Hindernis
obstacle_map[400:600, 2000:2400] = 1  # Beispiel-Hindernis
obstacle_map[1500:1700, 1600:2000] = 1  # Beispiel-Hindernis
# obstacle_map[150:250, 150:250] = 1  # weiteres Beispiel-Hindernis

pathfinder = Pathfinder(start, goal, map_size, obstacle_map)
path = pathfinder.plan()


plt.imshow(obstacle_map, cmap='gray', origin='lower')

# Linien
plt.plot([goal.x, path.x, start.x], [goal.y, path.y, start.y], '-r', linewidth=2, label='Path')  # Finalen Pfad in Rot zeichnen

# Start- und Zielpunkte
plt.plot(start.x, start.y, 'go', label='Start')
plt.plot(goal.x, goal.y, 'ro', label='Goal')

# Zwischenstep
plt.plot(path.x, path.y, 'bo', label='Step')


# Plot-Anpassungen
plt.xlim(0, map_size[0])
plt.ylim(0, map_size[1])
plt.legend()
plt.grid(True)
plt.show()