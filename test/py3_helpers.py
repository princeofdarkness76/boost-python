# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#  Py3 helpers
#

class shouldthrow:

    def __init__(self, extype, fn):
        if isinstance(extype, tuple):
            self.extype = extype
        else:
            self.extype = (extype,)
        self.fn = fn

    def __call__(self, *args, **kwargs):
        try:
            self.fn(*args, **kwargs)
        except self.extype as e:
            pass
        else:
            print("Expected a", self.extype.__name__, "exception")
            
def printer(*args):
    for x in args: print(x,)
    print()
