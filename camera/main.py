import os
import serial
from typing import List
from datetime import datetime
os.environ["OPENCV_LOG_LEVEL"]="SILENT" # to suppress OpenCV warnings and errors
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide" # to suppress Pygame welcome message
import cv2
import time
import pygame
import numpy as np
from pygame.locals import *
from consts import *
import random


# display message on screen
def msg_on_screen():
    global font, screen, screen_width, screen_height, font_size, found_arduino, camera_working
    text0 = "Press P to start/stop camera mode"
    text1 = "Press RIGHT/LEFT to change threshold for black and white filter"
    text2 = "Arduino not found" if not found_arduino else "Arduino found"
    text1_5 = "----------------------------------------------------------------------------"
    text3 = "Camera is connected" if camera_working else "Camera is NOT connected - reconnect the camera and press P to start"
    text = [text0, text1, text1_5, text2 ,text3]
    y_position = screen_height // 2 - len(text) * font_size // 2
    for line in text:
        text_surface = font.render(line, True, (255, 255, 255))
        text_rect = text_surface.get_rect()
        text_rect.center = (screen_width // 2, y_position)
        screen.blit(text_surface, text_rect)
        y_position += font_size


def take_picture():
    global cap, camera_working
    if not camera_working:
        return None
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


def send_drop_key():
    global arduino
    arduino.write(drop_key.encode())

def send_start_key():
    global arduino
    arduino.write(start_key.encode())

def send_end_key():
    global arduino
    arduino.write(end_key.encode())

def send_variables_to_arduino():
    global arduino, valve_on_time, space_time, full_light, led_start_time, led_on_time, drawing_depth, led_power, colors, colors_num
    arduino.write(bytearray([output_height]))
    arduino.write(bytearray([valve_on_time]))
    arduino.write(bytearray([drawing_depth]))
    # arduino.write(bytearray([full_light]))
    # arduino.write(bytearray([led_power]))
    # arduino.write(bytearray([colors_num]))
    # for color in colors:
    #     for value in color:
    #         arduino.write(bytearray([value]))
    # response = arduino.read()
    # if response is not None:
    #     # print(chr(ord(response)))
    #     pass
    # else:
    #     print('Error: No response from Arduino')
    #     return False
    return True


def send_to_arduino(byte_list):
    global arduino
    if log:
        print('Sending data to Arduino...')

    for i in range(len(byte_list)):
        if log:
            print(f"----  {bytearray([byte_list[i]]).hex()}  ----")
        if not send_one_number(bytearray([byte_list[i]])):
            print(f"Error: Could not send {ord(byte_list[i])}")
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
    global time_per_capture, log
    image = cv2.imread(input_path)
    if image is not None:
        resized_image = cv2.resize(image, (output_width, output_height))
        gray_image = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
        # Apply binary thresholding to create a black and white image
        _, bw_image = cv2.threshold(gray_image, threshold, 255, cv2.THRESH_BINARY)
        cv2.imwrite(output_path, bw_image)
        flattened_array = bw_image.flatten()
        # if log:
        #     print("len(flattened_array): ", len(flattened_array))
        arr = []
        black_pixels = 0
        for i in range(0, len(flattened_array), 8):
            if i % 64 == 0 and log:
                print()
            byte_value = 0
            for j in range(8):
                pixel_value = flattened_array[i + j]
                byte_value |= (pixel_value & 1) << (7 - j)
                if pixel_value == 0:
                    black_pixels += 1
                if log:
                    print(" " if (pixel_value & 1) else "#", end="")
            arr.append(byte_value)
        byte_array = bytearray(arr)
        if log:
            print('\n\nImage processed and saved successfully.')
            # print("\n\n len(byte_array): ", len(byte_array))
        return byte_array, black_pixels / len(flattened_array)
    else:
        print('Error loading the image.')
        return None


folder_name = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder_name_for_saved_pictures)
idle_folder_name = os.path.join(os.path.dirname(os.path.abspath(__file__)), folder_name_for_idle_pictures)
arduino = None
camera_on = True
camera_working = False
time_per_capture = time_per_capture_default
time_per_capture_idle = 1.5
empty_captures_in_a_row = 0
last_capture = time.time()

try:
    arduino = serial.Serial(port, baudrate, timeout=1)
    found_arduino = True
    print("Found Arduino")
except Exception as e:
    found_arduino = False
    time_per_capture = 0
    log = time_per_capture > 7
    print(f"Serial port error: {e}")
    print('ARDUINO NOT CONNECTED')
    # exit()

cap = None
while True:
    try:
        cap = cv2.VideoCapture(camera_index)
        cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)  # Disable automatic exposure
        cap.set(cv2.CAP_PROP_AUTO_WB, 0)  # Disable automatic white balance
        cap.set(cv2.CAP_PROP_EXPOSURE, -6) 
        if cap.isOpened():
            print("Camera is ready")
            camera_working = True
            break
        print("\nError: Could not open camera. please check if the camera is connected properly.\nRetrying in 2 seconds...\n\n")
        time.sleep(2)
    except Exception as e:
        pass


screen_width, screen_height = 720, 640
pygame.init()
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Camera")

# clear Serial buffer
if found_arduino:
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()
    time.sleep(0.1) # wait for the arduino to process the variables and be ready to receive the image, at least 1 second
    

