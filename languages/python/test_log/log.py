import logging
import logging.handlers
import time

LOG_FILE_NAME = 'test_logging.log'

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

handler = logging.handlers.RotatingFileHandler(
    LOG_FILE_NAME, maxBytes=20, backupCount=3)

logger.addHandler(handler)

for i in range(20):
    logger.debug('i = %d' % i)
    time.sleep(1)
