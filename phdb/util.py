import datetime
import os

MM_DD_YYYY = "%m/%d/%Y"
YYYYMMDD = "%Y%m%d"


def to_mmddyyyy(d):
    return datetime.datetime.strftime(d, MM_DD_YYYY)


def to_yyyymmdd(d):
    return datetime.datetime.strftime(d, YYYYMMDD)


def mk_dir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)
    return dir_name


def is_increasing(l):  # strictly monotonically increasing
    if len(l) > 1:
        return all(l[i] < l[i+1] for i in range(len(l)-1))
    else:
        return True


# sample asof from the source
# skeys -- source keys, very often it is timestamps
# tkeys -- target keys, often it is timestamps
# sdata -- source data corresponding to the source keys
def sample_asof(skeys, tkeys, sdata):

    assert len(skeys) == len(sdata)
    tdata = [], ref_keys = []
    sidx, tidx, slen, tlen = 0, 0, len(skeys), len(tkeys)
    while tidx < tlen:
        if sidx >= slen or skeys[sidx] > tkeys[tidx]:
            tdata.append(None)
            ref_keys.append(None)
        else:
            # move sidx to the right until [sidx+1] is greater
            # or [sidx+1] is the end
            while sidx < slen and skeys[sidx] <= tkeys[tidx]:

                if sidx+1 >= slen or skeys[sidx+1] > tkeys[tidx]:
                    tdata.append(sdata[sidx])
                    ref_keys.append(skeys[sidx])
                    break
                sidx += 1

        tidx += 1
    return tdata, ref_keys


# sample nearest from the source
def sample_nearest(skeys, tkeys, sdata):

    assert len(skeys) == len(sdata)
    assert skeys
    tdata = []
    sidx, tidx, slen, tlen = 0, 0, len(skeys), len(tkeys)
    while tidx < tlen:
        # sidx won't go beyond the last index
        if skeys[sidx] > tkeys[tidx]:
            if sidx == 0:
                tdata.append(sdata[sidx])
            else:
                if (skeys[sidx-1]+skeys[sidx]) >= 2*tkeys[tidx]:
                    tdata.append(sdata[sidx-1])
                else:
                    tdata.append(sdata[sidx])
        else:
            # move sidx to the right until [sidx+1] is greater
            # or [sidx+1] is the end
            while sidx < slen and skeys[sidx] <= tkeys[tidx]:
                if sidx+1 == slen:
                    tdata.append(sdata[sidx])
                    break
                elif skeys[sidx+1] >= tkeys[tidx]:
                    if (skeys[sidx+1]+skeys[sidx]) >= 2*tkeys[tidx]:
                        tdata.append(sdata[sidx])
                    else:
                        tdata.append(sdata[sidx+1])
                    sidx += 1
                    break

        tidx += 1
    return tdata


if __name__ == "__main__":
    # test sample asof
    print(sample_asof([1, 3, 5], [2, 4], ['a', 'b', 'c']))
    print(sample_asof([], [2, 4], []))
    print(sample_asof([1, 3, 5], [], ['a', 'b', 'c']))
    print(sample_asof([1, 3, 5], [0, 1, 2, 3, 4, 5, 6], ['a', 'b', 'c']))

    # test sample asof
    print(sample_nearest([1, 3, 5], [2, 4], ['a', 'b', 'c']))
    print(sample_nearest([1, 3, 5], [], ['a', 'b', 'c']))
    print(sample_nearest([1, 3, 5], [0, 1, 2, 3, 4, 5, 6], ['a', 'b', 'c']))
