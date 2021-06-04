import numpy as np
import cv2
import os
import sys

font_ratio = 2.5

def resize_img(img, lines, columns):
    x, y, _ = img.shape
    if y*lines * font_ratio >= x*columns:
        ty = columns
        tx = int(np.floor(x*columns / y / font_ratio))
    else:
        tx = lines
        ty = int(np.floor(font_ratio * y*lines / x))
    return cv2.resize(img, (ty, tx))

def print_img(img):
    print('\033[0;0H',end='')  # 移动光标到控制台开头
    for row in img:
        for col in row:
            b,g,r = col
            print(f"\033[0;48;2;{r};{g};{b}m ",end='')
        print('\033[0m\n', end='')

def videoplay(file:str):
    columns, lines = os.get_terminal_size()
    cap = cv2.VideoCapture(file)
    success,frame = cap.read()
    i = 0
    print('\033[2J')     # 清屏
    while success:
        i = i + 1
        if i%3 == 0:     # 为了速度，删掉一部分帧数
            continue
        small = resize_img(frame, lines, columns)
        print_img(small)
        success, frame = cap.read()
    cap.release()

if __name__ == "__main__":
    videoplay(sys.argv[1])
