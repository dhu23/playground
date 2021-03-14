import asyncio

async def say(what, when):
    await asyncio.sleep(when)
    print(what)         # can be interpreted as a function of what -> None

# in JavaScript it would be something like
# setTimeout(function (what) { console.log(what); }, when)


loop = asyncio.get_event_loop()
print('got event loop')
loop.run_until_complete(say('hello world', 1)) # blocks until done
print('done run_until_complete')
loop.close()
