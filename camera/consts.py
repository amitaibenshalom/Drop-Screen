import platform

time_per_capture_default = 3  # time in seconds between each picture taken by the camera
threshold = 70 # 70 is a good value for the threshold to convert the image to black and white
empty_image_threshold = 0.1 # if the percentage of black pixels in the image is less than this value, the image is considered empty

empty_captures_before_idle = 3  # if this many empty images are taken in a row, the camera will display an idle image

port = 'COM4'  # change this to the port where the arduino is connected
if platform.system() == "Linux":
    port = '/dev/ttyUSB0'

baudrate = 115200
save_picture = False # change this to True if you want to save the pictures taken by the camera - not recommended for long periods of time

log = time_per_capture_default > 7  # only write print statements if taking each picture takes more than 7 seconds to avoid spamming the console
folder_name_for_saved_pictures = "pictures_from_camera2"  # if save_picture is True, the pictures will be saved in this folder
folder_name_for_idle_pictures = "images_for_idle"  # this folder should be in the same folder as this file 'main.py'

idle_images = ["hand0.png", "hand1.png", "hand2.png", "hand3.png", "hand4.png", "hand5.png", "hand6.png", "hand7.png", "hand8.png"]
# these images will be used if the picture taken from camera is 'empty' (i.e. no object is detected on the picture).
# The images should be in the folder 'images_for_idle' and their paths should be relative to the folder 'images_for_idle'
