from flask import Flask

app = Flask(__name__)


def _slow_fib(n):
    if n <= 0: return 0
    if n == 1: return 1
    
    return _slow_fib(n-1) + _slow_fib(n-2)


def _fast_fib(n):
    if n <= 0: return 0
    if n == 1: return 1

    a, b = 0, 1
    
    while n > 1:
        a, b = b, a+b
        n -= 1
    return b


@app.route('/slow/<int:n>')
def slow_fib(n):
    return str(_slow_fib(n))


@app.route('/fast/<int:n>')
def fast_fib(n):
    return str(_fast_fib(n))


@app.route('/')
def home():
    return 'fibonacci'


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5010, debug=True)
