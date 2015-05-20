"""
Unit tests.
"""
import sys
BUILD_DIR = sys.argv[-1]
sys.path.append(BUILD_DIR)

import os
import subprocess
import pytest


def test_leaks():
    """
    Test memory leaks.
    """
    res = subprocess.Popen(
        [
            'valgrind',
            os.path.join(BUILD_DIR, 'bin', 'unit_tests')
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE).communicate()[1]
    if not 'All heap blocks were freed -- no leaks are possible' in res:
        assert 'definitely lost: 0 bytes in 0 blocks' in res
        assert 'indirectly lost: 0 bytes in 0 blocks' in res
        assert 'possibly lost: 0 bytes in 0 blocks' in res
