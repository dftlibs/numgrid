"""
Test memory leaks.
"""
import os
import sys
import subprocess
import pytest

BUILD_DIR = sys.argv[-1]

# ------------------------------------------------------------------------------


def test_leaks():
    """
    Test memory leaks.
    """
    res = subprocess.Popen(
        [
            'valgrind',
            os.path.join(BUILD_DIR, 'bin', 'cpp_test')
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE).communicate()[1]
    if 'All heap blocks were freed -- no leaks are possible' not in res:
        assert 'definitely lost: 0 bytes in 0 blocks' in res
        assert 'indirectly lost: 0 bytes in 0 blocks' in res
        assert 'possibly lost: 0 bytes in 0 blocks' in res
