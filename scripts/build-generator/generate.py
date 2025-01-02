FILENAME_BUILDNO = 'scripts/build-generator/last'
FILENAME_VERSION_H = 'lib/Configuration/Build.h'

import datetime

build_no = 0
try:
    with open(FILENAME_BUILDNO) as f:
        build_no = int(f.readline()) + 1
except:
    print('Starting build number from 1..')
    build_no = 1
with open(FILENAME_BUILDNO, 'w+') as f:
    f.write(str(build_no))
    print('Build number: {}'.format(build_no))

hf = """
#ifndef _Configuration_Build_h
#define _Configuration_Build_h
#define VERSION_BUILD_NUMBER {}
#define VERSION_BUILD_DATE "{}"
#endif  
""".format(build_no, datetime.datetime.now())
with open(FILENAME_VERSION_H, 'w+') as f:
    f.write(hf)