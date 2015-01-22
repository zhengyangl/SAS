'''
EXPERIMENTAL: this is a demonstrator and not the final product!
This module provides the common pieces for scripts wrapping around clang(++)
invocations which allows to invoke static analysis if certain environment
variables are set.
Tested with ROOT

In order to steer SA within your setup (your laptop, Jenknins or whatever CI
system) the following variables must be defined:
o SA_CHECKERS: list of checkers names separated by a ":", a la PATH.
  Example: SA_CHECKERS="core.uninitialized:cplusplus"
o SA_CLANG_CPP: path to clang++. This is the compiler to be called to perform
  static analysis.
  Example: SA_CLANG_CPP=/afs/cern.ch/sw/blaBlaBla/clang35/blaBlaBla/bin/clang++
o SA_PLUGIN: plugin library of custom checkers, for example built with SAS.
  https://indico.cern.ch/event/267747/contribution/2/1/material/slides/0.pdf
  Example: SA_PLUGIN="/path/to/my/libSas.so"
'''

import subprocess
import os

def WrapCommand(command, SA_CLANG_COMPILER):
   '''
   Wrap the compilation command and force clang to run static analysis.
   Invoke SA and compilation
   '''
   SA_CHECKERS = "SA_CHECKERS"
   SA_PLUGIN = "SA_PLUGIN"
   BASE_XCLANG_ARGS = ["-Xclang", "-analyze", "-Xclang", "-analyzer-output=text"]
   compilerArguments = command[1:]
   if "-c" in compilerArguments and not "-shared" in compilerArguments:
      env = os.environ
      sa_checkersString = env[SA_CHECKERS] if env.has_key(SA_CHECKERS) else ""
      sa_checkers = sa_checkersString.split(":")
      sa_plugin = env[SA_PLUGIN] if env.has_key(SA_PLUGIN) else ""

      # Accumulate checkers names
      checkers_args=[]
      for checkerName in sa_checkers:
         checkers_args += ["-Xclang", "-analyzer-checker=%s" %checkerName]

      # Load the plugin if one has been specified
      plugin_args=[]
      if sa_plugin != "":
         plugin_args += ["-Xclang", "-load", "-Xclang", "%s" %sa_plugin]


      outputToNull = ["-o","/dev/null"]

      # Recompose the SA command
      saCommand = [SA_CLANG_COMPILER] + \
                  BASE_XCLANG_ARGS + \
                  checkers_args + \
                  plugin_args + \
                  compilerArguments + \
                  outputToNull

      print "Invoking SA with command %s " %" ".join(saCommand)
      subprocess.call(saCommand)

   return subprocess.call(command)
