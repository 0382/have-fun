import cv2
import click
import time

def print_img(img):
    print('\033[0;0H',end='')  # 移动光标到控制台开头
    for row in img:
        for col in row:
            b,g,r = col
            print(f"\033[0;48;2;{r};{g};{b}m ",end='')
        print('\033[0m\n', end='')

@click.command()
@click.option("--file", type=str, required=True, help='video file name')
@click.option("--lines", type=int, default=27, help='console lines')
@click.option("--columns", type=int, default=120, help='console columns')
def _vdplay(file:str, lines:int, columns:int):
    cap = cv2.VideoCapture(file)
    success,frame = cap.read()
    i = 0
    print('\033[2J')     # 清屏
    while success:
        i = i + 1
        if i%3 == 0:     # 为了速度，删掉一部分帧数
            continue
        small = cv2.resize(frame, (columns, lines))
        print_img(small)
        success, frame = cap.read()
    cap.release()

if __name__ == "__main__":
    _vdplay()
