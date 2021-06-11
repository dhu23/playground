import pandas as pd

from core.computation import (
    Hierarchy,
    make_custom_computation,
    RRowAdjust,
    RColumnAdjust,
    SelectBuilder,
    RQuery, 
)

from core.report import (
    Reporter,
    ReportSpec,
    ReportNode,
)

from core.data import (
    DataModel,
    DataQuery,
    DSelect,
    DSequence, 
    DJoin,
    TableMeta, 
    make_foreignkey,
    make_stub_data_fetcher,
    Acquisition,
)


################### Stub data and Data acquisition objects #################

_data = {
    'orders': pd.DataFrame({
        'order_id': [1, 2, 3, 4, 5, 6],
        'ticker': ['AAPL', 'FB', 'FB', 'MSFT', 'IBM', 'AAPL'],
        'price': [276.0, 176.5, 175.0, 169.0, 115.0, 275.5],
        'qty': [120, 3, 20, 25, 1, 5],
        'side': ['B', 'S', 'S', 'S', 'B', 'B'],
        'client': ['A', 'A', 'P', 'S', 'P', 'A']
    }),
    'stocks': pd.DataFrame({
        'ticker': ['AAPL', 'FB', 'MSFT', 'IBM', 'CSCO'],
        'company': ['Apple', 'Facebook', 'Microsoft', 'IBM', 'Cisco'],
    })
}


TestFetcher = make_stub_data_fetcher('TestFetcher', _data)


_test_data_model_acq = Acquisition(
    querygen=DataModel([
        TableMeta(
            name='orders',
            columns=('order_id', 'ticker', 'price', 'qty', 'side', 'client'),
            foreign=[
                make_foreignkey(table='stocks', column='ticker'),
            ]
        ),
        TableMeta(
            name='stocks',
            columns=('ticker', 'company'),
            foreign=None
        )
    ]), 
    fetchergen=TestFetcher,
)


_test_data_query_acq = Acquisition(
    querygen=DataQuery(
        DSequence([
            DJoin(
                source=DSelect(
                    table='orders',
                    columns=['order_id', 'ticker', 'price', 'qty', 'side', 'client'],
                    transforms=None,
                ),
                joinby=None
            ),
            DJoin(
                source=DSelect(
                    table='stocks', columns=['ticker', 'company'], transforms=None
                ),
                joinby=['ticker']
            ),
        ])
    ),
    fetchergen=TestFetcher,
)


_acquisitions = {
    'test_data_model': _test_data_model_acq,
    'test_data_query': _test_data_query_acq,
}


##################### Computation hierarchy information ####################
_standard_hier = Hierarchy([
    make_custom_computation(RRowAdjust, 'value', 'price*qty', ('price', 'qty')),
    RColumnAdjust(name='total_value', func=sum, deps='value'),
    RColumnAdjust(name='count', func=lambda x : len(x.index), deps=None),
])


_hierarchies = {
    'test_standard_hier' : _standard_hier,
}


############################# Report configuration ##########################
    
_reports = {}

_reports['A'] = ReportSpec(
    node=ReportNode(
        tasks=[
            RQuery(data={
                'select': [
                    {'name': 'order_id'}, 
                    {'name': 'ticker'}, 
                    {'name': 'value'}
                ],
                'filterby': [
                    {'name': 'client', 'fn': 'equal_to', 'arg': 'A'},
                ]
            }),
        ],
        output='aqr_report', 
        subnodes=[
            ReportNode(
                tasks=[
                    RQuery(data={
                        'select': [
                            {'name': 'ticker'}, 
                            {'name': 'total_value'},
                            {'name': 'count'}
                        ],
                        'groupby': 'ticker'
                    }),
                    RRowAdjust(
                        name='total_value', 
                        func=int,
                        deps='total_value'
                    ),
                ],
                output='aqr_ticker_report',
                subnodes=None
            ),
        ]
    ),
    combine=None
)

_reports['P'] = ReportSpec(
    node=ReportNode(
        tasks=[
            RQuery(select=[
                SelectBuilder('order_id', None),
                SelectBuilder('ticker', None), 
                SelectBuilder('price', None),
                SelectBuilder('qty', None),
                SelectBuilder('side', None),
                SelectBuilder('company', None),
            ]).filterby(name='client', arg='P'),
        ],
        output='pimco_report',
        subnodes=None
    ),
    combine=None
)

########################## create reporter instance ##########################

test_reporter = Reporter(_acquisitions, _hierarchies, _reports, None) #'test_cache')

