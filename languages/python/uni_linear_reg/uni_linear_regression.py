#!/home/dhu/anaconda/bin/python
import math
import datetime

class GradientDescentSolver:

    def __init__(self, init_var, alpha, derivative_func):
        
        self.var = init_var
        self.alpha = alpha
        self.derivative_func = derivative_func
    
    def do_gradient_descent(self):
        while True:
            derivative = self.derivative_func(self.var)
            var = self.var - self.alpha*derivative

            derivative_new = self.derivative_func(var)
            print "y=%f, y_new=%f, x0=%f, x1=%f, alpha=%f" % (
                    derivative, derivative_new, self.var, var, self.alpha)

            if math.fabs(derivative_new) > math.fabs(derivative):
                self.alpha *= 0.9
                continue

            self.var = var
            return derivative_new, self.var

class GradientDescentSolver2:

    def __init__(self, init_point, alpha, derivative_func, func):

        self.point = init_point
        self.alpha = alpha
        self.derivative_func = derivative_func
        self.func = func

    def do_gradient_descent(self):
        c = 0
        while True: # c < 200:
            func_value = self.func(*self.point)
            derivatives = self.derivative_func(*self.point)
            point_new = []
            for i, point in enumerate(self.point):
                 p = point - self.alpha*derivatives[i]
                 point_new.append(p)
            func_value_new = self.func(*point_new)
            
            print "%d, y_new=%s, y=%s, p_new=%s, p=%s, alpha=%s" % (
                    c, func_value_new, func_value, point_new, self.point, 
                    self.alpha)

            c += 1
            if func_value_new > func_value:
                self.alpha *= 0.9
                continue
            
            self.point = point_new
            derivatives_new = self.derivative_func(*self.point)
            return derivatives_new, self.point

def solve_one_variable():
    # example funciton y = 100*x*x-200*x, y' = 200*x-200
    gds = GradientDescentSolver(20, 1, lambda x: 200*x-200)
    while True:
        derivative, variable = gds.do_gradient_descent()
        if math.fabs(derivative) < 0.005:
            print 'FOUND result: variable=%f' % variable
            break

def solve_two_variable():
    # example function y = 11*x*x+80082*z*z+1228*x*z-1170*x-150898*z+72555
    # partial dy/dx = 22*x+1228*z-1170
    # partial dy/dz = 160164*z+1228*x-150898
    gds = GradientDescentSolver2(
            [-210, 212], 0.1, 
            lambda x, z: [22*x+1228*z-1170, 160164*z+1228*x-150898],
            lambda x, z: 11*x*x+80082*z*z+1228*x*z-1170*x-150898*z+72555)
    c = 0
    while True: # c < 36000:
        derivatives, point = gds.do_gradient_descent()
        print '%d,d=%s,p=%s' % (c, derivatives, point)
        c += 1
        if math.fabs(derivatives[0]) < 0.01 and \
                math.fabs(derivatives[1]) < 0.01:
            print "FOUND result: point=%s" % point
            break

if __name__ == "__main__":
    start = datetime.datetime.now()
    solve_two_variable()
    end = datetime.datetime.now()
    print 'cost', end-start
