import unittest
from core.data import *
from core.util import (
    verify_topological_order, 
    tuples_from_dataframe,
    is_equal_series,
    is_equal_dataframe,
)

from pprint import pprint as pp
import pandas as pd
import datetime

def _make_djoin_dict(ds): # ds is DSequence
    return dict((dj.source.table, dj) for dj in ds)


_table_metas = [
    TableMeta(
        name='T0',
        columns=('C1', 'C2', 'C3', 'C4'),
        foreign=[
            make_foreignkey(table='T1', column='C1'),
            make_foreignkey(table='T2', column='C2'),
        ]
    ),
    TableMeta(
        name='T1',
        columns=('C1', 'C20', 'C30'),
        foreign=None
    ),
    TableMeta(
        name='T2',
        columns=('C2', 'C50', 'C60', 'C200', 'C300'),
        foreign=[
            make_foreignkey(table='T3', column='C50'),
            make_foreignkey(table='T4', column='C60'),
        ]
    ),
    TableMeta(
        name='T3',
        columns=('C50', 'C500'),
        foreign=None
    ),
    TableMeta(
        name='T4', 
        columns=('C60', 'C600'),
        foreign=None
    )
]


class TestDataModel(unittest.TestCase):

    def setUp(self):
        self.dm = DataModel(_table_metas)
        # print('---------------------------------')
        # print(self.dm.topo)
        # print(self.dm)
        
    def test_internal_data(self):
        self.assertEqual(
            self.dm.foreign_map,
            {
                'T1': {'C1'}, 
                'T2': {'C2'},
                'T3': {'C50'},
                'T4': {'C60'},
            }
        )

    def test_data_query(self):
        test_cases = [
            ['C1', 'C3'], # select from main table
            ['C3', 'C4'], # select from main table without key
            ['C1', 'C30'], # one join with key included
            ['C2', 'C20'], # one join without key included
            ['C1', 'C2', 'C3', 'C200', 'C500'], # 3 tables (chain structure)
            ['C1', 'C2', 'C3', 'C20', 'C200'], # 3 tables (fork structure)
            ['C4', 'C30', 'C300', 'C500'], # 4 tables without key involved
            ['C200', 'C500', 'C600'], # without T0
            ['C4', 'C600'], # skipping "bridge table"
        ]

        answers = [
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C3'}, transforms=None), 
                    joinby=None)
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C3', 'C4'}, transforms=None), 
                    joinby=None)
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C1', 'C30'}, transforms=None), 
                    joinby=None)
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2'}, transforms=None), 
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C1', 'C20'}, transforms=None), 
                    joinby={'C1'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2', 'C3'}, transforms=None), 
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C2', 'C50', 'C200'}, transforms=None), 
                    joinby={'C2'}),
                DJoin(
                    source=DSelect(
                        table='T3', columns={'C50', 'C500'}, transforms=None),
                    joinby={'C50'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2', 'C3'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C1', 'C20'}, transforms=None), 
                    joinby={'C1'}),
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C2', 'C200'}, transforms=None), 
                    joinby={'C2'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2', 'C4'}, transforms=None), 
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C1', 'C30'}, transforms=None), 
                    joinby={'C1'}),
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C2', 'C50', 'C300'}, transforms=None), 
                    joinby={'C2'}),
                DJoin(
                    source=DSelect(
                        table='T3', columns={'C50', 'C500'}, transforms=None),
                    joinby={'C50'})
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C50', 'C60', 'C200'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T3', columns={'C50', 'C500'}, transforms=None), 
                    joinby={'C50'}),
                DJoin(
                    source=DSelect(
                        table='T4', columns={'C60', 'C600'}, transforms=None), 
                    joinby={'C60'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C4', 'C2'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C2', 'C60'}, transforms=None),
                    joinby={'C2'}),
                DJoin(
                    source=DSelect(
                        table='T4', columns={'C60', 'C600'}, transforms=None), 
                    joinby={'C60'})
            ]),
        ]

        def adj_func(dag, n):
            tm = dag[n]
            if tm.foreign:
                return [f.table for f in tm.foreign]
            else:
                return []

        for test_case, ans in zip(test_cases, answers):
            with self.subTest(case=test_case):
                ret = self.dm.resolve(list(test_case))
                # print('=====')
                # print(ans)
                # print(ret)
                self.assertTrue(
                    verify_topological_order(
                        self.dm.tables, adj_func, 
                        [q.source.table for q in ret], # input nodes to check
                        [q.source.table for q in ret] # source first order
                    )
                )
                # topological order might not matching exactly
                self.assertEqual(_make_djoin_dict(ret), _make_djoin_dict(ans))

        
