from setuptools import setup
from setuptools import Extension

setup(
    name='arch-lib',
    version='1.0',
    description='Example Extension for Python 2.7',
    author='VA0',
    maintainer='Filip',
    url='https://github.com/VA0/ArchPythonExtension',
    ext_modules=[Extension('arch', ['arch.cpp', 'udp.cpp'])],
)