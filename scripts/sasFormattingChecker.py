#! /usr/bin/env python

'''
EXPERIMENTAL: this is a demonstrator and not the final product!
This script leverages clang-format to check for violations of formatting rules.
Many goodies are missing, for example selection of input clang-format
configuration and so on.
'''

import argparse
import subprocess
import difflib
import sys

gClangFormatExeName="clang-format-3.5"

def CompareFiles(filename, clangFormatOutput):
    '''
    Compare the formatted version of the file with the existing one.
    '''
    fileContentLines = open(filename,"r").read().splitlines()
    clangFormatOutputLines = clangFormatOutput.splitlines()

    diffLinesGen = difflib.context_diff(fileContentLines,
                                        clangFormatOutputLines,
                                        fromfile='Original File (%s)' %filename,
                                        tofile='Formatted File')
    diffLines = list(diffLinesGen)
    nViolations = sum(1 for line in diffLines if line[0] == '!')

    # Here we should put some mechanism to display violations as warnings to
    # integrate this script in CI systems, think to the jenkins warning parsers.
    # In addition if nViolations is greater than some number we could just print
    # an error.
    if nViolations >0 :
        print 'warning: %s violations detected.' %nViolations
        print '\n'.join(diffLines)
    return nViolations

def RunClangFormat(filename):
    '''
    Run clang-format and capture output
    '''
    process = subprocess.Popen([gClangFormatExeName,filename], stdout=subprocess.PIPE)
    result = process.communicate()[0]
    return result

def CheckFormattingRules(filename):
    '''
    Check the formatting rules
    '''
    clangFormatOutput = RunClangFormat(filename)
    nViolations = CompareFiles(filename, clangFormatOutput)


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
    violationsN = CheckFormattingRules(filename)
    sys.exit(violationsN)
