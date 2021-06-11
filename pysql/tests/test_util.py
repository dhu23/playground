import unittest

from core.util import *

from itertools import product
import pandas as pd
import numpy as np
import datetime
import math


class UtilTest(unittest.TestCase):

    def test_make_string_enum_class(self):

        for upper_enum, upper_value in product((True, False), (True, False)):

            with self.subTest(upper_enum=upper_enum, upper_value=upper_value):

                no_exception = True
                Color = make_string_enum_class(
                    'Color', 
                    ['Blue', 'Green', 'red'],
                    upper_case_enum=upper_enum,
                    upper_case_value=upper_value)

                if (upper_enum, upper_value) == (True, True):
                    try:
                        enums = (Color.BLUE, Color.GREEN, Color.RED)
                        names = ('BLUE', 'GREEN', 'RED')
                    except:
                        no_exception = False

                elif (upper_enum, upper_value) == (True, False):
                    try:
                        enums = (Color.BLUE, Color.GREEN, Color.RED)
                        names = ('blue', 'green', 'red')
                    except:
                        no_exception = False

                elif (upper_enum, upper_value) == (False, True):
                    try:
                        enums = (Color.blue, Color.green, Color.red)
                        names = ('BLUE', 'GREEN', 'RED')
                    except:
                        no_exception = False
                    
                else: # upper_enum, upper_value = False, False
                    try:
                        enums = (Color.blue, Color.green, Color.red)
                        names = ('blue', 'green', 'red')
                    except:
                        no_exception = False

                self.assertTrue(no_exception)
                self.assertEqual(tuple(map(lambda x : x.value, enums)), names)
                self.assertEqual(enums, tuple(map(Color, names)))


    def test_datetime_conversion(self):

        dates = [
            ('2020-01-01', (2020, 1, 1)),
            ('2020-02-29', (2020, 2, 29)),
            ('2020-03-02 14:20:35', (2020, 3, 2, 14, 20, 35))
        ]

        for d_str, d_args in dates:
            with self.subTest(d=d_str):
                self.assertEqual(
                    np.datetime64(d_str), 
                    make_datetime64(*d_args))

                self.assertEqual(
                    pd.Timestamp(d_str),
                    make_timestamp(*d_args))

        bad_dates = [ 
            (2020, 2, 35), 
            (2020, 14, 1),
            (2020, 3, 1, 25),
        ]

        for args in bad_dates:
            with self.subTest(d=args):
                with self.assertRaises(ValueError):
                    make_datetime64(*args)

                with self.assertRaises(ValueError):
                    make_timestamp(*args)

    def test_comp_func1(self):
        '''test comp functions that take 1 argument'''
        test_data = [
            {
                'case': 'int', 
                'value': 5,
                'data': [1, 2, 3, 4, 5, 6],
                'test': [
                    (equal_to, [False, False, False, False, True, False]),
                    (different_from, [True, True, True, True, False, True]),
                    (less_than, [True, True, True, True, False, False]),
                    (greater_than, [False, False, False, False, False, True])
                ]
            },
            {
                'case': 'str',
                'value': 'test-string',
                'data': ['test-string', 'test-string2'],
                'test': [
                    (equal_to, [True, False])
                ],
            },
            {
                'case': 'pandas-timestamp 1',
                'value': make_timestamp(2020, 3, 26, 15, 43, 20),
                'data': [
                    pd.Timestamp('2020-03-26 15:43:20'),
                    pd.Timestamp('2020-3-26 15:43:20'),
                    pd.Timestamp('2020-3-26 15:43:12'),
                    pd.Timestamp('2020-4-26 15:43:20')
                ],
                'test': [
                    (equal_to, [True, True, False, False]),
                    (different_from, [False, False, True, True]),
                    (less_than, [False, False, True, False]),
                    (greater_than, [False, False, False, True]),
                ],
            },
            {
                'case': 'pandas-timestamp 2',
                'value': make_timestamp(2020, 3, 26),
                'data': [
                    pd.Timestamp('2020-03-26'),
                    pd.Timestamp('2020-03-25'),
                    pd.Timestamp('2020-03-28 12:12:12')
                ],
                'test': [
                    (equal_to, [True, False, False]),
                    (different_from, [False, True, True]),
                    (less_than, [False, True, False]),
                    (greater_than, [False, False, True]),
                ],
            },
            # TODO add np.datetime64 cases
        ]
        for test_case in test_data:
            with self.subTest(case=test_case['case']):

                for f, results in test_case['test']:
                    run_func = f(test_case['value'])
                    for arg_data, result in zip(test_case['data'], results):

                        self.assertEqual(run_func(arg_data), result)

    def test_comp_func2(self):
        '''test functions that takes 2 arguments'''
        f = within(2, 5)
        data = range(10)
        results = (
            False, False,  # 0, 1
            True, True, True, True, # 2 to 5  
            False, False, False, False, # 6 - 9
        )

        self.assertEqual(tuple(map(f, data)), results)

    def test_past_day_of_week(self):
        d1, d2 = datetime.date(2020, 2, 20), datetime.date(2020, 5, 1)
        # wednesday of last week 
        self.assertEqual(
            past_day_of_week(d1, 2, 1), 
            datetime.date(2020, 2, 12))
        self.assertEqual(
            past_day_of_week(d2, 2, 1), 
            datetime.date(2020, 4, 22))

    def test_context(self):
        ctxt = Context(datetime.datetime(2020, 1, 15, 12, 13, 14))
        for ret, ans in [
                (ctxt.asof_by_dot, '20200115.121314'),
                (ctxt.asofyear, '2020'),
                (ctxt.asofmonth, '01'),
                (ctxt.asofday, '15'),
                (ctxt.asofhour, '12'),
                (ctxt.asofminute, '13'),
                (ctxt.asofsecond, '14'),
                (ctxt.asofdate, '20200115'),
                (ctxt.asofdate_dash, '2020-01-15'),
                (ctxt.asofdate_dot, '2020.01.15'),
                (ctxt.asoftime, '121314'),
                (ctxt.asoftime_colon, '12:13:14'),
                (ctxt.lastmonth, 'December'),
                (ctxt.lastyearmonth_dash, '2019-12'),
                (ctxt.yearoflastmonth, '2019'),
                (ctxt.prevweekday_dash, '2020-01-14'),
                (ctxt.recent30mbin, '2020.01.15-12:00:00'),
                (ctxt.recent15mbin, '2020.01.15-12:00:00'),
        ]:
            self.assertEqual(ret, ans)


    def test_general_function(self):

        data = [
            {
                'func' : ('a+b', ('a', 'b')),
                'check' : lambda a, b : a+b,
                'args' : product(range(-10, 10), range(-10, 10)),
                'count' : 20*20
            },
            {
                'func' : ('a*100+b*10+c', ('a', 'b', 'c')),
                'check' : lambda a, b, c : a*100+b*10+c,
                'args': product(
                    range(-10, 10), range(-10, 10), range(-10, 10)),
                'count' : 20*20*20
            },
            {
                'func' : ('math.sqrt(a*a+b*b)', ('a', 'b')),
                'check' : lambda a, b : math.sqrt(a*a+b*b),
                'args' : product(range(-10, 10), range(-10, 10)),
                'count' : 20*20
            },
            {
                'func' : ('divmod(x, y)', ('x', 'y')),
                'check' : lambda x, y : divmod(x, y),
                'args' : product(range(1, 51), range(1, 51)),
                'count' : 50*50
            }
        ]

        for test_data in data:
            with self.subTest(case=test_data):
                no_exception = True
                try:
                    func = make_function(*test_data['func'])
                    func0 = make_function0(*test_data['func'])
                except: 
                    no_exception = False
    
                self.assertTrue(no_exception)
                check = test_data['check']
                run_count = 0
                
                for args in test_data['args']:
                    # t0 = datetime.now()
                    left = func(*args)
                    # t1 = datetime.now()
                    left0 = func0(*args)
                    # t2 = datetime.now()
                    right = check(*args)
                    # t3 = datetime.now()
                    self.assertEqual(left0, right)
                    self.assertEqual(left, right)
                    # print('left0:', t2-t1, ',right:',t3-t2)
                    run_count += 1
    
                self.assertEqual(run_count, test_data['count'])
                
    def test_string_comp_functions(self):
        _startingwith_S = starting_with('S')
        self.assertTrue(_startingwith_S('S1'))
        self.assertFalse(_startingwith_S('A1'))

        _not_startingwith_S = not_starting_with('S')
        self.assertFalse(_not_startingwith_S('S1'))
        self.assertTrue(_not_startingwith_S('A1'))

    def test_assert_dataframe_columns(self):

        df = pd.DataFrame({'A': [1, 2], 'B': [3, 4], 'C': [5, 6]})
        
        for test_case in ['A', ['A'], ['A', 'B']]:
            self.assertEqual(None, assert_dataframe_columns(df, test_case))

        for test_case in ['a', ['a', 'A'], ('A', 'B', 'C', 'D')]:
            with self.assertRaises(DataFrameError):
                assert_dataframe_columns(df, test_case)

    def test_series_to_dataframe(self):

        # str case
        ret = series_to_dataframe('C', pd.Series([1, 2, 3, 4]))
        self.assertTrue(all(ret['C'] == pd.Series([1, 2, 3, 4])))

        # list/tuple cases
        ret = series_to_dataframe(['A', 'B'], pd.Series([(1, 2), (3, 4)]))
        self.assertTrue(all(ret['A'] == pd.Series([1, 3])))
        self.assertTrue(all(ret['B'] == pd.Series([2, 4])))

    def test_concat_datefroms(self):
        
        left_df = pd.DataFrame({'A': [1, 2, 3, 4], 'B': list('abcd')})
        right_df1 = pd.DataFrame({'C': [11, 22, 33, 44], 'D': list('ABCD')})
        right_df2 = pd.DataFrame({'A': [10, 20, 30, 40], 'C': [11, 22, 33, 44]})

        with self.subTest('has-no-dupulicate'):
            left = left_df.copy()

            concat_dataframes(left, right_df1, overwrite=True)

            self.assertTrue(all(c in left for c in 'ABCD'))
            self.assertTrue(all(left['A'] == left_df['A']))
            self.assertTrue(all(left['B'] == left_df['B']))
            self.assertTrue(all(left['C'] == right_df1['C']))
            self.assertTrue(all(left['D'] == right_df1['D']))

        with self.subTest('has dup with overwrite'):
            left = left_df.copy()

            concat_dataframes(left, right_df2, overwrite=True)

            self.assertTrue(all(c in left for c in 'ABC'))
            self.assertTrue(all(left['A'] == right_df2['A']))
            self.assertTrue(all(left['B'] == left_df['B']))
            self.assertTrue(all(left['C'] == right_df2['C']))

        with self.subTest('has dup without overwrite'):
            left = left_df.copy()

            concat_dataframes(left, right_df2, overwrite=False)

            self.assertTrue(all(c in left for c in 'ABC'))
            self.assertTrue(all(left['A'] == left_df['A']))
            self.assertTrue(all(left['B'] == left_df['B']))
            self.assertTrue(all(left['C'] == right_df2['C']))
        
    def test_join_dataframes(self):
        df1 = pd.DataFrame({
            'A': [1, 1, 1, 1, 1, 2, 2, 2],
            'B': list('aabbbaab'),
            'C': [11, 12, 13, 14, 15, 16, 17, 18]
        })

        df2 = pd.DataFrame({
            'A': [1, 2, 3],
            'A-def': ['one', 'two', 'three'],
        })

        df3 = pd.DataFrame({
            'A': [1, 1, 2, 2],
            'B': list('abab'),
            'D': ['a for 1', 'b for 1' ,'a for 2', 'b for 2']
        })

        df4 = pd.DataFrame({
            'A2': df3['A'],
            'B2': df3['B'],
            'D': df3['D']
        })

        ans1 = pd.DataFrame({
            'A': df1['A'],
            'B': df1['B'],
            'C': df1['C'],
            'A-def': ['one', 'one', 'one', 'one', 'one', 'two', 'two', 'two']
        })
        ret1 = left_join_dataframes('A', df1, df2, dup_resolve='left')
        assert_dataframe(self, ret1, ans1)

        ret1 = left_join_dataframes(['A'], df1, df2, dup_resolve='left') 
        assert_dataframe(self, ret1, ans1)

        ret1 = left_join_dataframes(
            JoinBy(left='A', right='A'), df1, df2, dup_resolve='left')
        assert_dataframe(self, ret1, ans1)

        ret1 = left_join_dataframes(
            JoinBy(left=['A'], right=['A']), df1, df2, dup_resolve='left')
        assert_dataframe(self, ret1, ans1)


        ans2 = pd.DataFrame({
            'A': df1['A'],
            'B': df1['B'],
            'C': df1['C'],
            'D': ['a for 1']*2 + ['b for 1']*3 + ['a for 2']*2 + ['b for 2']
        })

        ret2 = left_join_dataframes(['A', 'B'], df1, df3, dup_resolve='left')
        assert_dataframe(self, ret2, ans2)


        ans3 = ans2.copy()
        ret3 = left_join_dataframes(
            JoinBy(left=['A', 'B'], right=['A2', 'B2']), 
            df1, df4, dup_resolve='left')
        # print('ans3=\n', ans3)
        # print('ret3=\n', ret3)
        assert_dataframe(self, ret3, ans3)


