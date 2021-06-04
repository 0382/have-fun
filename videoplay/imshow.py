import numpy as np
import cv2
import os
import sys

font_ratio = 2.5

def get_img(filename, lines, columns):
    img = cv2.imread(filename)
    x, y, _ = img.shape
    if y*lines * font_ratio >= x*columns:
        ty = columns
        tx = int(np.floor(x*columns / y / font_ratio))
    else:
        tx = lines
        ty = int(np.floor(font_ratio * y*lines / x))
    return cv2.resize(img, (ty, tx))

def print_img(img):
    for row in img:
        for col in row:
            b,g,r = col
            print(f"\033[0;48;2;{r};{g};{b}m ", end='')
        print('\033[0m\n',end='')

def imshow(file:str):
    columns, lines = os.get_terminal_size()
    img = get_img(file, lines, columns)
    print_img(img)

if __name__ == "__main__":
    imshow(sys.argv[1])
