import os
import platform
import serial
from typing import List
from datetime import datetime
import cv2
import time
import pygame
import numpy as np
from pygame.locals import *


# display message on screen
def msg_on_screen():
    global font, screen, screen_width, screen_height, text, font_size
    y_position = screen_height // 2 - len(text) * font_size // 2
    for line in text:
        text_surface = font.render(line, True, (255, 255, 255))
        text_rect = text_surface.get_rect()
        text_rect.center = (screen_width // 2, y_position)
        screen.blit(text_surface, text_rect)
        y_position += font_size


def take_picture():
    global cap
    ret, frame = cap.read()
    if ret:
        return frame
    return None


def send_one_number(value):
    try:
        arduino.write(value)
        # arduino.flush() 
    except Exception:
        print(f"{ord(value)} not sent")
        return False
    # print(f"sent {value}")
    return True


def send_to_arduino(byte_list):
    global arduino

    arduino.flush()
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()
    if log:
        print('Sending data to Arduino!!!!!!!')
    # send START KEY ('s')
    arduino.write('s'.encode())
    response = arduino.read()
    if response is not None:
        if log:
            print(chr(ord(response)))
    else:
        print('Error: No response from Arduino')
        return False

    for i in range(len(byte_list)):
        # send the byte to the arduino
        # print(ord(byte_list[i]))
        # print(f"Sending {i+1}/{len(byte_list)}")
        if log:
            print(f"----  {bytearray([byte_list[i]]).hex()}  ----")
        if not send_one_number(bytearray([byte_list[i]])):
            print(f"Error: Could not send {ord(byte_list[i])}")
            return False
        response = arduino.readline()
        if response is not None and len(response) > 0:
            if log:
                print(response.decode(), end="")
        else:
            print('Error: No response from Arduino')
            return False
        if (i+1) % 8 == 0:
            if log:
                print()
            response = arduino.read()
            if response is not None:
                # print(chr(ord(response)))
                pass
            else:
                print('Error: No response from Arduino')
                return False
    if log:
        print('Data sent to Arduino')
    return True


def process_and_save_image(input_path, output_path):
    global time_per_caputure, log, arduin
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
        arr = []
        for i in range(0, len(flattened_array), 8):
            if i % 64 == 0 and log:
                print()
            byte_value = 0
            for j in range(8):
                pixel_value = flattened_array[i + j]
                byte_value |= (pixel_value & 1) << (7 - j)
                if log:
                    print(" " if (pixel_value & 1) else "#", end="")
            arr.append(byte_value)
        byte_array = bytearray(arr)
        if log:
            print('Image processed and saved successfully.')
            print("\n\n len(byte_array): ", len(byte_array))
        return byte_array
    else:
        print('Error loading the image.')
        return None


folder_name = "pictures_from_camera"
folder_name = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder_name)

port = 'COM4'  # change this to the port where the arduino is connected
if platform.system() == "Linux":
    port = '/dev/ttyACM0'

baudrate = 115200
save_picture = False # change this to True if you want to save the pictures taken by the camera - not recommended
arduino = None

camera_on = False
time_per_caputure = 3
last_capture = time.time()
threshold = 70 # 70 is a good value for the threshold to convert the image to black and white

try:
    arduino = serial.Serial(port, baudrate, timeout=1)
    found_arduino = True
    print("Found Arduino")
except Exception as e:
    found_arduino = False
    time_per_caputure = 0
    print(f"Serial port error: {e}")
    print('ARDUINO NOT CONNECTED')
    # exit()

cap = cv2.VideoCapture(1) # 0 is the default camera on the computer, change it to the camera you want to use
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

log = time_per_caputure > 7  # only write print statements if taking each picture takes more than 7 seconds to avoid spamming the console

screen_width, screen_height = 1280, 480
pygame.init()
screen = pygame.display.set_mode((1280, 480))
pygame.display.set_caption("Camera")

# clear Serial buffer
if found_arduino:
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()

font_size = 40
font = pygame.font.Font(None, font_size)  # Use default system font
text0 = "Press P to start/stop camera mode"
text1 = "Press RIGHT/LEFT to change threshold for black and white filter"
text2 = "Arduino not found" if not found_arduino else "Arduino found"
text = [text0, text1, text2]
str = ""
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
        # delete images if not saving, or if the time to take the picture is less than 5 seconds for not saving too many images in a short time
        if not save_picture or time_per_caputure < 5:
            os.remove(in_path)
            os.remove(out_path)
        if found_arduino and byte_list is not None:
            if log:
                print(byte_list)
                print("sending to arduino")
            send_to_arduino(byte_list)
            last_capture = time.time()

    elif not camera_on:
        screen.fill((0, 0, 0))
        msg_on_screen()
        pygame.display.flip()


cap.release()
cv2.destroyAllWindows()