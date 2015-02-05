#! /usr/bin/env python

'''
EXPERIMENTAL: this is a demonstrator and not the final product!
This script leverages clang-format to check for violations of formatting rules.
Many goodies are missing, for example selection of input clang-format
configuration and so on.
'''

import common
import argparse
import sys

def GetArgParser():
    '''
    Prepare an argument parser
    '''

    parser = argparse.ArgumentParser()
    parser.add_argument("filename", type=str,
                         help="The file to be checked")
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = GetArgParser()
    filename = args.filename
    violationsN = common.CheckFormattingRules(filename)
    sys.exit(violationsN)
