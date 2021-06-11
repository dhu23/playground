import unittest

from core.computation import *
from core.data import *
from core import util
from pprint import pprint as pp

import itertools
import operator
import pandas as pd
import numpy as np


class TestDataFrameTransformation(unittest.TestCase):

    def setUp(self):

        self.df = pd.DataFrame({
            'A': [1, 2, 3] * 4,
            'B': [11, 22, 33, 44] * 3,
            'C': ['one', 'two'] * 6,
        })
        self.empty_df = pd.DataFrame(columns=['A', 'B', 'C'])

    def test_select(self):

        ret = transform_dataframe(RSelect(columns='A'), self.df)
        self.assertEqual(['A'], list(ret.columns))

        ret = transform_dataframe(RSelect(columns=('A', 'B')), self.df)
        self.assertEqual(['A', 'B'], list(ret.columns))

    def test_delete(self):

        ret = transform_dataframe(RDelete(columns='A'), self.df)
        self.assertEqual(['B', 'C'], list(ret.columns))

        ret = transform_dataframe(RDelete(columns=('A', 'B')), self.df)
        self.assertEqual(['C'], list(ret.columns))
    
    def test_rowfilter(self):

        filters = [
            RRowFilter(name='A', func=util.different_from(3)),
            RRowFilter(name='B', func=util.within(0, 30)), 
            RRowFilter(name='C', func=util.element_of(['one'])),
        ]

        answers = [
            pd.DataFrame({
                'A': [1, 2] * 4,
                'B': [11, 22, 44, 11, 33, 44, 22, 33],
                'C': ['one', 'two', 'two', 'one'] * 2,
            }),
            pd.DataFrame({
                'A': [1, 2, 2, 3, 3, 1], 
                'B': [11, 22] * 3, 
                'C': ['one', 'two'] * 3
            }),
            pd.DataFrame({
                'A': [1, 3, 2] * 2, 
                'B': [11, 33] * 3, 
                'C': ['one'] * 6
            }),
        ]

        cum_answer = pd.DataFrame({
            'A': [1, 2], 
            'B': [11, 11], 
            'C': ['one', 'one']})

        for i, (flt, ans) in enumerate(zip(filters, answers)):
            with self.subTest(i=i):
                ret = transform_dataframe(flt, self.df).reset_index(drop=True)
                self.assertTrue(util.is_equal_dataframe(ans, ret))

        # apply sequential filters
        cum_df = transform_dataframe(filters, self.df).reset_index(drop=True)
        self.assertTrue(util.is_equal_dataframe(cum_df, cum_answer))

        # apply multi dependency filter
        self.assertTrue(
            util.is_equal_dataframe(
                transform_dataframe(
                    RRowFilter(name=('A', 'B'), func=lambda a, b : 11*a == b),
                    self.df),
                pd.DataFrame({
                    'A': [1, 2, 3],
                    'B': [11, 22, 33],
                    'C': ['one', 'two', 'one']
                })
            )
        )

    def test_rowfilter_on_empty_dataframe(self):
        ret_df = transform_dataframe(
            RRowFilter(name='A', func=util.greater_than(5)),
            self.empty_df)

        self.assertEqual(set(ret_df.columns), {'A', 'B', 'C'})
        self.assertEqual(0, len(ret_df.index))

    def test_string_rowfilters(self):
        df = pd.DataFrame({
            'A': ['AB', 'BC', 'AC', 'BA'],
            'B': ['A-B', 'AA', 'AB', 'B-A'],
        })
        _startswith_A = RRowFilter(name='A', func=util.starting_with('A'))
        self.assertTrue(
            util.is_equal_dataframe(
                transform_dataframe(_startswith_A, df),
                pd.DataFrame({
                    'A': ['AB', 'AC'],
                    'B': ['A-B', 'AB']
                })
            )
        )

        _not_startingwith_A = RRowFilter(name='A', func=util.not_starting_with('A'))
        self.assertTrue(
            util.is_equal_dataframe(
                transform_dataframe(_not_startingwith_A, df),
                pd.DataFrame({
                    'A': ['BC', 'BA'],
                    'B': ['AA', 'B-A']
                })
            )
        )

    def test_columnfilter(self):
        self.assertTrue(
            util.is_equal_dataframe(
                transform_dataframe(
                    RColumnFilter(column='A', func=util.nlargest_unique(1)),
                    self.df).reset_index(drop=True),
                pd.DataFrame({
                    'A': [3, 3, 3, 3],
                    'B': [33, 22, 11, 44],
                    'C': ['one', 'two', 'one', 'two']
                })
            )
        )

    def test_rename(self):
        # single rename
        self.assertEqual(
            list(transform_dataframe(
                RRename(name='A', to='a'), self.df).columns),
            ['a', 'B', 'C'])

        # multiple renames
        self.assertEqual(
            list(transform_dataframe(
                RMultiRename({'a': 'a1', 'A': 'a', 'B': 'b'}), self.df)),
            ['a', 'b', 'C'])

    def test_rowadjust(self):
        rowadjust1 = RRowAdjust(
            name='A',
            func=util.make_function('A+B', ('A', 'B')), 
            deps=('A', 'B'))
        
        data1 = self.df.copy()
        # print(data1)
        data1 = transform_dataframe(rowadjust1, data1)
        # print(data1)
        
        self.assertTrue(all(data1['B'] == self.df['B']))
        self.assertTrue(all(data1['C'] == self.df['C']))
        self.assertTrue(
            all(data1['A'] == [12, 24, 36, 45, 13, 25, 34, 46, 14, 23, 35, 47]))

        rowadjust2 = RRowAdjust(
            name='C', 
            func=lambda x: x.upper(),
            deps='C')

        data2 = data1.copy()
        data2 = transform_dataframe(rowadjust2, data2)
        # print(data1)
        # print(data2)

        self.assertTrue(all(data2['A'] == data1['A']))
        self.assertTrue(all(data2['B'] == data1['B']))
        self.assertTrue(all(data2['C'] == ['ONE', 'TWO']*6))

        def _func(b, c):
            if c == 'one': c = 1
            elif c == 'two': c = 2
            else: c = 0
            return (b+c, b*c)
            
        rowadjust3 = RRowAdjust(name=('C', 'D'), func=_func, deps=('B', 'C'))
        data3 = transform_dataframe(rowadjust3, self.df)

        self.assertTrue(all(data3['A'] == self.df['A']))
        self.assertTrue(all(data3['B'] == self.df['B']))
        self.assertTrue(all(data3['C'] == [12, 24, 34, 46]*3))
        self.assertTrue(all(data3['D'] == [11, 44, 33, 88]*3))

    def test_rowadjust_on_empty_dataframe(self):
        rowadjust = RRowAdjust(
            name='D', func=util.make_function('A+B', ('A', 'B')),
            deps=('A', 'B'))

        ret_df = transform_dataframe(rowadjust, self.empty_df)
        self.assertEqual(set(ret_df.columns), {'A', 'B', 'C', 'D'})
        self.assertEqual(0, len(ret_df.index))
        
    def test_columnadjust(self):
        adjust1 = RColumnAdjust(name='A', func=operator.add, deps=('A', 'B'))
        ret = transform_dataframe(adjust1, self.df)
        self.assertTrue(all(ret['B'] == self.df['B']))
        self.assertTrue(all(ret['C'] == self.df['C']))
        self.assertTrue(
            all(ret['A'] == [12, 24, 36, 45, 13, 25, 34, 46, 14, 23, 35, 47]))

    def test_aggregate(self):
        # using a different source dataframe for easy aggregate
        df = pd.DataFrame({
            'A': [1, 2]*3,
            'B': ['a']*3 + ['b']*3,
            'C': [10, 20, 30, 40, 50, 60],
            'D': [1, 2, 3, 4, 5, 6],
        })
        # print('test agg')
        util.assert_dataframe(
            self, 
            transform_dataframe(RAggregate(by='A', aggs=[]), df),
            pd.DataFrame({'A': [1, 2]}))

        util.assert_dataframe(
            self,
            transform_dataframe(RAggregate(by=('A', 'B'), aggs=[]), df),
            pd.DataFrame({'A': [1, 1, 2, 2], 'B': ['a', 'b', 'a', 'b']}))

        util.assert_dataframe(
            self,
            transform_dataframe(
                RAggregate(
                    by=('A', 'B'), 
                    aggs=[
                        RColumnAdjust(
                            name='C', # renaming to existing column
                            func=lambda c, d : sum(c*d),
                            deps=('C', 'D')
                        ),
                        RColumnAdjust(
                            name=('E', 'F'),
                            func=lambda c : (sum(c), sum(c)**2),
                            deps='C'
                        )
                    ]),
                df),
            pd.DataFrame({
                'A': [1, 1, 2, 2], 'B': list('abab'),
                'C': [100, 250, 40, 520],
                'E': [40, 50, 20, 100], 'F': [1600, 2500, 400, 10000]
            })
        )

        util.assert_dataframe(
            self, 
            transform_dataframe(
                RAggregate(
                    by=None,
                    aggs=[
                        RColumnAdjust(
                            name='C', 
                            func=lambda c, d : sum(c*d),
                            deps=('C', 'D')
                        ),
                        RColumnAdjust(
                            name=('E', 'F'),
                            func=lambda c : (sum(c), sum(c)**2),
                            deps='C'
                        ),
                    ]),
                df),
            pd.DataFrame({
                'C': [910], 'E': [210], 'F': [44100]
            })
        )

    def test_rowsort(self):
        #print(transform_dataframe(RRowSort(by='A', ascending=True), self.df))
        util.assert_dataframe(
            self, 
            transform_dataframe(RRowSort(by='A', ascending=True), self.df),
            pd.DataFrame({
                'A': [1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3],
                'B': [11, 44, 33, 22, 22, 11, 44, 33, 33, 22, 11, 44],
                'C': [
                    'one', 'two', 'one', 'two', 
                    'two', 'one', 'two', 'one', 
                    'one', 'two', 'one', 'two'
                ],
            })
        )


