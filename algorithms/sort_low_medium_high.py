#!/usr/bin/python

# data from 1-99
dataList = [5, 23, 16, 80, 78, 4, 18, 45, 84, 32, 9, 34, 70, 59, 10]

def get_priority(num):
    if 1 <= num <= 33:
        return 'L'
    elif 34 <= num <= 66:
        return 'M'
    elif 67 <= num <= 99:
        return 'H'

def partition_data(data):
    head, tail = 0, len(data)-1
    for i, each in enumerate(data):
        pass


def print_data_priority(data):
    print [get_priority(each) for each in data]

if __name__ == "__main__":
    print_data_priority(dataList)
