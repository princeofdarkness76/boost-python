# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# A script that runs doctests that works on python 2.2 and up.
# The catch:  only do portable stuff in the doctests.
#
import sys

python_version = sys.version_info[0] * 100 \
                 + sys.version_info[1] * 10 \
                 + sys.version_info[2]

if python_version < 300:
    import py2_helpers as helpers
else:
    import py3_helpers as helpers

pythonpath = sys.argv[1]
testmodulename = sys.argv[2]
sys.argv = sys.argv[2:]
sys.path.append(pythonpath)

testmodule = __import__(testmodulename)


shouldthrow = helpers.shouldthrow

globs = {'shouldthrow' : shouldthrow,
         'python_version' : python_version }

def main(docstring):
    if python_version < 300:
        #
        #  exception types are printed fully qualified in py3k
        #
        docstring = docstring.replace('Boost.Python.ArgumentError', 'ArgumentError')
        docstring = docstring.replace('Boost.Python.ArgumentError', 'ArgumentError')
        from doctest import Tester
        t = Tester(globs=globs)
        (failures, tries) = t.runstring(docstring, sys.argv[0])
        t.summarize(verbose=1)
        sys.exit(failures > 0)
    else:
        import doctest
        parser = doctest.DocTestParser()
        dt = parser.get_doctest(docstring,
                                globs=globs,
                                name=sys.argv[0],
                                filename=None,
                                lineno=None)
        print(dt)
        runner = doctest.DocTestRunner(verbose=1)
        runner.run(dt)
        (failed, attempted) = runner.summarize(verbose=True)
        sys.exit(failed > 0)
        # doctest.testfile(scriptfile, module_relative=False)


if s in testmodule:
    if isinstance(testmodule.s, string):
        main(string)
elif main in testmodule:
    if isinstance(testmodule.main, function):
        testmodule.main(sys.argv)
else:
    raise RuntimeError("neither doctest string nor main found in module")


