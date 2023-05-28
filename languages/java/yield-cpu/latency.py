import pandas as pd
import matplotlib.pyplot as plt


def load_file(file_path):
    with open(file_path, 'r') as f:
        data = str(f.read().strip())
        assert data[0] == '['
        assert data[-1] == ']'

        latencies = []
        for s in data[1:-1].split(','):
            latencies.append(int(s))

        return latencies


if __name__ == '__main__':
    dir_path = '/home/daowen/github/playground/languages/java/yield-cpu/test3'
    labels = [
        'yielding', 'timeout-polling', 'polling', 'smart-polling',
        'timeout-polling-100', 'smart-polling-100',
        'timeout-polling-500', 'smart-polling-500', 'smart-taking',
    ]
    data = dict((name, load_file(f'{dir_path}/{name}.txt')) for name in labels)

    for k, v in data.items():
        print(f'{k}, len={len(v)}')

    df = pd.DataFrame.from_dict(data=data)

    plt.yscale('log')

    for label in ['smart-polling', 'smart-taking', 'polling']:
        plt.plot(df.index, df[label], label=label)

    plt.legend()
    plt.show()
