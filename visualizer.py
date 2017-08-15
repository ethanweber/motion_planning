"""
 Example program to show using an array to back a grid on-screen.

 Sample Python/Pygame Programs
 Simpson College Computer Science
 http://programarcadegames.com/
 http://simpson.edu/computer-science/

 Explanation video: http://youtu.be/mdTeqiWyFnc
"""

# visualization library
import pygame

# line algorithm for filling in the boundaries of the objects defined by points
from bresenham import bresenham

import time


# this gets the outline of points using the bresenham line algorithm
def get_outline( points ):
    all_outline = []
    for i in range(len(points)):
        i_next = (i+1)%len(points)
        all_outline += list(bresenham(points[i][0], points[i][1], points[i_next][0], points[i_next][1]))
    return all_outline


# this gets the outline of points using the bresenham line algorithm
def intersection( points, obstacle_points ):
    # center of mass of points
    pnt_1 = center_of_mass(points)
    pnt_2 = center_of_mass(obstacle_points)

    # print pnt_2
    # print dist(pnt_1, pnt_2)

    if dist(pnt_1, pnt_2) < 5.0:
        return True
    return False


def center_of_mass( points ):
    # center of mass of points
    x_sum = 0.0
    y_sum = 0.0
    for p in points:
        x_sum += p[0]
        y_sum += p[1]
    x_val = x_sum/len(points)
    y_val = y_sum/len(points)
    return (x_val, y_val)

def dist( pnt1, pnt2 ):
    return ( (pnt1[0]-pnt2[0])**2 + (pnt1[1]-pnt2[1])**2 ) ** .5

# These are the values stored in the grid to keep track of empty, robot, obstacle points, and goal
# 0: empty space - white
# 1: robot point - green
# 2: obstacle - red
# 3: goal - yellow


# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
YELLOW = (255, 255, 0)

# This sets the WIDTH and HEIGHT of each grid location
WIDTH = 20
HEIGHT = 20

# This sets the margin between each cell
MARGIN = 5

# Used to initialize the grid
X_LEN = 30
Y_LEN = 30

# Create a 2 dimensional array. A two dimensional
# array is simply a list of lists.
grid = []
for row in range(Y_LEN):
    # Add an empty array that will hold each cell
    # in this row
    grid.append([])
    for column in range(X_LEN):
        grid[row].append(0)  # Append a cell

# Initialize pygame
pygame.init()

# Set the HEIGHT and WIDTH of the screen
WINDOW_SIZE = [255*3-10, 255*3-10]
screen = pygame.display.set_mode(WINDOW_SIZE)

# Set title of screen
pygame.display.set_caption("Motion Planning")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

robot_points = [(10,10), (10,13), (13,13), (13,10)] # set the coordinates of the robot
robot_all = get_outline( robot_points ) # fills in the outline of the robot points with the bresenham line algorithm

# set the value of the robot points in the grid
for coord in robot_all:
    grid[coord[0]][coord[1]] = 1

# set the coordinates of the goal
# goal_points = [(20,20), (20,23), (23,23), (23,20)]
# goal_points = [(0,0), (0,3), (3,3), (3,0)]
goal_points = [(22,2), (22,5), (25,5), (25,2)]
goal_all = get_outline( goal_points )
for coord in goal_all:
    grid[coord[0]][coord[1]] = 3


# set the coordinates of the obstacles
obj_points = [(22,9), (22,12), (25,12), (25,9)]
obj_all = get_outline( obj_points )
for coord in obj_all:
    grid[coord[0]][coord[1]] = 2

# define the potential moves that can occur
moves = [(0,1), (1,0), (0,-1), (-1,0)]

q = []

q.append([ robot_all, [] ]) # robot points, array of moves

path = []
i = 0
while (i < len(q)):
    # get the object points at this step
    x = q[i][0] # position
    prev_moves = q[i][1] # moves to position
    i += 1
    if x[0] == goal_points[0]: # if position is the goal
        print "Success"
        print prev_moves
        path = prev_moves
        break
    for move in moves:
        new_points = []
        new_moves = prev_moves + [move]
        # add the move to each of the points
        for point in x:
            new_points.append( (point[0] + move[0], point[1] + move[1]) )

        found = False
        for v in q:
            if new_points == v[0]:
                found = True
                break
        if not found:
            # make sure it's not an obstacle
            # if new_points != obj_all:
            if not intersection(new_points, obj_all):
                q.append([new_points, new_moves])

print "Failure"

current_points = robot_all

for move in path:

    # Set the screen background
    screen.fill(BLACK)

    new_points = []
    for point in current_points:
        new_points.append( (point[0] + move[0], point[1] + move[1]) )

    # set the value of the robot points in the grid
    for coord in current_points:
        grid[coord[0]][coord[1]] = 0

    # set the value of the robot points in the grid
    for coord in new_points:
        grid[coord[0]][coord[1]] = 1

    # Draw the grid
    for row in range(Y_LEN):
        for column in range(X_LEN):
            color = WHITE
            if grid[row][column] == 1: color = GREEN
            if grid[row][column] == 2: color = RED
            if grid[row][column] == 3: color = YELLOW
            pygame.draw.rect(screen,
                             color,
                             [(MARGIN + WIDTH) * row + MARGIN,
                              (MARGIN + HEIGHT) * column + MARGIN,
                              WIDTH,
                              HEIGHT])

    # Limit to 60 frames per second
    clock.tick(60)

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    current_points = new_points

    time.sleep(.1)



# implement the path planning algorithms here and then step through the generated path in the while loop


# # -------- Main Program Loop -----------
# while not done:
#
#     for event in pygame.event.get():  # User did something
#         if event.type == pygame.QUIT:  # If user clicked close
#             done = True  # Flag that we are done so we exit this loop
#
#     # Set the screen background
#     screen.fill(BLACK)
#
#     # Draw the grid
#     for row in range(Y_LEN):
#         for column in range(X_LEN):
#             color = WHITE
#             if grid[row][column] == 1: color = GREEN
#             if grid[row][column] == 2: color = RED
#             if grid[row][column] == 3: color = YELLOW
#             pygame.draw.rect(screen,
#                              color,
#                              [(MARGIN + WIDTH) * row + MARGIN,
#                               (MARGIN + HEIGHT) * column + MARGIN,
#                               WIDTH,
#                               HEIGHT])
#
#     # Limit to 60 frames per second
#     clock.tick(60)
#
#     # Go ahead and update the screen with what we've drawn.
#     pygame.display.flip()

# Be IDLE friendly. If you forget this line, the program will 'hang'
# on exit.
pygame.quit()
