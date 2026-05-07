### Typical toipcs
## Root Finding 
You should be fluent implementing
- Bisection
- Newton-Raphson
- Possibly Secant
You must know
- convergence speed
- when Newton fails
- stopping criteria
- tolerance vs iterations
You must instantly know:
x(n+1) = x(n) - f(x(n)) / f'(x(n))
Key talking points:
- derivative near 0 -> unstable
- bad initial guess -> divergence
- fallback to bisection

## Numerical Integration
Know how to implement
- Trapezoidal Rule
- Simpson's Rule
- Monte Carlo Integration
Understand 
- error order
- step size tradeoff

## Optimization / Minimization
Often appears as 
- find minimum of function
- calibrate paramter
Know basics
- gradient descent idea
- line search intuition
- convex vs nonconvext

## Finite Differences 
Recognize immediately:
f'(x) = (f(x+h) - f(x)) / h
f''(x) = (f(x+h) - 2f(x) + f(x-h)) / h^2
How do you numerically compute derivatives
Critical insight
- too small h -> floating point cancellation
- too big h -> truncation error

## Monte Carlo Simulation
Be comfortable
- generating random numbers
- estimating expectation
- standard error & 1/sqrt(N)
- variance reduction

### Evaluation Points
- writes working iterative code
- defines tolerance
- guards against divergence
- discusses stability
- tests edge cases

### Core Algorithms
# Implement from scratch
- Bisection
- Newton method
- Trapezoidal integration
- Finite difference derivative

### Quant-Flavored Problems
# Implied volatility solver
- root finding
- convergence
- finance relevance

# Monte Carlo expectation
- estimate pi
- estimate option payoff
- mean of random variable

# Numerical derivative + Optimization
- minimize simple function numerically