# T0 -- office hour schedule
# C1 -> student_id 
# C2 -> teacher_id 
# C3 -> office_hour
# C4 -> at teacher's office

# T1 -- student profile
# C1 -> student_id
# C20 -> student_name
# C30 -> grade

# T2 -- teach profile
# C2 -> teacher_id
# C50 -> education_level
# C60 -> building
# C200 -> teacher_name
# C300 -> subject

# T3 -- education data
# C50 -> education_level
# C500 -> base salary

# T4 -- building information
# C60 -> building
# C600 -> open after school hours

# this is a fact table that may or may not make sense
test_t0 = pd.DataFrame({
    'C1': ['s1', 's2', 's3', 's4', 's1', 's3',],
    'C2': ['t1', 't2', 't3', 't2', 't2', 't1'],
    'C3': ['1PM', '2PM', '3PM', '3PM', '2PM', '4PM'],
    'C4': [True, True, False, False, False, True],
})

# dimension tables
test_t1 = pd.DataFrame({
    'C1': ['s1', 's2', 's3', 's4', 's5', 's6'],
    'C20': ['Joe', 'Mary', 'Dave', 'Sarah', 'Mike', 'Liz'], 
    'C30': [9, 12, 12, 8, 11, 11],
})

test_t2 = pd.DataFrame({
    'C2': ['t1', 't2', 't3', 't4', 't5'],
    'C50': ['BA', 'BA', 'MS', 'BA', 'PhD'],
    'C60': ['B1', 'B2', 'B1', 'B2', 'B2'],
    'C200': ['Aaron', 'Steve', 'Elizabeth', 'Maria', 'Sarah'],
    'C300': ['Math', 'English', 'Math', 'History', 'Science'],
})

test_t3 = pd.DataFrame({
    'C50': ['BA', 'BS', 'MS', 'PhD'],
    'C500': [75, 75, 78, 80],
})

test_t4 = pd.DataFrame({
    'C60': ['B1', 'B2', 'B3', 'B4'],
    'C600': [True, True, False, False]
})


# make TestFetcher class
TestFetcher = make_stub_data_fetcher(
    'TestFetcher', 
    dict(zip(
        'T0 T1 T2 T3 T4'.split(),
        [test_t0, test_t1, test_t2, test_t3, test_t4]))
)


class TestDataFetching(unittest.TestCase):
    
    def setUp(self):

        self.dm = DataModel(_table_metas)
        self.fetchergen = TestFetcher
    
    def _fetch(self, columns):

        # dt is not used, passed as None
        query_order = self.dm.resolve(columns)
        return fetch(query_order, self.fetchergen(), 'env', None)

    def test_query1(self):
        ret = self._fetch(['C1', 'C3'])
        # print(ret)
        ans = pd.DataFrame({
            'C1': test_t0['C1'],
            'C3': test_t0['C3'],
        })
        self.assertTrue(is_equal_dataframe(ret, ans, check_order=False))

    def test_query2(self):
        columns = ['C1', 'C2', 'C3', 'C200', 'C500']
        ret = self._fetch(columns)
        # print('test query2 ret=\n', ret)

        self.assertEqual(len(ret), 6)
        self.assertEqual(set(ret['C1']), {'s1', 's2', 's3', 's4'})
        
        ans = pd.DataFrame({
            'C1': ['s1', 's2', 's3', 's4', 's1', 's3',],
            'C2': ['t1', 't2', 't3', 't2', 't2', 't1'],
            'C3': ['1PM', '2PM', '3PM', '3PM', '2PM', '4PM'],
            'C200': ['Aaron', 'Steve', 'Elizabeth', 'Steve', 'Steve', 'Aaron'],
            'C500': [75, 75, 78, 75, 75, 75]
        })

        _to_tuples = tuples_from_dataframe(columns)

        self.assertEqual(set(_to_tuples(ret)), set(_to_tuples(ans)))

    def test_query3(self):
        columns = ['C200', 'C500', 'C600']
        ret = self._fetch(columns)

        ans = pd.DataFrame({
            'C200': ['Aaron', 'Steve', 'Elizabeth', 'Maria', 'Sarah'],
            'C500': [75, 75, 78, 75, 80],
            'C600': [True, True, True, True, True]
        })

        _to_tuples = tuples_from_dataframe(columns)

        self.assertEqual(set(_to_tuples(ret)), set(_to_tuples(ans)))


