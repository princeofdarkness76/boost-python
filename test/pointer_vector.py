# Copyright Joel de Guzman 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''

>>> import pointer_vector_ext
>>> d = pointer_vector_ext.DoesSomething()
>>> lst = d.returnList()
>>> lst[0].f();
'harru'

'''


import sys
import doctest

sys.stdout.write("running ...\n")
status = doctest.testmod(sys.modules.get(__name__))[0]
sys.stdout.write("Done\n")
sys.exit(status)



