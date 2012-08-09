import sys

python_version = sys.version_info[0] * 100 \
                 + sys.version_info[1] * 10 \
                 + sys.version_info[2]

if python_version > 300:
    def execfile(fname):
        exec(open(fname).read())

# pythonpath = sys.argv[1]
# testmodule = sys.argv[2]
#sys.argv = sys.argv[2:]
sys.path.append(pythonpath)
execfile(scriptfile)


