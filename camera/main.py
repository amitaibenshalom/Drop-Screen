import os
from typing import List
from PIL import Image
from datetime import datetime
import cv2
import keyboard

# a function that takes an image from a file and makes it black and white (no grayscale), for example, by setting all pixels with a brightness greater than 128 to white and all pixels with a brightness less than or equal to 128 to black. The function should return the modified image as a list of bytes. The function should have the following signature: def black_and_white(image_path: str) -> List[int]: where image_path is the path to the image file and List is the list type from the typing module. The function should use the PIL library to work with images. The function should return None if the image cannot be opened or if the image cannot be converted to black and white. The function should not modify the original image file.
def image_to_water(image_path: str) -> List[int]:
    try:
        with Image.open(image_path) as img:
            img = img.convert("L")
            img = img.point(lambda p: p > 128 and 255)
            
            # reduce the size of the image to 64x20
            img = img.resize((64, 20))

            img_bytes = img.tobytes()
            byte_list = list(img_bytes)
            return byte_list
    except Exception as e:
        print(f"Error: {e}")
        return None
    

def take_picture():
    global cap
    ret, frame = cap.read()
    cap.release()
    cv2.destroyAllWindows()
    if ret:
        return frame
    return None


def process_and_save_image(input_path, output_path):
    # Read the image from the given path
    image = cv2.imread(input_path)

    # Check if the image was successfully loaded
    if image is not None:
        # Convert the image to grayscale
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        # Apply binary thresholding to create a black and white image
        _, bw_image = cv2.threshold(gray_image, 100, 255, cv2.THRESH_BINARY)

        # Resize the image to 64x20 pixels
        resized_image = cv2.resize(bw_image, (64, 20))

        # Save the processed image
        cv2.imwrite(output_path, resized_image)
        print('Image processed and saved successfully.')
    else:
        print('Error loading the image.')

# Replace 'input_image.jpg' with the path to your input image file
# Replace 'output_image.jpg' with the desired filename for the processed image
in_path = 'D:\Amitai_D\museum\drop_screen\Drop-Screen\camera\pictures_from_camera\hand.jpg'
out_path = 'D:\Amitai_D\museum\drop_screen\Drop-Screen\camera\pictures_from_camera\hand_bw.jpg'
process_and_save_image(in_path, out_path)





# cap = cv2.VideoCapture(0)
running = False
while(running):
    if keyboard.is_pressed("q"):
        running = False
    elif keyboard.is_pressed("p"):
        img = take_picture()
        if img is None:
            print("Error: Could not take picture")
            continue
        cv2.imshow("Picture", img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        time_stamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        os.makedirs("pictures_from_camera", exist_ok=True)
        path = os.path.join("pictures_from_camera",time_stamp + ".png")
        cv2.imwrite(path, img)
        byte_list = image_to_water(path)
        if byte_list:
            print(f"Byte list representing the image: {byte_list}")


