import os
import serial
from typing import List
# from PIL import Image
from datetime import datetime
import cv2
import time
import pygame
import numpy as np
from pygame.locals import *
    
def take_picture():
    global cap
    ret, frame = cap.read()
    if ret:
        return frame
    return None


def send_to_arduino(byte_list):
    global arduino
    if arduino is not None:
        # for each byte in the byte list send it to the arduino
        for i in range(len(byte_list)):
            arduino.write(byte_list[i])
            if (i + 1) % 8 == 0:
                response = arduino.read(1, 0.1)
                if response is not None:
                    print(response)
                else:
                    print('Error: No response from Arduino')
        print('Data sent to Arduino')
    else:
        print('Error: Arduino not connected')


def process_and_save_image(input_path, output_path):
    global time_per_caputure, log
    image = cv2.imread(input_path)
    if image is not None:
        resized_image = cv2.resize(image, (64, 20))
        gray_image = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
        # Apply binary thresholding to create a black and white image
        _, bw_image = cv2.threshold(gray_image, threshold, 255, cv2.THRESH_BINARY)
        cv2.imwrite(output_path, bw_image)
        flattened_array = bw_image.flatten()
        if log:
            print("len(flattened_array): ", len(flattened_array))
        byte_array = bytearray()
        for i in range(0, len(flattened_array), 8):
            if i % 64 == 0 and log:
                print()
            byte_value = 0
            for j in range(8):
                pixel_value = flattened_array[i + j]
                byte_value |= (pixel_value & 1) << (7 - j)
                if log:
                    print(" " if (pixel_value & 1) else "#", end="")
            byte_array.append(byte_value)
        if log:
            print('Image processed and saved successfully.')
            print("\n\n len(byte_array): ", len(byte_array))
        return byte_array
    else:
        print('Error loading the image.')
        return None


folder_name = "pictures_from_camera"
current_dir = os.path.dirname(os.path.abspath(__file__))
folder_name = os.path.join(current_dir, folder_name)

port = 'COM4'
baudrate = 115200
arduino = None

try:
    arduino = serial.Serial(port, baudrate)
    found_arduino = True
    print("Found Arduino")
except Exception as e:
    print(f"Serial port error: {e}")
    print('ARDUINO NOT CONNECTED')
    exit()

cap = cv2.VideoCapture(1)
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()
camera_on = False
time_per_caputure = 5
last_capture = time.time()
threshold = 70
log = time_per_caputure > 7

pygame.init()
screen = pygame.display.set_mode((1280, 480))
pygame.display.set_caption("Camera")

running = True
while(running):
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
            cap.release()
            cv2.destroyAllWindows()
            print("quitting...")
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE or event.key == K_q:
                running = False
                cap.release()
                cv2.destroyAllWindows()
                print("quitting...")
            if event.key == K_p:
                camera_on = not camera_on
                print(f"Camera is {'on' if camera_on else 'off'}")
                last_capture = time.time()
            if event.key == K_RIGHT:
                threshold += 10
                print(f"Threshold: {threshold}")
            if event.key == K_LEFT:
                threshold -= 10
                print(f"Threshold: {threshold}")
    if camera_on and time.time() - last_capture >= time_per_caputure:
        img = take_picture()
        last_capture = time.time()
        if img is None:
            print("Error: Could not take picture")
        img = cv2.flip(img, 0)
        # cv2.imshow("Picture", img)
        time_stamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        os.makedirs(folder_name, exist_ok=True)
        in_path = os.path.join(folder_name,time_stamp + ".png")
        out_path = os.path.join(folder_name,time_stamp + "_bw.png")
        cv2.imwrite(in_path, img)
        byte_list = process_and_save_image(in_path, out_path)
        # show on screen the image
        screen.fill((0, 0, 0))
        image_display = pygame.image.load(in_path)
        image_bw_display = pygame.image.load(out_path)
        image_bw_display = pygame.transform.scale(image_bw_display, (640, 480))
        screen.blit(image_display, (0, 0))
        screen.blit(image_bw_display, (640, 0))
        pygame.display.flip()
        if not log:
            # delete images
            os.remove(in_path)
            os.remove(out_path)
    elif not camera_on:
        screen.fill((0, 0, 0))
        pygame.display.flip()


cap.release()
cv2.destroyAllWindows()