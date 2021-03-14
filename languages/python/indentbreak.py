config = [

    JobSpec(
        name='name1',
        trigger=None, 
        jobs=[
            DataJob(
                name='dummy1', acquisition='dummy2',
                env='prod', dtfunc=None),
        ],
        deliveries=None, 
    ),

    JobSpec(
        name='name2',
        trigger=None, 
        jobs=[
            DataJob(
                name='dummy2', acquisition='',
                env='prod', dtfunc=None),
        ],
        deliveries=[
            Delivery(
                files={
                    'fil1': Template('file1_$date.csv'),
                    'fil2': Template('file2_$date.csv'),
                },
                methods=[
                    FileDrop(
                        user='usr1', host='host1',
                        path='path1'
                    ),
                    FileDrop(
                        user='usr2', host='host2',
                        path='path2'
                    ),
                ]
            ),
            Delivery(
                files={
                    'file1': Template('file1_$date.csv'),
                },
                methods=[
                    Email(
                        sender=[myself],
                        receiver=['usr1@client1.com'],
                        cc=None,
                        bcc=[myself],
                        sub=Template('Report $dmonth/$dday'),
                        msg="Please review files attached."
                    )
                ],
            ),
        ]
    ),

    JobSpec(
        name='name3',
        trigger=None,
        jobs=[
            DataJob(
                name='dummy11', acquisition=None,
                env='prod', dtfunc=_today),
            DataJob(
                name='dummy12', acquisition=None, 
                env='prod', dtfunc=_today),
            DataJob(
                name='dummy13', acquisition=None,
                env='prod', dtfunc=_today),
        ],
        deliveries=[
            Delivery(
                files={
                    'file1': Template('${date}_file1.csv'),
                    'file2': Template('${date}_file2.csv'),
                    'file3': Template('${date}_file3'),
                },
                methods={
                    'file1': Template('${date}_file1.csv'),
                    'file2': Template('${date}_file2.csv'),
                    'file3': Template('${date}_file3'),
                    'file3': Template('${date}_file3'),
                }
            ),
        ]
    ),

    JobSpec(
        name='name4',
        trigger=None, 
        jobs=[],
        deliveries=None,
    ),

    Email(
        sender=[myself],
        receiver=[
            'user2@client2.com', 'user3@client3.com',
            'user4@client2.com'
        ],
        cc=None,
        bcc=None,
        sub=Template('Files ${dyear}-${dmonth}-${dday}'),
        msg=Template('Hello,\n\nPlease find report as of ${thour}:${tminute}:${tsecond} on ${dyear}-${dmonth}-${dday}.')
    ),






]