def _get_count(dag, n):
    if n in dag:
        return len(dag[n])
    else:
        return 0


def _make_node_item(dag, n):
    return NodeItem(n, _get_count(dag, n))


def _make_node_item_list(dag, ns):
    return [_make_node_item(dag, n) for n in ns]


class TopologicalSortTest(unittest.TestCase):

    def setUp(self):

        self.dag = {
            'A': ['B', 'C', 'E'],
            'B': ['F'],
            'C': ['D', 'E'],
            'D': ['H', 'G', 'F'],
            'E': ['G'],
            'F': [],
            'G': [],
            'H': []
        }

        def _adj_func(dag, node):
            return dag.get(node, [])

        self.adj_func = _adj_func
     
    def _run_test(self, test_cases, answers):
        for test_case, ans in zip(test_cases, answers):
            with self.subTest(case=test_case):
                ret = topological_order(
                    self.dag, self.adj_func, *test_case)
                self.assertEqual(ret, ans)

    def _make_tests(self, nodes):

        ret = []
        # make 
        # False, False
        # False, True
        # True, False 
        # True, True
        for source_first, include_connection in \
                product([False, True], [False, True]):
            ret.append((nodes, source_first, include_connection))

        return ret

    def _make_order(self, nodes):
        return _make_node_item_list(self.dag, nodes)

    def test_order1_result_comparsion(self):

        test_cases1 = self._make_tests(['C', 'A'])
        test_cases2 = self._make_tests(['A', 'C'])

        partial_order = self._make_order(['C', 'A'])
        full_order = self._make_order(['G', 'E', 'F', 'H', 'D', 'C', 'B', 'A'])

        answers = [
            partial_order,
            full_order,
            list(reversed(partial_order)),
            list(reversed(full_order))
        ]

        self._run_test(test_cases1, answers)
        self._run_test(test_cases2, answers)

    def test_order2_result_comparision(self): 

        test_cases1 = self._make_tests(['G', 'C'])
        test_cases2 = self._make_tests(['C', 'G'])

        partial_order = self._make_order(['G', 'C'])
        full_order = self._make_order(['G', 'E', 'F', 'H', 'D', 'C'])

        answers = [
            partial_order, 
            full_order,
            list(reversed(partial_order)),
            list(reversed(full_order)),
        ]

        self._run_test(test_cases1, answers)
        self._run_test(test_cases2, answers)

    def test_order3_result_comparsion(self):

        test_cases1 = self._make_tests(['B', 'F'])
        test_cases2 = self._make_tests(['F', 'B'])

        partial_order = self._make_order(['F', 'B'])

        answers = [
            partial_order,
            partial_order,
            list(reversed(partial_order)),
            list(reversed(partial_order)),
        ]

        self._run_test(test_cases1, answers)
        self._run_test(test_cases2, answers)

    def test_order_verification(self):

        source_first_order = topological_order(
            self.dag, self.adj_func, list(self.dag.keys()), source_first=True)

        self.assertTrue(
            verify_topological_order(
                self.dag, self.adj_func,self.dag.keys(), 
                [tn.name for tn in source_first_order]
            )
        )

    def test_graph_connection(self):
        
        topo_order = topological_order(
            self.dag, self.adj_func, list(self.dag.keys()))

        ret_inclusive = graph_connection(
            self.dag, topo_order, self.adj_func, True)
        ret_non_inclusive = graph_connection(
            self.dag, topo_order, self.adj_func, False)

        #print(ret_inclusive)
        #print(ret_non_inclusive)
        self.assertEqual(
            ret_inclusive, 
            [
                {'G'},
                {'G', 'E'},
                {'F'},
                {'H'},
                {'D', 'G', 'H', 'F'},
                {'G', 'H', 'E', 'D', 'C', 'F'},
                {'B', 'F'},
                {'G', 'H', 'B', 'A', 'D', 'C', 'F', 'E'}
            ]
        )
        self.assertEqual(
            ret_non_inclusive,
            [
                set(),
                {'G'},
                set(),
                set(),
                {'H', 'G', 'F'},
                {'D', 'F', 'H', 'G', 'E'},
                {'F'},
                {'H', 'B', 'D', 'G', 'E', 'C', 'F'}
            ]
        )


if __name__ == '__main__':
    unittest.main()
