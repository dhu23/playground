# given two straight line segments (represented as a start point and an end
# point), compute the point of intersection, if any

import collections

Point = collections.namedtuple("Point", "x y")
LineSeg = collections.namedtuple("LineSeg", "p1 p2")

def slope(ls):
    if ls.p1.x == ls.p2.x: return None
    return float(ls.p2.y-ls.p1.y)/float(ls.p2.x-ls.p1.x)

def intersection(ls1, ls2):
    if slope(ls1) is None:
        # check if both ends of ls2 are on different side of ls1
        ls1_x = ls1.p1.x

        if ls2.p1.x > ls1_x and ls2.p2.x > ls1_x: return None
        if ls2.p1.x < ls1_x and ls2.p2.x < ls1_x: return None

        # both points of ls2 are on different sides of ls1
        y_intersection = slope(ls2)*(ls1_x-ls2.p1.x)+ls2.p1.y

        # check if y_intersection is within ls1
        if (ls1.p1.y-y_intersection)*(ls1.p2.y-y_intersection) > 0:
            return None
        return Point(ls1_x, y_intersection)
    elif slope(ls2) is None:
        return intersection(ls2, ls1)
    else: # neither is vertical line segment
        f1 = lambda x : slope(ls1)*(x-ls1.p1.x)+ls1.p1.y
        f2 = lambda x : slope(ls2)*(x-ls2.p1.x)+ls2.p1.y

        # check ls2 points are on both sides of f1
        ls2_on_different_sides_of_ls1 = (ls2.p1.y-f1(ls2.p1.x))* \
                                        (ls2.p2.y-f1(ls2.p2.x)) <= 0.0
        ls1_on_different_sides_of_ls2 = (ls1.p1.y-f2(ls1.p1.x))* \
                                        (ls1.p2.y-f2(ls1.p2.x)) <= 0.0
        if (not ls2_on_different_sides_of_ls1 or
            not ls1_on_different_sides_of_ls2):
            return None
        print('got here', slope(ls1), slope(ls2))
        x_intersection = (float(ls1.p1.y-slope(ls1)*ls1.p1.x
                               -ls2.p1.y+slope(ls2)*ls2.p1.x) / 
                          float(slope(ls2)-slope(ls1)))
        y_intersection = f1(x_intersection)
        return Point(x_intersection, y_intersection)

if __name__ == "__main__":
    line_seg1 = LineSeg(Point(1, 2), Point(2, 3))
    print(slope(line_seg1))

    print(intersection(LineSeg(Point(0, 0), Point(1, 2)),
                       LineSeg(Point(1, 0), Point(0, 1))))
