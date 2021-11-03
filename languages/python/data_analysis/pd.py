import pandas as pd
from pprint import pprint as pp
import time

# profile pandas json serialization

def _make_large_json_sample(size):
    '''
    the structure would be a list of dictionary. Each dictionary could have
    sub-dictionaries.
    '''

    def _simple_dict(prefix, seed, dsize):

        def _val(prefix, idx, seed):
            if idx in (0, 1): # a datetime value
                return '2021-11-02T05:30:00.%s' % (('%7d' % seed).replace(' ', '0'))
            elif idx in (2, 3): # int
                return int(seed)
            elif idx in (4, 5, 6, 7): # float
                return 10.0*seed
            elif idx == 8: # just a None value
                return None
            else: # string
                return '%s%d for %s' % (prefix, idx, seed)

        return dict(
            (
                '%s%d' % (prefix, _field_idx),
                _val(prefix, _field_idx, seed),
            )
            for _field_idx in range(dsize)
        )

    def _one_element(seed):
        ret = _simple_dict('field', seed, 20)
        ret.update({'outerone' : _simple_dict('innerone', seed, 10)})
        ret.update({'outertwo' : _simple_dict('innertwo', seed, 40)})
        return ret

    return [_one_element(_idx) for _idx in range(size)]


if __name__ == '__main__':
    sample = _make_large_json_sample(50000)
    # pp(sample)

    start = time.time()
    df = pd.json_normalize(sample)
    end = time.time()

    print('json normalize cost %s seconds' % (end-start))
