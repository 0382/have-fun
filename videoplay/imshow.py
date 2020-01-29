import numpy as np
import cv2
import click

def get_img(filename, _lines, _columns):
    img = cv2.imread(filename)
    return cv2.resize(img, (_columns, _lines))

def print_img(img):
    for row in img:
        for col in row:
            b,g,r = col
            print(f"\033[0;48;2;{r};{g};{b}m ", end='')
        print('\033[0m\n',end='')

@click.command()
@click.option("--file", type=str, required=True, help='image file name')
@click.option("--lines", type=int,default=27, help='console lines')
@click.option("--columns", type=int, default=120, help='console columns')
def _imshow(file:str, lines:int, columns:int):
    img = get_img(file, lines, columns)
    print_img(img)

if __name__ == "__main__":
    _imshow()
