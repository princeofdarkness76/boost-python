# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from dict_ext import *
>>> new_dict()
{}
>>> data_dict()
{1: {'key2': 'value2'}, 'key1': 'value1'}
>>> tmp = data_dict()
>>> dict_keys(tmp)
[1, 'key1']
>>> dict_values(tmp)
[{'key2': 'value2'}, 'value1']
>>> dict_items(tmp)
[(1, {'key2': 'value2'}), ('key1', 'value1')]
>>> dict_from_sequence([(1,1),(2,2),(3,3)])
{1: 1, 2: 2, 3: 3}
>>> test_templates(printer) #doctest: +NORMALIZE_WHITESPACE
a test string
13
None
{1.5: 13, 1: 'a test string'}
default
default
'''

