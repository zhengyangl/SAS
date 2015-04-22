#! /usr/bin/env python

import sys
import os
import subprocess
import difflib

def getBasePath(sas_plugin):
   """
   Get the path specific to the SAS installation
   """
   scriptPath = os.path.realpath(__file__)
   return scriptPath.replace("/scripts/test.py","")

def cleanOutput(specificPath,analyze_output):
   return analyze_output.replace(specificPath,"")

def printDiff(str1,str2):
   diff = difflib.ndiff(str1.splitlines(1), str2.splitlines(1))
   print ''.join(diff),

def runTest():
   cc, src, sas_plugin, checkers, refOutputName = sys.argv[1:]
   basePath = getBasePath(sas_plugin)

   compile_env = {"SA_PLUGIN": sas_plugin, "SA_CHECKERS": checkers}
   process = subprocess.Popen([cc, "-c", "-std=c++11", src], stderr=subprocess.PIPE, env=compile_env)

   analyze_output = process.communicate()[1]
   clean_analyze_output = cleanOutput(basePath, analyze_output)

   refOutput = open(refOutputName).read()

   if refOutput == clean_analyze_output:
      return 0
   else:
      printDiff(refOutput,clean_analyze_output)
      return 1

if __name__ == "__main__":
   exitCode = runTest()
   sys.exit(exitCode)
