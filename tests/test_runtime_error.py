#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
"""
from __future__ import unicode_literals, print_function, absolute_import

from unittest import TestCase

import boost_python_converters


class TestRuntimeError(TestCase):

    def setUp(self):  # noqa
        pass

    def test_ctor(self):
        """
        std::runtime_error is not usually constructed from python so we cheat
        on our unicode rule
        """
        e = boost_python_converters.RuntimeError(b'Oh noes!')
        self.assertEqual(str(e), b'Oh noes!')

    def tearDown(self):  # noqa
        pass
