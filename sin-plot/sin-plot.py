import taichi as ti

ti.init(arch=ti.gpu)

N = 1000
omega = 0.1

img = ti.field(dtype=ti.f32, shape=(N, N))

@ti.func
def g(x, y, t, e, l):
    x = x - N//2
    y = y - N//2
    r = ti.sqrt(float(x*x+y*y))
    return ti.sin(r**e/l + omega*t)

@ti.kernel
def paint(t: ti.f32, e: ti.f32, l: ti.f32):
    for i, j in img:
        img[i, j] = g(i, j, t, e, l)

gui = ti.GUI("sin-plot", (N, N))


for i in range(1000):
    e = i * 0.002 + 1.0
    l = 500**e / (1000 * 3.14)
    paint(i, e, l)
    gui.set_image(img)
    gui.show()