_test_data_model = DataModel(
    [
        TableMeta(
            name='maintable', 
            columns=('date', 'orderid', 'sym', 'price', 'qty', 'side'),
            foreign=[
                make_foreignkey(table='symtable', column='sym')
            ],
        ),
        TableMeta(
            name='symtable', 
            columns=('sym', 'ctype', 'inst', 'exchange'),
            foreign=[
                make_foreignkey(table='insttable', column=('inst', 'ctype')),
            ]
        ),
        TableMeta(
            name='insttable', 
            columns=('inst', 'ctype', 'name', 'fo'),
            foreign=None
        ),
    ]
)


class TestDataModelWithCombinedForeignKey(unittest.TestCase):
    
    def setUp(self):

        self.data_model = _test_data_model
        # print('\n', self.data_model.to_dict(), '\n')

    def test1(self):
        tests = [
            ['orderid', 'name']
        ]
        answers = [
            DSequence([
                DJoin(
                    source=DSelect(
                        table='maintable', columns={'orderid', 'sym'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='symtable', columns={'sym', 'inst', 'ctype'}, transforms=None),
                    joinby={'sym'}),
                DJoin(
                    source=DSelect(
                        table='insttable', columns={'name', 'inst', 'ctype'}, transforms=None),
                    joinby={'inst', 'ctype'})
            ])
        ]

        for test_case, ans in zip(tests, answers):
            with self.subTest(case=test_case):
                ret = self.data_model.resolve(list(test_case))
                self.assertTrue(
                    verify_topological_order(
                        self.data_model.graph, self.data_model.adj_func,
                        [q.source.table for q in ret], 
                        [q.source.table for q in ret]
                    )
                ) 
                # order might not matching exactly
                self.assertEqual(_make_djoin_dict(ret), _make_djoin_dict(ans))


_test_order_data = {
    'maintable': pd.DataFrame({
        'orderid': [1, 2, 3, 4, 5], 
        'sym': ['CLZ0', 'CLZ0', 'CLM0', 'GCZ0', 'GCZ0'],
    }),
    'symtable': pd.DataFrame({
        'sym': ['CLZ0', 'CLM0', 'GCZ0', 'GCM0'],
        'inst': ['CL', 'CL', 'GC', 'GC'],
        'ctype': ['OT', 'OT', 'OT', 'OT'],
    }),
    'insttable': pd.DataFrame({
        'name': [
            'NYME Light Sweet Crude Oil Future', 
            'COMEX Gold 100 Troy Ounces Future',
            'CME Ultra Long Term US Treasury Bond Future',
        ] * 2,
        'inst': ['CL', 'GC', 'UB'] * 2, 
        'ctype': ['OT'] * 3 + ['SP'] * 3
    }),
}


class TestOrderDataFetcher(DataFetcher):

    def __init__(self):
        super(TestOrderDataFetcher, self).__init__()
        self.interested_syms = None
        self.interested_insts = None

    def fetch_data(self, env, table_name, columns, by_dt): # env/date ignored
        if table_name == 'maintable':
            ret = _test_order_data['maintable'][columns].copy()
            if 'sym' in ret:
                self.interested_syms = list(ret['sym'])
            return ret

        elif table_name == 'symtable':
            _symtable = _test_order_data['symtable']
            if self.interested_syms is None:
                _selected = _symtable
            else:
                _selected = _symtable[
                    _symtable['sym'].isin(self.interested_syms)
                ]
            ret = _selected[columns].copy()
            if 'inst' in ret:
                self.interested_insts = list(ret['inst'])
            return ret
        elif table_name == 'insttable':
            _insttable = _test_order_data['insttable']
            if self.interested_insts is None:
                _selected = _insttable
            else:
                _selected = _insttable[
                    _insttable['inst'].isin(self.interested_insts)
                ]
            ret = _selected[columns].copy()
            return ret
        else:
            raise RuntimeError('Unknow table %s' % table_name)

    def support(self, table_name):
        return table_name in ('maintable', 'symtable', 'insttable')



class TestOrderDataFetching(unittest.TestCase):
    
    def setUp(self):
        self.dm = _test_data_model
        self.fetchergen = TestOrderDataFetcher
    
    def test_fetching(self):
        query_order = self.dm.resolve(['orderid', 'name'])
        data = fetch(query_order, self.fetchergen(), 'env', None)
        # print(data)
        self.assertEqual(len(data), 5)
        self.assertEqual(
            list(data['orderid']),
            list(_test_order_data['maintable']['orderid'])
        )
        self.assertEqual(
            list(data['name']), 
            [
                'NYME Light Sweet Crude Oil Future',
                'NYME Light Sweet Crude Oil Future',
                'NYME Light Sweet Crude Oil Future',
                'COMEX Gold 100 Troy Ounces Future',
                'COMEX Gold 100 Troy Ounces Future',
            ]
        )


_txn_table_meta = [
    TableMeta(
        name='childexecs',
        columns=('date', 'cordid', 'execid'),
        foreign=[
            make_foreignkey(table='childords', column=('date', 'cordid')),
        ]
    ),
    TableMeta(
        name='childords', 
        columns=('date', 'cordid', 'parentid', 'cqty'),
        foreign=[
            make_foreignkey(table='parentords', column=('date', 'parentid')),
        ]
    ),
    TableMeta(
        name='parentords', 
        columns=('date', 'parentid', 'client', 'pqty'),
        foreign=None
    ),
]


class TestDatedDataModel(unittest.TestCase):

    def setUp(self):
        self.dm = DataModel(_txn_table_meta)
        # pp(self.dm)
    
    def test_resolve(self):
        test_cases = [
            ['date', 'execid', 'parentid'],
            ['date', 'parentid', 'pqty'],
            ['date', 'cordid', 'cqty'],
            ['date', 'cordid', 'execid', 'client']
        ]

        answers = [
            DSequence([
                DJoin(
                    source=DSelect(
                        table='childexecs', 
                        columns={'date', 'cordid', 'execid'},
                        transforms=None,
                    ),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='childords',
                        columns={'date', 'cordid', 'parentid'},
                        transforms=None,
                    ),
                    joinby={'date', 'cordid'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='parentords',
                        columns={'date', 'parentid', 'pqty'},
                        transforms=None,
                    ),
                    joinby=None),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='childords',
                        columns={'date', 'cordid', 'cqty'}, 
                        transforms=None,
                    ),
                    joinby=None),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='childexecs', 
                        columns={'date', 'cordid', 'execid'}, 
                        transforms=None,
                    ),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='childords',
                        columns={'date', 'cordid', 'parentid'},
                        transforms=None,
                    ),
                    joinby={'date', 'cordid'}),
                DJoin(
                    source=DSelect(
                        table='parentords', 
                        columns={'date', 'parentid', 'client'},
                        transforms=None,
                    ),
                    joinby={'date', 'parentid'}),
            ]),
        ]

        for test_case, ans in zip(test_cases, answers):
            with self.subTest(case=test_case):
                ret = self.dm.resolve(test_case)
                self.assertEqual(ret, ans)