def _make_dummy_func(to, deps):
    def _inner():
        print('this is function takes:{0}, returns:{1}'.format(deps, to))
    return _inner


def _make_filter(adjust_type, result_cols, dep_cols):
    return adjust_type(
        name=result_cols,
        func=_make_dummy_func(result_cols, dep_cols), 
        deps=dep_cols)


def _run_tests(ut, test_cases, answers):
    for test_case, ans in zip(test_cases, answers):
        with ut.subTest(case=test_case):
             # print('============')
             ret = ut.hier.resolve(test_case)
             # pp('ret.non-agg:\n')
             # pp(ret.non_agg)
             # pp('ans.non-agg\n')
             # pp(ans.non_agg)
             ut.assertEqual(ans, ret)


class HierarchyGraphTest(unittest.TestCase):

    def setUp(self):
        
        # col3 -- col1, col2, func1
        # col4 -- col1, col2
        # col2 -- col1
        # col5 -- col3
        # col7 -- col3
        # col8 -- col6
        # col7(agg) -- col7

        self.hier = Hierarchy()
        self.hier.add(_make_filter(RRowAdjust, ('col3', 'col4'), ('col1', 'col2')))
        self.hier.add(_make_filter(RRowAdjust, 'col2', 'col1'))
        self.hier.add(_make_filter(RRowAdjust, 'col5', 'col3'))
        self.hier.add(_make_filter(RRowAdjust, 'col7', 'col3'))
        self.hier.add(_make_filter(RRowAdjust, 'col8', 'col6'))
        self.hier.add(_make_filter(RColumnAdjust, 'agg-col7', 'col7'))

    def test_hierarchical_dependency1(self):
        '''simpole one-column/non-interacting-columns calculation test'''
        test_cases = [
            ['col1'],
            ['col2'],
            ['col7'],
            ['agg-col7'],
            ['col5', 'col8'],
            ['col8', 'col5'],
        ]
        
        answers = [
            ComputationSequence(
                non_agg=[], 
                agg=set(),
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                ],
                agg=set(),
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col7'), # 3 -> 7
                ],
                agg=set(),
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col7'), # 3 -> 7
                ],
                agg={
                    make_computation_key('agg-col7'), # 7 -> 7(agg)
                },
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col5'), # 3 -> 5
                    make_computation_key('col8'), # 6 -> 8
                ],
                agg=set(),
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col8'), # 6 -> 8
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col5'), # 3 -> 5
                ],
                agg=set(),
            ),
        ]

        _run_tests(self, test_cases, answers)
        
    def test_hierarchical_dependency2(self):
        '''corelated columns with different input order'''
        test_cases = [
            ['col3', 'col5'],
            ['col5', 'col3'],
        ]

        answers = [
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col5'), # 3 -> 5
                ],
                agg=set(),
            ),
            # idential to the previous case
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']), # 1, 2 -> 3
                    make_computation_key('col5'), # 3 -> 5
                ],
                agg=set(),
            ),
        ]

        _run_tests(self, test_cases, answers)

    def test_hierarchical_dependency3(self):
        '''columns with co-dependency'''

        test_cases = [
            ['col3', 'col4'],
            ['col4', 'col3'],
        ]

        answers = [
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']) # 1, 2 -> 3, 4
                ],
                agg=set(),
            ),
            ComputationSequence(
                non_agg=[
                    make_computation_key('col2'), # 1 -> 2
                    make_computation_key(['col3', 'col4']) # 1, 2 -> 3, 4
                ],
                agg=set(),
            ),
        ]
        
        _run_tests(self, test_cases, answers)


