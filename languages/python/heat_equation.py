# heat equation

#  d                 d2
# --- u(X, t) = D * ---- u(X, t)
#  dt                dx2

# 1D diffusion
# u(x, t+dt) = u(x, t) +
#              dt * D * [u(x+dx, t)+u(x-dx, t)-2u(x, t)]/(dx)/(dx)

# 2D diffusion
# u(x, y, t+dt) = u(x, y, t) +
#                 dt * D * [u(x+dx, y, t)+u(x-dx, y, t)-2u(x, y, t)]/dx/dx +
#                 dt * D * [u(x, y+dy, t)+u(x, y-dy, t)-2u(x, y, t)]/dy/dy

# naive pure python solution

import time
import numpy as np

grid_shape = (1024, 1024)

# ignore out_grid
def evolve1(grid, out_grid, dt, D=1.0, dx=1, dy=1):
    xmax, ymax = grid_shape
    new_grid = [[0.0,]*ymax for x in range(xmax)]
    for i in range(xmax):
        for j in range(ymax):
            grid_xx = grid[(i+1)%xmax][j]+grid[(i-1)%xmax][j]-2.0*grid[i][j]
            grid_yy = grid[i][(j+1)%ymax]+grid[i][(j-1)%ymax]-2.0*grid[i][j]
            new_grid[i][j] = grid[i][j] + \
                             D * (grid_xx/dx/dx + grid_yy/dy/dy) * dt
    return new_grid, out_grid

# use the new grid input
def evolve2(grid, out_grid, dt, D=1.0, dx=1, dy=1):
    xmax, ymax = grid_shape
    for i in range(xmax):
        for j in range(ymax):
            grid_xx = grid[(i+1)%xmax][j]+grid[(i-1)%xmax][j]-2.0*grid[i][j]
            grid_yy = grid[i][(j+1)%ymax]+grid[i][(j-1)%ymax]-2.0*grid[i][j]
            out_grid[i][j] = grid[i][j] + \
                             D * (grid_xx/dx/dx + grid_yy/dy/dy) * dt
    return out_grid, grid

# vectorized
def evolve3(grid, out_grid, dt, D=1.0, dx=1, dy=1):
    def laplacian(grid):
        return (np.roll(grid, +1, 0)+np.roll(grid, -1, 0)-2.0*grid)/dx/dx + \
            (np.roll(grid, +1, 1)+np.roll(grid, -1, 1)-2.0*grid)/dy/dy
            
    return grid + dt * D * laplacian(grid), out_grid

def run_experiment(num_iterations, evolve_func, use_np=True):
    xmax, ymax = grid_shape
    if use_np:
        grid = np.zeros(grid_shape)
        out_grid = np.zeros(grid_shape)
    else:
        grid = [[0.0,]*ymax for x in range(xmax)]
        out_grid = [[0.0,]*ymax for x in range(xmax)]

    for i in range(int(xmax*0.4), int(xmax*0.5)):
        for j in range(int(ymax*0.4), int(ymax*0.5)):
            grid[i][j] = 0.005

    start = time.time()
    for i in range(num_iterations):
        grid, out_grid = evolve_func(grid, out_grid, 0.1)
    return time.time()-start

def simple_loop_comparison():
    xmax, ymax = grid_shape

    py_grid = [[0]*ymax for x in range(xmax)]
    py_ones = [[1]*ymax for x in range(xmax)]

    np_grid = np.zeros(grid_shape)
    np_ones = np.ones(grid_shape)

    def add_with_loop(grid, add_grid, xmax, ymax):
        for x in range(xmax):
            for y in range(ymax):
                grid[x][y] += add_grid[x][y]
    
    repeat = 20
    
    #start = time.time()
    #for i in range(repeat):
    #    add_with_loop(py_grid, py_ones, xmax, ymax)
    #print('pure python loops=', time.time()-start)

    start = time.time()
    for i in range(repeat):
        add_with_loop(np_grid, np_ones, xmax, ymax)
    print('numpy loops=', time.time()-start)
    '''
    start = time.time()
    for i in range(repeat):
        np_grid += np_ones
    print('numpy vector=', time.time()-start)
    '''

if __name__ == "__main__":
    #print(run_experiment(10, evolve1, False))
    #print(run_experiment(10, evolve2, False))
    #print(run_experiment(10, evolve3, True))
    simple_loop_comparison()
    