class TestDataQuery(unittest.TestCase):

    def test_setup(self):
        queries = [
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2'}, transforms=None), 
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C2', 'C3'}, transforms=None),
                    joinby={'C2'}),
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(
                        table='T1', columns={'C2', 'C3'}, transforms=None),
                    joinby={'C2'}),
                DJoin(
                    source=DSelect(
                        table='T2', columns={'C1', 'C3', 'C4'}, transforms=None),
                    joinby={'C1', 'C3'})
            ]),
            DSequence([
                DJoin(
                    source=DSelect(
                        table='T0', columns={'C1', 'C2', 'C3'}, transforms=None),
                    joinby=None),
                DJoin(
                    source=DSequence([
                        DJoin(
                            source=DSelect(
                                table='T1', 
                                columns={'C2', 'C200', 'C201'},
                                transforms=None,
                            ),
                            joinby=None),
                        DJoin(
                            source=DSelect(
                                table='T2', 
                                columns={'C3', 'C200', 'C301'},
                                transforms=None,
                            ),
                            joinby={'C200'})
                    ]),
                    joinby={'C2', 'C3'})
            ]),
        ]

        self.dqs = [DataQuery(q) for q in queries]

        # print(to_jsonlike(self.dqs))


class TestAcquisitionKey(unittest.TestCase):

    def test_datakey(self):
        test_cases = [
            (
                'dummy_acquisition.prod.dr20200210-20200210',
                AcquisitionKey(
                    acquisition='dummy_acquisition', env='prod', 
                    srcdt=DateRange(
                        datetime.date(2020, 2, 10), 
                        datetime.date(2020, 2, 10))
                ),
            ),
            (
                'dummy_acquisition.sim.dt20200210143025',
                AcquisitionKey(
                    acquisition='dummy_acquisition', env='sim',
                    srcdt=DateTimePoint(datetime.datetime(2020, 2, 10, 14, 30, 25))
                ),
            ),
        ]
        for s, ak in test_cases:
            with self.subTest(case=s):
                ret_ak = acquisition_key_from_string(s)
                self.assertEqual(ret_ak, ak)
                self.assertEqual(acquisition_key_to_str(ret_ak), s)

    def test_date_datakey_exceptions(self):
        with self.assertRaises(RuntimeError):
            _ = acquisition_key_from_string('dummy.env.20200210143025')

        with self.assertRaises(RuntimeError):
            _ = acquisition_key_from_string('dummy.env.20200210')



