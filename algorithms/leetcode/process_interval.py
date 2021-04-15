# given internal data that look like the following, which 
# describes the start time and end time of a process
# 1: (12.23, 15.12)
# 2: (14.10, 23.64)
# 3: (21.12, 22.22)
# build an algorithm to study usage cores of the system at a given time

# data simulation:
# start N processes sequentially, with 1 sec apart, 
# randomly assign process time, and at the end of the process, start 
# another process to replace it

import random
import datetime
import collections

Interval = collections.namedtuple('Interval', ['start', 'end'])

# proc_costs is a list of proc time
def simulate_proceses(start_time, wait_time, proc_costs):
    ret = []
    for cost in proc_costs:
        st = ret[-1].end + wait_time if ret else start_time
        ret.append(Interval(st, st + cost))
    return ret

def simulate_data(start_time, num_core, proc_per_core, start_wait, f_gen):
    proc_costs_list = (f_gen(num_proc) for i in range(num_proc))

    ret = []
    for i in range(num_core):
        ret.extend(
            simulate_process(
                start_time + datetime.timedelta(seconds=i*seq_start_wait),
                start_wait, 
                f_gen(proc_per_core)))
    return ret
    


if __name__ == "__main__":
    pass
