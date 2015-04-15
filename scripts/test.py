#! /usr/bin/env python

import sys
import subprocess

if __name__ == "__main__":
   cc, src, sas_binary, checkers, sample_output = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5]
   compile_env = {"SA_PLUGIN": sas_binary, "SA_CHECKERS": checkers}
   process = subprocess.Popen([cc, "-c", "-std=c++11", src], stderr=subprocess.PIPE, env=compile_env)
   analyze_output = process.communicate()[1]
   if sample_output in analyze_output:
      sys.exit(0)
   else:
      sys.exit(1)
