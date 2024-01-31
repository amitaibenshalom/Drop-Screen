# simulation of a row of nozzles dripping water from above

import pygame
import math
import time

height = 1.5 # meters
g = 9.8 # m/s^2
N = 20 # number of rows in image
W = 64 # number of columns in image

# color of the rainbows
Colors = [(255, 0, 0), (255, 127, 0), (255, 255, 0), (0, 255, 0), (0, 0, 255), (75, 0, 130), (148, 0, 211)]
screen_width = 800
screen_height = 600
water_width = 10
pixels_per_meter = screen_height / 3
pause_time = 3 # seconds

def calc_time_row(row):
    global height, g, N
    return math.sqrt(height / ( N*g*(N - row - 0.5)))


pygame.init()
screen = pygame.display.set_mode((screen_width, screen_height))
screen.fill((0, 0, 0))
pygame.display.set_caption("Simulation")

running = True
water_on = False
last_time = time.time()
row = 0
points = []
starting_times = []
time_per_row = []
clock = pygame.time.Clock()
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            running = False
    if not water_on:
        water_on = True
        last_time = time.time()
        points.append(0)
        starting_times.append(time.time())
        time_per_row.append(calc_time_row(row))
    else:
        if time.time() - last_time > time_per_row[row]:
            water_on = False
            row += 1
            if row == N:
                time.sleep(pause_time)
                screen.fill((0, 0, 0))
                pygame.display.flip()
                row = 0
                last_time = time.time()
                points.clear()
                starting_times.clear()
                time_per_row.clear()
    if points:
        for i in range(len(points)):
            points[i] = 0.5 * g * (time.time() - starting_times[i])**2 
        screen.fill((0, 0, 0))
        for i in range(len(points)-1):
            pygame.draw.line(screen, Colors[i % len(Colors)], (screen_width/2, points[i] * pixels_per_meter), (screen_width/2, points[i+1] * pixels_per_meter), water_width)
        pygame.draw.line(screen, Colors[(len(points)-1) % len(Colors)], (screen_width/2, points[-1] * pixels_per_meter), (screen_width/2, 0), water_width)
        pygame.display.flip()
