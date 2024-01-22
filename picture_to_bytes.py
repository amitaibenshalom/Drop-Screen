from PIL import Image

def image_to_byte_list(image_path):
    try:
        # Open the image file
        with Image.open(image_path) as img:
            # Convert the image to raw bytes
            img_bytes = img.tobytes()
            byte_list = list(img_bytes)

            return byte_list
    except Exception as e:
        print(f"Error: {e}")
        return None

image_path = r"pictures/heart_4.png"
byte_list = image_to_byte_list(image_path)

if byte_list:
    print(f"Byte list representing the image: {byte_list}")
