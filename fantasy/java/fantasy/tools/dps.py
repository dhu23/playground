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



def calculate_dps(df):
    total_damage = df['value'].sum()
    time_period = (df['dt'].iloc[-1] - df['dt'].iloc[0]).total_seconds()
    return total_damage / time_period


def short(df):
    return df[['dt', 'skill', 'value', 'critical', 'dps']]


def plot_data(df):
    df['dt'] = pd.to_datetime(df['time'])

    df['time_diff'] = df['dt'] - df['dt'].iloc[0]
    df['time_diff_secs'] = df['time_diff'].dt.total_seconds()
    df['total_value'] = df['value'].cumsum()

    df['time_diff_secs'] = df['time_diff_secs'].replace(0, pd.NA)
    df['dps'] = df['total_value'] / df['time_diff_secs']

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
    dps = calculate_dps(df)

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


def read_data(path):
    return pd.read_csv(path)


def analyze_data(df):
    group_by_cols = ['skill', 'critical']
    total_value_group = df.groupby(group_by_cols)['value'].sum().unstack(fill_value=0)
    total_value = total_value_group.sum().sum()
    # this way it returns a series, if done with [['value']], a dateframe
    avg_value_group = df.groupby(group_by_cols)['value'].mean().unstack(fill_value=0.0)
    hits_group = df.groupby(group_by_cols)['value'].count().unstack(fill_value=0)
    total_hits = hits_group.sum(axis=1)
    return pd.DataFrame({
        #'total crit': total_value_group['Y'],
        #'total non-crit': total_value_group['N'],
        'total': total_value_group.sum(axis=1),
        'avg crit': avg_value_group['Y'],
        'avg non-crit': avg_value_group['N'],
        '(%) of total': total_value_group.sum(axis=1) / total_value * 100.0,
        'crit (%)': hits_group['Y'] / total_hits * 100.0,
    })


if __name__ == '__main__':
    pass