font_size = 40
font = pygame.font.Font(None, font_size)  # Use default system font
is_folder_created = os.path.exists(folder_name)
sample_index = 0
arduino_done = True
idle = False
send_parameters = False
running = True
while(running):
    
    img = take_picture()  # take a picture every 0.002 seconds to keep the buffer clean with the latest image and avoid delay in the images. Do not remove for Raspberry Pi
    time.sleep(0.002)
    
    if not send_parameters and found_arduino:
        send_variables_to_arduino()
        send_parameters = True
        time.sleep(0.2) # wait for the arduino to process the variables and be ready to receive the image, at least 0.1 second

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
                if not camera_working:
                    try:
                        cap = cv2.VideoCapture(camera_index)
                        if cap.isOpened():
                            camera_working = True
                            print("Camera is ready")
                    except Exception as e:
                        print("Error: Could not open camera. please check if the camera is connected properly")
                        continue
                camera_on = not camera_on
                print(f"Camera is {'on' if camera_on else 'off'}")
                last_capture = time.time()
            if event.key == K_RIGHT:
                threshold += 10
                print(f"Threshold: {threshold}")
            if event.key == K_LEFT:
                threshold -= 10
                print(f"Threshold: {threshold}")

    if camera_on and time.time() - last_capture >= space_time/1000 and arduino_done:
        img = take_picture()
        if img is None:
            print("Error: Could not take picture")
            camera_on = False
            camera_working = False
            continue
        else:
            camera_working = True
        img = cv2.flip(img, 0)

        # img = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)
        # squish the image on the width axis and add white padding in the sides
        # img.resize(360-100, 640)
        # img = cv2.copyMakeBorder(img, 0, 0, 70, 70, cv2.BORDER_CONSTANT, value=[255, 255, 255])

        time_stamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        if not is_folder_created:
            os.makedirs(folder_name, exist_ok=True)
        in_path = os.path.join(folder_name,time_stamp + ".png")
        out_path = os.path.join(folder_name,time_stamp + "_bw.png")
        cv2.imwrite(in_path, img)
        byte_list, black_percentage = process_and_save_image(in_path, out_path)
        # show on screen the image
        screen.fill((0, 0, 0))
        image_display = pygame.image.load(in_path)
        image_bw_display = pygame.image.load(out_path)
        image_bw_display = pygame.transform.scale(image_bw_display, (screen_width//2, screen_width//2 * output_height // output_width))
        image_display = pygame.transform.scale(image_display, (screen_width//2, screen_height))
        screen.blit(image_display, (0, 0))
        screen.blit(image_bw_display, (screen_width//2, 0))
        pygame.display.flip()

        if found_arduino and byte_list is not None and black_percentage > empty_image_threshold:
            empty_captures_in_a_row = 0
            idle = False
            if log:
                # print(byte_list)
                print("sending to arduino")
            arduino.flush()
            arduino.reset_input_buffer()
            arduino.reset_output_buffer()

            cassette = random.randint(0, cassettes_num-1)
            send_start_key()
            if not send_one_number(bytearray([cassette])):
                print("Error: Could not send cassette number")
                continue
            if not send_to_arduino(byte_list):
                print("Error: Could not send image to arduino")
                continue
            send_end_key()
            last_capture = time.time()
            arduino_done = False
        elif found_arduino and byte_list is not None:  # black_percentage <= empty_image_threshold - 'empty' image
            empty_captures_in_a_row += 1
            if empty_captures_in_a_row >= empty_captures_before_idle:
                if log:
                    print("Image is empty, sending sample image to arduino...")
                # time_per_capture = time_per_capture_idle
                idle = True
                byte_list, _ = process_and_save_image(os.path.join(idle_folder_name, idle_images[sample_index]), out_path)
                if byte_list is not None:
                    arduino.flush()
                    arduino.reset_input_buffer()
                    arduino.reset_output_buffer()
                    cassette = random.randint(0, cassettes_num-1)
                    send_start_key()
                    if not send_one_number(bytearray([cassette])):
                        print("Error: Could not send cassette number")
                        continue
                    if not send_to_arduino(byte_list):
                        print("Error: Could not send image to arduino")
                        continue
                    send_end_key()
                    sample_index += 1
                    if sample_index >= len(idle_images):
                        sample_index = 0
            else:
                send_drop_key() # drop the image in the arduino's buffer
            arduino_done = False
            last_capture = time.time()
        elif found_arduino and byte_list is None:
            print("Error processing image")

        # delete images if not saving, or if the time to take the picture is less than 2 seconds for not saving too many images in a short time
        if not save_picture or space_time/1000 <= 2:
            try:
                os.remove(in_path)
                os.remove(out_path)
            except Exception as e:
                print(f"Error deleting images: {e}")
    
    elif found_arduino and not arduino_done:
        if arduino.in_waiting > 0:
            received_data = arduino.readline().decode().rstrip()
            # print("Received from Arduino:", received_data)
            arduino_done = True
    elif not camera_on:
        screen.fill((0, 0, 0))
        msg_on_screen()
        pygame.display.flip()

if found_arduino:
    arduino.close()
if not is_folder_created and not save_picture:  # if the folder was created in this execution, delete it. If it was already there, keep it
    try: 
        os.rmdir(folder_name)
    except Exception as e:
        print(f"Error deleting folder: {e}")
cap.release()
cv2.destroyAllWindows()