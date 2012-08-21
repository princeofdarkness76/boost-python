#!/usr/bin/env python
#
# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# A script that runs doctests that works on python 2.2 and up.
# The catch:  only do portable stuff in the doctests.
#
# Usage:  bpl_testdriver.py [pythonpath] [testname|testname.py]
# Usage:  bpl_testdriver.py [testname|testname.py]
#
# Examples:
#
#    bpl_testdriver.py /path/to/build/lib ambig1.py
#    bpl_testdriver.py /path/to/build/lib ambig1
#    bpl_testdriver.py ambig1.py
#    bpl_testdriver.py ambig1
#
import sys

python_version = sys.version_info[0] * 100 \
                 + sys.version_info[1] * 10 \
                 + sys.version_info[2]

if python_version < 300:
    import py2_helpers as helpers
else:
    import py3_helpers as helpers

#
# this used later on
#
argv0 = sys.argv[0]

#
# we don't need the name of this script
#
sys.argv = sys.argv[1:]

#
# If there are two arguments this one is pythonpath,
# add it to path
#
if len(sys.argv) == 2:
    sys.path.append(sys.argv[0])
    sys.argv.pop(0)

#
# Next arg is module to import, strip the .py if it is there
#
if sys.argv[0].endswith('.py'):
    testmodulename = sys.argv[0].rstrip('.py')
else:
    testmodulename = sys.argv[0]
sys.argv.pop(0)

globs = { 'shouldthrow' : helpers.shouldthrow,
          'python_version' : python_version,
          'printer' : helpers.printer }

print "Importing", testmodulename 
if python_version < 300:
    try:
        testmodule = __import__(testmodulename)
    except :
        try:
            testmodule = __import__(testmodulename+"_ext")
        except Exception as e:
            print("Ooops... couldn't import testing module "\
                  "(python, not shared library) %s to driver" % testmodulename)
            raise e
else:
    try:
        testmodule = __import__(testmodulename)
    except :
        try:
            testmodule = __import__(testmodulename+"_ext")
        except e:
            print("Ooops... couldn't import testing module "\
                  "(python, not shared library) %s to driver" % testmodulename)
            raise e

def main(docstring):
    if python_version < 300:
        #
        #  exception types are printed fully qualified in py3k
        #
        docstring = docstring.replace('Boost.Python.ArgumentError', 'ArgumentError')
        docstring = docstring.replace('Boost.Python.ArgumentError', 'ArgumentError')
        from doctest import Tester
        t = Tester(globs=globs)
        (failures, tries) = t.runstring(docstring, argv0)
        t.summarize(verbose=1)
        sys.exit(failures > 0)
    else:
        import doctest
        parser = doctest.DocTestParser()
        dt = parser.get_doctest(docstring,
                                globs=globs,
                                name=argv0,
                                filename=None,
                                lineno=None)
        runner = doctest.DocTestRunner(verbose=1)
        runner.run(dt)
        (failed, attempted) = runner.summarize(verbose=True)
        sys.exit(failed > 0)

if 'should_run' in testmodule.__dict__:
    if not testmodule.should_run(python_version):
        print("Skipping test on python_version = %d" % python_version)
        sys.exit(0)
        
if testmodule.__doc__: # will be none if no docstring
    print("Running doctest...")
    main(testmodule.__doc__)
elif 'main' in testmodule.__dict__:
    print("Running main()...")
    sys.argv = [argv0] + sys.argv
    testmodule.main()
else:
    print("neither docstring nor main() found in testmodule")
    sys.exit(1)
print("Done.")
