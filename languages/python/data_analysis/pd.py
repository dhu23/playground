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
    start = time.time()
    sample = _make_large_json_sample(50000)
    end = time.time()

    print('data generation costs %s seconds' % (end-start)) # 2.38s on kudu
    # pp(sample)

    start = time.time()
    df = pd.json_normalize(sample)
    end = time.time()

    print('json normalize costs %s seconds' % (end-start)) # 8.03s on kudu

    # picking fields that are of interest manually
    # say you only need field 1, 2, 3, 4, 5, 10, 11
    # and outerone.interone0-9
    # and outertwo.intertwo0-4, 10-14
    def _extract_simple_key(key):
        return [element[key] for element in sample]

    def _extract_nest_key(key1, key2):
        return [element[key1][key2] for element in sample]

    start = time.time()
    data = {
        'field1' : _extract_simple_key('field1'),
        'field2' : _extract_simple_key('field2'),
        'field3' : _extract_simple_key('field3'),
        'field4' : _extract_simple_key('field4'),
        'field5' : _extract_simple_key('field5'),
        'field10' : _extract_simple_key('field10'),
        'field11' : _extract_simple_key('field11'),
        'outerone.innerone0': _extract_nest_key('outerone', 'innerone0'),
        'outerone.innerone1': _extract_nest_key('outerone', 'innerone1'),
        'outerone.innerone2': _extract_nest_key('outerone', 'innerone2'),
        'outerone.innerone3': _extract_nest_key('outerone', 'innerone3'),
        'outerone.innerone4': _extract_nest_key('outerone', 'innerone4'),
        'outerone.innerone5': _extract_nest_key('outerone', 'innerone5'),
        'outerone.innerone6': _extract_nest_key('outerone', 'innerone6'),
        'outerone.innerone7': _extract_nest_key('outerone', 'innerone7'),
        'outerone.innerone8': _extract_nest_key('outerone', 'innerone8'),
        'outerone.innerone9': _extract_nest_key('outerone', 'innerone9'),
        'outertwo.innertwo0': _extract_nest_key('outertwo', 'innertwo0'),
        'outertwo.innertwo1': _extract_nest_key('outertwo', 'innertwo1'),
        'outertwo.innertwo2': _extract_nest_key('outertwo', 'innertwo2'),
        'outertwo.innertwo3': _extract_nest_key('outertwo', 'innertwo3'),
        'outertwo.innertwo4': _extract_nest_key('outertwo', 'innertwo4'),
        'outertwo.innertwo10': _extract_nest_key('outertwo', 'innertwo10'),
        'outertwo.innertwo11': _extract_nest_key('outertwo', 'innertwo11'),
        'outertwo.innertwo12': _extract_nest_key('outertwo', 'innertwo12'),
        'outertwo.innertwo13': _extract_nest_key('outertwo', 'innertwo13'),
        'outertwo.innertwo14': _extract_nest_key('outertwo', 'innertwo14'),
    }
    df2 = pd.DataFrame(data)
    end = time.time()

    print('manual normalization costs %s seconds' % (end-start)) # 0.77s on kudu

    start = time.time()
    df22 = pd.json_normalize(data)
    end = time.time()

    print('manual normalization with json_normalize costs %s seconds' % (end-start)) # 0.60s on kudu
