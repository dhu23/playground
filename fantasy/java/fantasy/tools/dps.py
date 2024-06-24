import matplotlib
matplotlib.use('TkAgg')

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

class ColorAssignment(object):
    def __init__(self, targets):
        assert len(targets) < 9

        self._mapping = dict()
        for t, c in zip(sorted(targets), ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']):
            self._mapping[t] = c

    def get_color(self, target):
        return self._mapping[target]


def assign_dk_skill_color(skill):
    m = {
        'Icy Touch': 'aqua',
        'Plague Strike': 'green',
        'Blood Strike': 'red',
        'Frost Strike': 'blue',
        'Obliterate': 'gray',
    }
    return m[skill]


def plot_data(df):
    ca = ColorAssignment(set(df['skill']))

    df['color'] = df['skill'].map(assign_dk_skill_color)

    event_types = df['skill'].unique()
    legend_patches = [
        mpatches.Patch(color=assign_dk_skill_color(skill), label=skill) 
        for skill in event_types
    ]

    plt.figure(figsize=(10, 6))  # Adjust the figure size if needed

    plt.scatter(x=df['dt'], y=df['value'], c=df['color'])
    plt.plot(df['dt'], df['value'])

    plt.legend(
        handles=legend_patches, 
        loc='upper right', 
        bbox_to_anchor=(1.15, 1)
    )  # Adjust location and position

    plt.xlabel('Time')
    plt.ylabel('Damage')
    plt.title('Frost DPS')

    plt.tight_layout() # adjust layout to fit everthing nicely

    plt.show()


if __name__ == '__main__':
    df = pd.read_csv('../logs/log.1719243148019')
    df['dt'] = pd.to_datetime(df['time'])

    plot_data(df)