class HierarchyRunTest(unittest.TestCase):

    def setUp(self):

        dates = pd.date_range(start='2020-01-01', periods=4)

        self.source_data = pd.DataFrame({
            'date': itertools.chain.from_iterable((d, d, d) for d in dates),
            'sym': (['GC']*2 + ['GE']*2)*3,
            'price': map(float, range(1, 13)),
            'qty': [1, 2, 3, 4, 5, 6]*2,
            'client': ['A', 'B']*6
        })

        self.computation_rules = [
            make_custom_computation(RRowAdjust, 'value',
                                    'price*qty', ('price', 'qty')),
            make_custom_computation(RColumnAdjust, 'price-wavg-by-qty', 
                                    'sum(price*qty)/sum(qty)', 
                                    ('price', 'qty')),
            make_custom_computation(RColumnAdjust, 'sum-value',
                                    'sum(value)', 'value'),
        ]
        
        self.hier = Hierarchy()
        for rcomp in self.computation_rules:
            self.hier.add(rcomp)
        # print(source_data)

    def test_generate_report1(self): 
        '''non-agg report'''
        
        report_request = RQuery(data={
            'select': [
                {'name': 'sym'}, 
                {'name': 'price'}, 
                {'name': 'qty'}, 
                {'name': 'value'}, 
                {'name': 'client'},
            ],
            'filterby': [ 
                {'name': 'client', 'arg': 'A'} 
            ]
        })
        
        report = self.hier.run(report_request, self.source_data)
        
        ans = pd.DataFrame({
            'sym': ['GC', 'GE', 'GC', 'GE', 'GC', 'GE'],
            'price': [1., 3., 5., 7., 9., 11.],
            'qty': [1, 3, 5, 1, 3, 5],
            'value': [1., 9., 25., 7., 27., 55.],
            'client': 'A'
        })

        # print('generated\n', report)
        # print('should be\n', ans)

        self.assertTrue(util.is_equal_dataframe(report, ans))
    
    def test_generate_report2(self):

        report_request = RQuery(
            select=[
                SelectBuilder(name='sym', rename=None),
                SelectBuilder(name='client', rename=None),
                SelectBuilder(name='price-wavg-by-qty', rename=None),
                SelectBuilder(name='sum-value', rename=None)
            ],
            groupby=['sym', 'client'],
            filterby=None
        )

        report = self.hier.run(report_request, self.source_data)

        ans = pd.DataFrame({
            'sym': ['GC', 'GC', 'GE', 'GE'],
            'client': ['A', 'B', 'A', 'B'], 
            'price-wavg-by-qty': [5.8888889, 6.6666667, 7.8888889, 8.6666667],
            'sum-value': [53., 80., 71., 104.],
        })

        # print('generated\n', report)
        # print('should be\n', ans)

        self.assertTrue(util.is_equal_dataframe(report, ans))

    def test_generate_report3(self):

        report_request= RQuery(
            select=[
                SelectBuilder('client', None),
                SelectBuilder('price-wavg-by-qty', None),
                SelectBuilder('sum-value', None),
            ],
            groupby=['client'],
            filterby=[
                FilterBuilder(
                    name='date', fn='equal_to', 
                    arg=util.make_timestamp(2020, 1, 1)
                ),
            ]
        )

        report = self.hier.run(report_request, self.source_data)
        # print(self.source_data)
        # print(report)
        self.assertTrue(
            util.is_equal_dataframe(
                report, 
                pd.DataFrame({
                    'client': ['A', 'B'],
                    'price-wavg-by-qty': [2.5, 2.0],
                    'sum-value': [10.0, 4.0]
                })
            )
        )


if __name__ == '__main__':
    unittest.main()
