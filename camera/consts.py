import platform

camera_index = 0 # 0 is the default camera on the computer, change it to the camera you want to use

threshold = 60 # 60 is a good value for the threshold to convert the image to black and white
empty_image_threshold = 0.1 # if the percentage of black pixels in the image is less than this value, the image is considered empty
empty_captures_before_idle = 3  # if this many empty images are taken in a row, the camera will go to idle mode

output_height = 25 # the height of the image that will be sent to the arduino (max value is 40 - DO NOT OVERPASS THIS VALUE)
output_width = 64 # DO NOT CHANGE - the width of the image that will be sent to the arduino (the arduino can only receive images with a width of 64 pixels - DO NOT CHANGE)
valve_on_time = 5 # time in milliseconds the valve will be open when the arduino receives the 'd' key
space_time = 250 # time in milliseconds the arduino will wait after receiving the 's' key before starting the process of dropping the image
full_light = True # if True, the lights will be on all the time. If False, the lights will be on only after 'led_start_time' milliseconds and for 'led_on_time' milliseconds
led_start_time = 350 # time in milliseconds the arduino will wait after dropping the first row before turning on the lights
led_on_time = 1000 # time in milliseconds the lights will be on after 'led_start_time' milliseconds
drawing_depth = 1 # the amount of layers each picture will be (1 is the default value)
led_power = 255 # the power of the lights (0-255) - currently not used
colors = [[255,0,255],[255,255,0],[255,215,215]] # the colors of the lights, max 8 colors (each color is 3 bytes - RGB)  - currently not used
colors_num = len(colors) # the number of colors of the lights  - currently not used
cassettes_num = 10 # the number of cassettes available in the exhibit

port = 'COM3'  # change this to the port where the arduino is connected
if platform.system() == "Linux":  # if the system is Linux, the port should be '/dev/ttyUSB0' or '/dev/ttyACM0'
    port = '/dev/ttyUSB0'

baudrate = 115200  # the baudrate of the serial communication with the arduino
save_picture = False # change this to True if you want to save the pictures taken by the camera - not recommended for long periods of time.
log = (True) and space_time > 5000  # if True, the program will print logs to the console. if the time between sending pictures is less than 5 seconds, no logs will be printed to avoid spamming the console
folder_name_for_saved_pictures = "pictures_from_camera"  # if save_picture is True, the pictures will be saved in this folder
folder_name_for_idle_pictures = "images_for_idle"  # this folder should be in the same folder as this file 'main.py'

idle_images = ["hand0.png", "hand1.png", "hand2.png", "hand3.png", "hand4.png", "hand5.png", "hand6.png", "hand7.png", "hand8.png"]
# these images will be used if the picture taken from camera is 'empty' (i.e. no object is detected on the picture).
# The images should be in the folder 'images_for_idle' and their paths should be relative to the folder 'images_for_idle'

drop_key = 'd'  # the key to drop (in water) the image in the arduino's buffer
start_key = 's'  # the key to send a new image to the arduino's buffer and start the process of dropping it
end_key = 'e'  # the key to stop the arduino's process of collecting data from computer and dropping the image
check_key = 'g'  # the key to check the arduino's response

crop_percentage = [80,98]  # the percentage of the image that WILL STAY after cropping the sides (the first value is the percentage of the width, the second value is the percentage of the height)
# adjust these values if the image is not being cropped correctly. 100% means no cropping, 0% means the image will be cropped completely