import turtle


def sun(c1, c2):
    turtle.color(c1, c2)
    turtle.begin_fill()

    while True: 
        turtle.forward(600)
        turtle.left(130)
        if abs(turtle.pos()) < 1:
            break

    turtle.end_fill()


sun('red', 'yellow')
#turtle.left(180)
#sun('magenta', 'green')

turtle.done()
