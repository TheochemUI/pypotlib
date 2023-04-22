# Kanged from SciPy: https://github.com/scipy/scipy/tree/main/tools/wheels
set -xe

PROJECT_DIR="$1"
PLATFORM=$(PYTHONPATH=wheel_tools python -c "import openblas_support; print(openblas_support.get_plat())")

printenv
# Update license
cat $PROJECT_DIR/wheel_tools/LICENSE_linux.txt >> $PROJECT_DIR/LICENSE.txt

# Install Openblas
basedir=$(python wheel_tools/openblas_support.py)
cp -r $basedir/lib/* /usr/local/lib
cp $basedir/include/* /usr/local/include
