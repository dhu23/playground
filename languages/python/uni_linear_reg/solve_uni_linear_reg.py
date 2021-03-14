#!/home/daowen/anaconda/bin/python

import math
# from itertools import izip

# theta_derivative_list is in the following format:
# [(theta_value, derivative_value)]
def do_gradient_descent(theta_derivative_list, alpha_list):

    def update_theta(theta_value, derivative_value, alpha):
        print 'using ', theta_value, derivative_value, alpha
        return theta_value - derivative_value*alpha

    if len(alpha_list) != len(theta_derivative_list):
        raise Exception("Error")

    input_list = zip(theta_derivative_list, alpha_list)

    return [update_theta(theta_derivative[0], theta_derivative[1], alpha) \
            for theta_derivative, alpha in input_list]

       
input_x_list = [2, 5, 23, 25, 2, 51, 30, 231, 45, 102, 98]
input_y_list = [3, 2, 9, 29, 0, 45, 40, 205, 29, 123, 100]

input_xy_list = zip(input_x_list, input_y_list)
print input_xy_list

def do_linear_least_square():
    y_sum = 0
    x_sum = 0
    x2_sum = 0
    xy_sum = 0
    y2_sum = 0
    num = 0
    for x, y in input_xy_list:
        y_sum += y
        x_sum += x
        x2_sum += x**2
        y2_sum += y**2
        xy_sum += x*y
        num += 1

    print 'theta0^2', num
    print 'theta1^2', x2_sum
    print 'constant', y2_sum
    print 'theta0*theta1', 2*x_sum
    print 'theta0', -2*y_sum
    print 'theta1', -2*xy_sum

    num = float(num)
    y_bar = float(y_sum)/num
    x_bar = float(x_sum)/num
    x2_bar = float(x2_sum)/num
    xy_bar = float(xy_sum)/num
    beta = (xy_bar - x_bar*y_bar)/(x2_bar-x_bar**2)
    alpha = y_bar - beta*x_bar
    print 'alpha/beta', alpha, beta

    return alpha, beta

# the contour plot would shows a suppressed ellipse along one axis

do_linear_least_square()


raise Exception('Intended')

def calculate_hypothesis_value(x, theta0, theta1):
    return theta0 + theta1*x

def calculate_hypothesis_derivative(xy_list, theta0, theta1):
    sum_value0 = 0
    sum_value1 = 0
    num = 0
    for x, y in xy_list:
        # print num
        num += 1
        diff = calculate_hypothesis_value(x, theta0, theta1)-y
        sum_value0 += diff
        sum_value1 += diff*x
    print 'sum_value0/num', sum_value0, num
    return sum_value0/num, sum_value1/num

theta0_init = 121
theta1_init = -912

learning_rate0 = 0.1 # alpha 0
learning_rate1 = 0.1 # alpha 1
c = 0

# calculate initial derivatives for theta0 and theta1
d0_init, d1_init = calculate_hypothesis_derivative(
        input_xy_list, theta0_init, theta1_init)

print 'initial derivatives:', d0_init, d1_init

theta0_start = theta0_init
theta1_start = theta1_init

d0_start, d1_start = d0_init, d1_init

def verify_thetas(theta0_old, theta0_new, theta1_old, theta1_new, threshold):
    def verify(old, new, threshold):
        return (new-old*(1+threshold))*(new-old*(1-threshold)) < 0

    return verify(theta0_old, theta0_new, threshold) and \
            verify(theta1_old, theta1_new, threshold)

cc = 0
while c < 20:

    # attempt gradient descent. evaluate the result and see if 
    # learning rate is proper
    cc += 1
    theta0_new, theta1_new = do_gradient_descent(
            [(theta0_start, d0_start), (theta1_start, d1_start)],
            (learning_rate0, learning_rate1))
    print 'theta0_new, theta1_new', theta0_new, theta1_new
    d0_new, d1_new = calculate_hypothesis_derivative(
            input_xy_list, theta0_new, theta1_new)
    
    if math.fabs(d1_new) > math.fabs(d1_start):
        print 'Overshooting theta1 %f(%f), %f(%f)' % (
                theta1_new, theta1_start, d1_new, d1_start)
        learning_rate1 /= 2
        print 'learning rate1 updated:', learning_rate1
        continue 
    if math.fabs(d0_new) > math.fabs(d0_start):
        print 'Overshooting theta0 %f(%f), %f(%f)' % (
                theta0_new, theta0_start, d0_new, d0_start)
        learning_rate0 /= 2
        print 'learning rate0 updated:', learning_rate0
        continue

    
    # learning rate check is passed

    c += 1
    print 'descent=%d, %.8f(%.8f), %f(%f), d0(%f), d1(%f)' % (
            c, theta0_new, theta0_start, theta1_new, theta1_start,
            d0_new, d1_new)

    #if verify_thetas(theta0_start, theta0_new, theta1_start, theta1_new, 0.01):
    #    print 'FOUND: theta0', theta0_new, 'theta1', theta1_new
    #    # break

    theta0_start, theta1_start = theta0_new, theta1_new
    d0_start, d1_start = d0_new, d1_new
