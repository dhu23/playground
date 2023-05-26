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
    dir_path = '/home/daowen/github/playground/languages/java/yield-cpu/test2'
    data = dict(
        (name, load_file(f'{dir_path}/{name}.txt'))
        for name in ['yielding', 'timeout-polling', 'polling'])

    for k, v in data.items():
        print(f'{k}, len={len(v)}')

    df = pd.DataFrame.from_dict(data=data)

    df1 = df[['yielding', 'timeout-polling']]
    df2 = df[['yielding', 'polling']]
    df3 = df[['timeout-polling', 'polling']]

    plt.yscale('log')

    plt.plot(df.index, df['yielding'], label='yielding')
    plt.plot(df.index, df['timeout-polling'], label='timeout-polling')
    plt.plot(df.index, df['polling'], label='polling')

    plt.legend()
    plt.show()