class TestDateRelated(unittest.TestCase):

    def test_past_days(self):
        d1, d2 = datetime.date(2020, 2, 20), datetime.date(2020, 5, 1)

        _5_days_ago = DateRange.past_days(5, 5)
        self.assertEqual(
            _5_days_ago(d1), 
            DateRange(datetime.date(2020, 2, 15))
        )
        self.assertEqual(
            _5_days_ago(d2), 
            DateRange(datetime.date(2020, 4, 26))
        )

        self.assertEqual(DateRange.just_today(d1), DateRange(d1))
        self.assertEqual(DateRange.just_today(d2), DateRange(d2))

        _2_days_future = DateRange.past_days(-2, -2)
        self.assertEqual(
            _2_days_future(d1), 
            DateRange(datetime.date(2020, 2, 22))
        )
        self.assertEqual(
            _2_days_future(d2), 
            DateRange(datetime.date(2020, 5, 3))
        )


    def test_past_minute_bin(self):
        
        t1 = datetime.datetime(2020, 2, 20, 20, 25, 21, 123456)
        t2 = datetime.datetime(2020, 5, 1, 1, 2, 1, 654321)

        _1_15_mins_bin_ago = DateTimePoint.last_minutes_bin(15, 1)
        self.assertEqual(
            _1_15_mins_bin_ago(t1),
            DateTimePoint(datetime.datetime(2020, 2, 20, 20, 0, 0))
        )
        self.assertEqual(
            _1_15_mins_bin_ago(t2),
            DateTimePoint(datetime.datetime(2020, 5, 1, 0, 45, 0))
        )

        _current_15_mins_bin = DateTimePoint.last_minutes_bin(15, 0)
        self.assertEqual(
            _current_15_mins_bin(t1),
            DateTimePoint(datetime.datetime(2020, 2, 20, 20, 15, 0))
        )
        self.assertEqual(
            _current_15_mins_bin(t2),
            DateTimePoint(datetime.datetime(2020, 5, 1, 1, 0, 0))
        )

        _2_15_mins_bin_future = DateTimePoint.last_minutes_bin(15, -2)
        self.assertEqual(
            _2_15_mins_bin_future(t1),
            DateTimePoint(datetime.datetime(2020, 2, 20, 20, 45, 0))
        )
        self.assertEqual(
            _2_15_mins_bin_future(t2),
            DateTimePoint(datetime.datetime(2020, 5, 1, 1, 30, 0))
        )

    
    def test_past_month_daterange(self):
        this_month = DateRange.past_month(0)
        d = datetime.date(2020, 2, 20)

        self.assertEqual(
            this_month(d),
            DateRange(datetime.date(2020, 2, 1), datetime.date(2020, 2, 20))
        )

        _3_months_ago = DateRange.past_month(3)
        self.assertEqual(
            _3_months_ago(d),
            DateRange(datetime.date(2019, 11, 1), datetime.date(2019, 11, 30))
        )

    def test_year_to_last_month(self):
        self.assertEqual(
            DateRange.year_to_last_month(datetime.date(2020, 2, 20)),
            DateRange(datetime.date(2020, 1, 1), datetime.date(2020, 1, 31))
        )

        self.assertEqual(
            DateRange.year_to_last_month(datetime.date(2020, 12, 31)),
            DateRange(datetime.date(2020, 1, 1), datetime.date(2020, 11, 30))
        )

        self.assertEqual(
            DateRange.year_to_last_month(datetime.date(2020, 1, 15)),
            DateRange(datetime.date(2019, 1, 1), datetime.date(2019, 12, 31))
        )
                


if __name__ == '__main__':
    unittest.main()
