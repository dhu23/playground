import unittest
import pandas as pd
from string import Template
import datetime
import logging

from core import util
from core.report import *
from core.data import (
    AcquisitionKey,
    DateRange, 
    DateTimePoint,
)
from reporter_testdata import test_reporter

class TestReportContext(unittest.TestCase):

    def setUp(self):
        self.rc = make_report_context(
            AcquisitionKey(
                acquisition='a', 
                env='env', 
                srcdt=DateRange(
                    datetime.date(2020, 2, 20),
                    datetime.date(2020, 2, 20)
                )
            ),
            'b', 
            'c', 
            datetime.datetime(2020, 2, 20, 15, 6, 34, 123456),
            ctxt1='ONE',
            ctxt2='TWO',
        )


    #def test_jsonlike(self):
    #    jsonlike_ans = {
    #        'acquisition_name': 'a', 
    #        'hierarchy_name': 'b',
    #        'report_spec_name': 'c',
    #        'env': 'env', 
    #        'dt_str': '20200220', 
    #        'dtp': DateTimePoint(datetime(2020, 2, 20, 15, 6, 34)),
    #        'ts': datetime(2020, 2, 20, 15, 6, 34),
    #        'ctxt1': 'ONE',
    #        'ctxt2': 'TWO',
    #        'dyear': '2020', 
    #        'dmonth': '02',
    #        'dday': '20', 
    #        'thour': '15',
    #        'tminute': '06', 
    #        'tsecond': '34',
    #    }
    #    self.assertEqual(util.to_jsonlike(self.rc), jsonlike_ans)

        
    def test_contextual_make_str(self):
        
        def _make_str_func(context):
            return 'date-%s-%s-%s' % (
                context.asofyear, context.asofmonth, context.asofday)
            
        
        self.assertEqual(make_str(_make_str_func, self.rc), 'date-2020-02-20')
        self.assertEqual(make_str('a dummy string', self.rc), 'a dummy string')
        self.assertEqual(make_str(Template('year-${asofyear}'), self.rc), 'year-2020')

    
    def test_contextual_filters(self):
        # test against 'contextual equal to'
        _equal_to_ctxt1 = util.contextual_equal_to('ctxt1')

        self.assertTrue(_equal_to_ctxt1(self.rc)('ONE'))
        self.assertFalse(_equal_to_ctxt1(self.rc)('TWO'))

        _equal_to_ctxt2 = util.contextual_equal_to('ctxt2', lambda x: x.lower())
        self.assertTrue(_equal_to_ctxt2(self.rc)('two'))


        _time_after_ctxt_asof = util.contextual_time_after(
            'asof', False, False, lambda x: x.time())
        self.assertTrue(_time_after_ctxt_asof(self.rc)(pd.Timestamp('2020-02-01 16:00:00')))
        self.assertFalse(_time_after_ctxt_asof(self.rc)(pd.Timestamp('2010-05-10 10:00:00')))


def _verify_report(ut, context, report_answers, data_answer=None):

    # print('verifying ', context)
    ret = test_reporter.report(context)
    # print('calculated data:\n', ret.cache.data)

    if data_answer is not None:
        # print('should be:\n', data_answer)
        util.assert_dataframe(ut, ret.cache.data, data_answer)

    for report_name, report_ans in report_answers.items():
        ut.assertTrue(report_name in ret.reports)
        _report = ret.reports[report_name]

        # print('checking report data:', report_name)
        # print('calculated:\n', _report.data)
        # print('should be:\n', report_ans)
        util.assert_dataframe(ut, _report.data, report_ans)


class TestReport(unittest.TestCase):

    def test_report_aqr(self):

        #print(util.to_jsonlike(test_reporter.reports), '\n')

        #_data_model_ans = pd.DataFrame({
        #    'client': ['A', 'A', 'P', 'S', 'P', 'A'],
        #    'qty': [120, 3, 20, 25, 1, 5],
        #    'ticker': ['AAPL', 'FB', 'FB', 'MSFT', 'IBM', 'AAPL'],
        #    'order_id': [1, 2, 3, 4, 5, 6],
        #    'price': [276.0, 176.5, 175.0, 169.0, 115.0, 275.5]
        #})

        _data_query_ans = pd.DataFrame({
            'client': ['A', 'A', 'P', 'S', 'P', 'A'],
            'qty': [120, 3, 20, 25, 1, 5],
            'ticker': ['AAPL', 'FB', 'FB', 'MSFT', 'IBM', 'AAPL'],
            'order_id': [1, 2, 3, 4, 5, 6],
            'price': [276.0, 176.5, 175.0, 169.0, 115.0, 275.5],
            'side': list('BSSSBB'),
            'company': ['Apple', 'Facebook', 'Facebook', 'Microsoft', 'IBM', 'Apple'],
        })

        _report_answers = {
            'aqr_report': pd.DataFrame({
                'order_id': [1, 2, 6],
                'ticker': ['AAPL', 'FB', 'AAPL'],
                'value': [33120.0, 529.5, 1377.5]
            }),
            'aqr_ticker_report': pd.DataFrame({
                'ticker': ['AAPL', 'FB'],
                'total_value': [int(34497.5), int(529.5)],
                'count': [2, 1],
            }),
        }

        #_verify_report(self, 
        #               make_report_context(
        #                   acquisition_name='test_data_model',
        #                   hierarchy_name='test_standard_hier',
        #                   report_spec_name='A',
        #                   env='env',
        #                   dt_str='20200101',
        #               ),
        #               _report_answers, _data_model_ans)

        _verify_report(self, 
                       make_report_context(
                           AcquisitionKey(
                               acquisition='test_data_query', 
                               env='env', 
                               srcdt=DateRange(
                                   datetime.date(2020, 1, 1),
                                   datetime.date(2020, 1, 1)
                               )
                           ),
                           'test_standard_hier', 
                           'A',
                           datetime.datetime(2020, 2, 20, 12, 0, 0)
                       ),
                       _report_answers, _data_query_ans)


    def test_report_pimco(self):

        _report_answers = {
            'pimco_report': pd.DataFrame({
                'order_id': [3, 5],
                'ticker': ['FB', 'IBM'],
                'price': [175.0, 115.0],
                'qty': [20, 1],
                'side': ['S', 'B'], 
                'company': ['Facebook', 'IBM'],
            })
        }

        _verify_report(self,
                       make_report_context(
                           AcquisitionKey(
                               acquisition='test_data_query',
                               env='env', 
                               srcdt=DateRange(
                                   datetime.date(2020, 1, 1),
                                   datetime.date(2020, 1, 1)
                               )
                           ),
                           'test_standard_hier', 
                           'P',
                           datetime.datetime(2020, 2, 20, 12, 0, 0)
                       ),
                       _report_answers)


if __name__ == '__main__':
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    logger.addHandler(logging.StreamHandler())
    unittest.main()
