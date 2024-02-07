import os
from typing import List
# from PIL import Image
from datetime import datetime
import cv2
import time
import pygame
from pygame.locals import *

def img_to_byte(img_path):
    with open(img_path, "rb") as img_file:
        byte_list = img_file.read()
    return byte_list
    
def take_picture():
    global cap
    ret, frame = cap.read()
    if ret:
        return frame
    return None


def process_and_save_image(input_path, output_path):
    global time_per_caputure, trheshold
    image = cv2.imread(input_path)
    if image is not None:
        resized_image = cv2.resize(image, (64, 20))
        gray_image = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
        # Apply binary thresholding to create a black and white image
        _, bw_image = cv2.threshold(gray_image, trheshold, 255, cv2.THRESH_BINARY)
        cv2.imwrite(output_path, bw_image)
        if time_per_caputure >= 1:
            print('Image processed and saved successfully.')
        return img_to_byte(output_path)
    else:
        print('Error loading the image.')
        return None


folder_name = "pictures_from_camera"
current_dir = os.path.dirname(os.path.abspath(__file__))
folder_name = os.path.join(current_dir, folder_name)

cap = cv2.VideoCapture(1)
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()
camera_on = False
time_per_caputure = 4
last_capture = time.time()
trheshold = 70

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
                trheshold += 10
                print(f"Threshold: {trheshold}")
            if event.key == K_LEFT:
                trheshold -= 10
                print(f"Threshold: {trheshold}")
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
        if byte_list:
            # print(f"Byte list representing the image: {byte_list}")
            pass
        # show on screen the image
        screen.fill((0, 0, 0))
        image_display = pygame.image.load(in_path)
        image_bw_display = pygame.image.load(out_path)
        image_bw_display = pygame.transform.scale(image_bw_display, (640, 480))
        screen.blit(image_display, (0, 0))
        screen.blit(image_bw_display, (640, 0))
        pygame.display.flip()
        if time_per_caputure < 1:
            # delete images
            os.remove(in_path)
            os.remove(out_path)
    elif not camera_on:
        screen.fill((0, 0, 0))
        pygame.display.flip()


cap.release()
cv2.destroyAllWindows()