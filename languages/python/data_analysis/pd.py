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

    print('data generation costs %s seconds' % (end-start)) # around 2.38s
    # pp(sample)

    start = time.time()
    df1 = pd.json_normalize(sample)
    end = time.time()

    print('json normalize costs %s seconds' % (end-start)) # around 8.17s

    # picking fields that are of interest manually
    # say you only need field 1, 2, 3, 4, 5, 10, 11
    # and outerone.interone0-9
    # and outertwo.intertwo0-4, 10-14
    def _extract_simple_key(key):
        return [element[key] for element in sample]

    def _extract_nest_key(key1, key2):
        return [element[key1][key2] for element in sample]

    def _extract_part():
        return {
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
    start = time.time()
    df2 = pd.DataFrame(_extract_part())
    end = time.time()
    print('partial manual normalization costs %s seconds' % (end-start)) # around 0.78s

    def _extract_full():
        return {
            'field0' : _extract_simple_key('field0'),
            'field1' : _extract_simple_key('field1'),
            'field2' : _extract_simple_key('field2'),
            'field3' : _extract_simple_key('field3'),
            'field4' : _extract_simple_key('field4'),
            'field5' : _extract_simple_key('field5'),
            'field6' : _extract_simple_key('field6'),
            'field7' : _extract_simple_key('field7'),
            'field8' : _extract_simple_key('field8'),
            'field9' : _extract_simple_key('field9'),
            'field10' : _extract_simple_key('field10'),
            'field11' : _extract_simple_key('field11'),
            'field12' : _extract_simple_key('field12'),
            'field13' : _extract_simple_key('field13'),
            'field14' : _extract_simple_key('field14'),
            'field15' : _extract_simple_key('field15'),
            'field16' : _extract_simple_key('field16'),
            'field17' : _extract_simple_key('field17'),
            'field18' : _extract_simple_key('field18'),
            'field19' : _extract_simple_key('field19'),

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
            'outertwo.innertwo5': _extract_nest_key('outertwo', 'innertwo5'),
            'outertwo.innertwo6': _extract_nest_key('outertwo', 'innertwo6'),
            'outertwo.innertwo7': _extract_nest_key('outertwo', 'innertwo7'),
            'outertwo.innertwo8': _extract_nest_key('outertwo', 'innertwo8'),
            'outertwo.innertwo9': _extract_nest_key('outertwo', 'innertwo9'),
            'outertwo.innertwo10': _extract_nest_key('outertwo', 'innertwo10'),
            'outertwo.innertwo11': _extract_nest_key('outertwo', 'innertwo11'),
            'outertwo.innertwo12': _extract_nest_key('outertwo', 'innertwo12'),
            'outertwo.innertwo13': _extract_nest_key('outertwo', 'innertwo13'),
            'outertwo.innertwo14': _extract_nest_key('outertwo', 'innertwo14'),
            'outertwo.innertwo15': _extract_nest_key('outertwo', 'innertwo15'),
            'outertwo.innertwo16': _extract_nest_key('outertwo', 'innertwo16'),
            'outertwo.innertwo17': _extract_nest_key('outertwo', 'innertwo17'),
            'outertwo.innertwo18': _extract_nest_key('outertwo', 'innertwo18'),
            'outertwo.innertwo19': _extract_nest_key('outertwo', 'innertwo19'),
            'outertwo.innertwo20': _extract_nest_key('outertwo', 'innertwo20'),
            'outertwo.innertwo21': _extract_nest_key('outertwo', 'innertwo21'),
            'outertwo.innertwo22': _extract_nest_key('outertwo', 'innertwo22'),
            'outertwo.innertwo23': _extract_nest_key('outertwo', 'innertwo23'),
            'outertwo.innertwo24': _extract_nest_key('outertwo', 'innertwo24'),
            'outertwo.innertwo25': _extract_nest_key('outertwo', 'innertwo25'),
            'outertwo.innertwo26': _extract_nest_key('outertwo', 'innertwo26'),
            'outertwo.innertwo27': _extract_nest_key('outertwo', 'innertwo27'),
            'outertwo.innertwo28': _extract_nest_key('outertwo', 'innertwo28'),
            'outertwo.innertwo29': _extract_nest_key('outertwo', 'innertwo29'),
            'outertwo.innertwo30': _extract_nest_key('outertwo', 'innertwo30'),
            'outertwo.innertwo31': _extract_nest_key('outertwo', 'innertwo31'),
            'outertwo.innertwo32': _extract_nest_key('outertwo', 'innertwo32'),
            'outertwo.innertwo33': _extract_nest_key('outertwo', 'innertwo33'),
            'outertwo.innertwo34': _extract_nest_key('outertwo', 'innertwo34'),
            'outertwo.innertwo35': _extract_nest_key('outertwo', 'innertwo35'),
            'outertwo.innertwo36': _extract_nest_key('outertwo', 'innertwo36'),
            'outertwo.innertwo37': _extract_nest_key('outertwo', 'innertwo37'),
            'outertwo.innertwo38': _extract_nest_key('outertwo', 'innertwo38'),
            'outertwo.innertwo39': _extract_nest_key('outertwo', 'innertwo39'),
        }

    start = time.time()
    df3 = pd.DataFrame(_extract_full())
    end = time.time()
    print('full manual normalization costs %s seconds' % (end-start)) # around 2.06s

