import pandas as pd
from pprint import pprint as pp

# profile pandas json serialization

def _make_large_json_sample():
    '''
    the structure would be a list of dictionary. Each dictionary could have
    sub-dictionaries.
    '''

    def _simple_dict(prefix, seed):
        # add 20 fields
        return dict(
            (
                '%s%d' % (prefix, _field_idx),
                '%s%d for %s' % (prefix, _field_idx, seed),
            )
            for _field_idx in range(20)
        )

    def _nest_dict(prefix1, prefix2, seed):
        return {prefix1 : _simple_dict(prefix2, seed)}

    def _one_element(seed):
        ret = _simple_dict('field', seed)
        ret.update(_nest_dict('outerone', 'innerone', seed))
        ret.update(_nest_dict('outertwo', 'innertwo', seed))
        return ret

    # create 1000 elements
    return [_one_element(_idx) for _idx in range(1000)]


if __name__ == '__main__':
    sample = _make_large_json_sample()
    # pp(sample)
    df = pd.json_normalize(sample)
