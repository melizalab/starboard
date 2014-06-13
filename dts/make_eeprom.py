# -*- coding: utf-8 -*-
# -*- mode: python -*-
"""Create EEPROM binary for Beaglebone Black cape

Copyright (C) 2014 Dan Meliza <dan@meliza.org>
Created Fri 13 Jun 2014 10:27:22 AM EDT
"""

# python 3 compatibility
from __future__ import absolute_import
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals

import struct
import json
import argparse

if __name__ == '__main__':
    p = argparse.ArgumentParser()
    p.add_argument('json', help="cape EEPROM data in JSON format")
    p.add_argument('output', help="output file (will be overwritten)")
    p.add_argument('--serial','-s', help="serial number (overrides value in json file)")
    args = p.parse_args()

    js = json.load(open(args.json, 'rU'))

    with open(args.output, 'wb') as fp:
        fp.write(struct.pack(b">4s2s32s4s16s16sh12s148shhhh",
                             b"\xAA\x55\x33\xEE", # magic
                             b"A1", # revision
                             bytes(js['board']),
                             bytes(js['revision']),
                             bytes(js.get('manufacturer', "")),
                             bytes(js['part_number']),
                             js.get('pin_count', 0),
                             # serial should be WWYY&&&&NNNN
                             # TODO get week and year
                             bytes(args.serial or js.get('serial', "")),
                             b"", # pins - not used
                             js.get('3v3_current', 0),
                             js.get('vdd_current', 0),
                             js.get('sys_current', 0),
                             js.get('dc_supplied', 0),
                         ))
        fp.write(b"\x00" * 32543)



# Variables:
# End:
