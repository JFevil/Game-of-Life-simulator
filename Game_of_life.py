import tkinter as tk

class GameOfLife:
    def __init__(self, size):
        self.size = size
        self.live_cells = set()

    def randomize(self):
        import random
        for i in range(self.size):
            for j in range(self.size):
                if random.random() < 0.2:
                    self.live_cells.add((i, j))

    def next_generation(self):
        new_live_cells = set()
        potential_cells = set()
        for cell in self.live_cells:
            neighbors = self.get_neighbors(cell)
            live_neighbors = sum((neighbor in self.live_cells) for neighbor in neighbors)
            if live_neighbors == 2 or live_neighbors == 3:
                new_live_cells.add(cell)
            potential_cells.update(neighbors)
        
        for cell in potential_cells:
            if cell not in self.live_cells:
                neighbors = self.get_neighbors(cell)
                live_neighbors = sum((neighbor in self.live_cells) for neighbor in neighbors)
                if live_neighbors == 3:
                    new_live_cells.add(cell)
        
        self.live_cells = new_live_cells

    def get_neighbors(self, cell):
        i, j = cell
        return [(i-1, j-1), (i-1, j), (i-1, j+1), (i, j-1), (i, j+1), (i+1, j-1), (i+1, j), (i+1, j+1)]

class GameOfLifeApp:
    def __init__(self, size):
        self.game = GameOfLife(size)
        self.game.randomize()
        self.root = tk.Tk()
        self.canvas = tk.Canvas(self.root, width=size*10, height=size*10)
        self.canvas.pack()
        self.root.after(1000//60, self.update)
        self.root.mainloop()

    def update(self):
        self.game.next_generation()
        self.draw()
        self.root.after(1000//60, self.update)

    def draw(self):
        self.canvas.delete('all')
        for cell in self.game.live_cells:
            i, j = cell
            self.canvas.create_rectangle(j*10, i*10, (j+1)*10, (i+1)*10, fill='black')

if __name__ == '__main__':
    GameOfLifeApp(100)