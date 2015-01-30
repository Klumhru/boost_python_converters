#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Setup for the OR ws client
"""
from __future__ import unicode_literals, print_function, absolute_import

import sys

from distutils.core import setup
from setuptools import find_packages, Extension


def parallel_compile(self, sources, output_dir=None, macros=None,
                     include_dirs=None, debug=0, extra_preargs=None,
                     extra_postargs=None, depends=None):
    '''Hack for parallel compilation in distutils'''
    # those lines are copied from distutils.ccompiler.CCompiler directly
    macros, objects, extra_postargs, pp_opts, build = \
        self._setup_compile(output_dir, macros, include_dirs, sources, depends,
                            extra_postargs)
    cc_args = self._get_cc_args(pp_opts, debug, extra_preargs)
    # parallel code
    cores = 4  # number of parallel compilations
    import multiprocessing.pool

    def _single_compile(obj):
        try:
            src, ext = build[obj]
        except KeyError:
            return
        self._compile(obj, src, ext, cc_args, extra_postargs, pp_opts)
    # convert to list, imap is evaluated on-demand
    list(multiprocessing.pool.ThreadPool(cores).imap(_single_compile, objects))
    return objects


if '-j' in sys.argv:
    sys.argv.remove('-j')
    import distutils.ccompiler
    distutils.ccompiler.CCompiler.compile = parallel_compile

sources = [
    str('src/boost_python_converters.cpp'),
]

ext_modules = [
    Extension(str('boost_python_converters'),
              sources=sources,
              define_macros=[('DEBUG', '1',)],
              libraries=['boost_python',
                         'boost_date_time'],
              include_dirs=['src/', 'include/'],
              extra_compile_args=['-w', '-std=c++11'])
]

requires = ['python-dateutil==1.5',
            'nose',
            'ipython']

setup(
    name='boost_python_converters',
    version='0.1.1',
    packages=find_packages(),
    author="Högni Gylfason",
    author_email="klumhru@gmail.com",
    install_requires=requires,
    description=__doc__,
    include_package_data=True,
    classifiers=[
        "Development Status :: 2 - Beta",
        "Intended Audience :: Boost Python Developers",
        "Licence :: MIT",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: C++",
    ],
    zip_safe=False,
    ext_modules=ext_modules,
)
