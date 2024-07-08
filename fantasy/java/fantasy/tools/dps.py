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
        'Auto Attack': 'black',
        'Icy Touch': 'blue',
        'Frost Fever': 'aqua',
        'Plague Strike': 'olive',
        'Blood Plague': 'yellowgreen',
        'Blood Strike': 'red',
        'Frost Strike': 'deepskyblue',
        'Obliterate': 'gray',
    }
    return m[skill]


def plot_data(df):
    df['dt'] = pd.to_datetime(df['time'])

    ca = ColorAssignment(set(df['skill']))

    df['color'] = df['skill'].map(assign_dk_skill_color)

    event_types = df['skill'].unique()
    legend_patches = [
        mpatches.Patch(color=assign_dk_skill_color(skill), label=skill) 
        for skill in event_types
    ]

    major_df = df[~df['skill'].isin(['Frost Fever', 'Blood Plague'])]

    plt.figure(figsize=(10, 6))  # Adjust the figure size if needed

    plt.scatter(x=major_df['dt'], y=major_df['value'], c=major_df['color'])
    plt.plot(major_df['dt'], major_df['value'])

    plt.legend(
        handles=legend_patches,
        loc='upper right',
        bbox_to_anchor=(1.15, 1)
    )  # Adjust location and position

    # Calculate DPS
    total_damage = df['value'].sum()
    time_period = (df['dt'].iloc[-1] - df['dt'].iloc[0]).total_seconds()
    dps = total_damage / time_period

    # Display DPS on the plot
    plt.text(
        0.05,                  # x-coordinate (5% from the left of the plot)
        0.95,                  # y-coordinate (95% from the bottom of the plot)
        f'DPS: {dps:.2f}',     # Text to display, formatted to 2 decimal places
        transform=plt.gca().transAxes,  # Use axes coordinate system
        fontsize=12,           # Font size of the text
        verticalalignment='top',        # Align the top of the text to the y-coordinate
        bbox=dict(
            boxstyle='round',  # Rounded corners for the box
            facecolor='wheat', # Wheat-colored background for the box
            alpha=0.5          # Semi-transparent box
        )
    )

    plt.xlabel('Time')
    plt.ylabel('Damage')
    plt.title('Frost DPS')

    plt.tight_layout() # adjust layout to fit everthing nicely

    plt.show()


if __name__ == '__main__':
    #df1 = pd.read_csv('../logs/log.1719798265343')
    #df2 = pd.read_csv('../logs/log.1719801701983')
    df1 = pd.read_csv('../logs/log.1720495748666')
    plot_data(df1)
