from math import sqrt
import random
import numpy as np
import matplotlib.pyplot as plt

class Position():
    def __init__(self, x, y, theta=0):
        self.x = x
        self.y = y
        self.theta = theta

class Pathfinder():
    def __init__(self, start=Position(100,100), target=Position(2900, 1900), map_size=[3000, 2000]):
        self.start = start
        self.target = target
        self.map_size = map_size
        self.obstacle_map = np.zeros((map_size[1], map_size[0]), dtype='int32')
        
        self.add_stack(Position(900, 900), Position(1300, 1000))
        self.add_stack(Position(1700, 900), Position(2100, 1000))
        self.add_stack(Position(1950, 1700), Position(2350, 1800))
        self.add_stack(Position(650, 1700), Position(1050, 1800))
        self.add_stack(Position(550, 0), Position(950, 100))
        self.add_stack(Position(2050, 0), Position(2450, 100))
        self.add_stack(Position(2600, 450), Position(2999, 550))
        self.add_stack(Position(0, 450), Position(400, 550))
        self.add_stack(Position(2600, 1000), Position(2999, 1100))
        self.add_stack(Position(0, 1000), Position(400, 1100))
        
        self.add_obstacle(Position(650, 1800), Position(2350, 1999))
        self.add_obstacle(Position(1050, 1500), Position(1950, 1800))
        
        
        
    def set_start_target(self, start: Position, target: Position):
        self.start = start
        self.target = target
        
    def distance(self, node1: Position, node2: Position):
        return int(sqrt((node1.x - node2.x)**2+(node1.y - node2.y)**2))
    
    def add_stack(self, pos1: Position, pos2: Position):
        self.obstacle_map[pos1.y:pos2.y, pos1.x:pos2.x] = 2
        
    def add_obstacle(self, pos1: Position, pos2: Position):
        self.obstacle_map[pos1.y:pos2.y, pos1.x:pos2.x] = 1

    def collission(self, node1: Position, node2: Position):
        distance = self.distance(node1, node2)
        for i in range(distance):
            t = i / distance
            x = int(node1.x + t * (node2.x - node1.x))
            y = int(node1.y + t * (node2.y - node1.y))
                        
            if self.obstacle_map[y, x] != 0:
                return True
            
        return False
    
    def plan(self):    
        path = []
            
        if not self.collission(self.start, self.target): return None
        
        possibilities = []
        while(len(possibilities) < 100):
            pos = Position(random.randint(0, 2999), random.randint(0, 1999))
            if not self.collission(self.start, pos) and not self.collission(pos, self.target):
                possibilities.append(pos)
                
        best_pos = [0, 9999]
        for x in possibilities:
            dist = self.distance(self.start, x) + self.distance(x, self.target)
            # print("Possibility: x=" + str(x.x) + ", y=" + str(x.y) + ", dist=" + str(dist))
            if dist < best_pos[1]:
                best_pos[0] = possibilities.index(x)
                best_pos[1] = dist
        
        # print("Best possibility: " + str(best_pos[0]))
        
        return possibilities[best_pos[0]]
    
    def display(self, path: list[Position]):
        plt.imshow(self.obstacle_map, cmap='gray', origin='lower')
        
        # Zwischenstep
        if path is not None:
            plt.plot([self.target.x, path.x, self.start.x], [self.target.y, path.y, self.start.y], '-r', linewidth=2, label='Path')  # Finalen Pfad in Rot zeichnen
            plt.plot(path.x, path.y, 'bo', label='Step')
        else: 
            plt.plot([self.target.x, self.start.x], [self.target.y, self.start.y], '-r', linewidth=2, label='Path')  # Finalen Pfad in Rot zeichnen
    
        # Start- und Zielpunkte
        plt.plot(self.start.x, self.start.y, 'go', label='Start')
        plt.plot(self.target.x, self.target.y, 'ro', label='Target')

        # Plot-Anpassungen
        plt.xlim(0, self.map_size[0])
        plt.ylim(0, self.map_size[1])
        plt.legend()
        plt.grid(True)
        plt.show()
        
    def proccess(self) -> None:
        path = self.plan()
        self.display(path)

pathfinder = Pathfinder()
pathfinder.proccess()
