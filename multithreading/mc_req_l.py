#!/usr/bin/python

import optparse
import fileinput
import re
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

reports = [
  # order matters !!!
  "tmp.log",
  #"bypass.log",
]


#-------------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------------

def main():
  
  #a list to store the results
  res = []
  i = 0
  for report in reports:
    if os.path.exists( report ):
      report_file = open( report )
    else:
      continue 

    report_txt = report_file.read()

#===================================================
# mc req queue size
#===================================================
    re1='(currMcReqQueueSize)'  # Alphanum 1
    re2='( )' # Any Single Character 1
    re3='(\\d+)'

    rg=re.compile(re1+re2+re3,re.IGNORECASE|re.DOTALL)
    results = rg.findall( report_txt )

    #print(len(results))

    #q_sz = int(results[i][2])
    #print(q_sz)

    #i += 1

  max_sz = 0
  q_sz = 0
  for i in range(len(results)):
    q_sz = int(results[i][2])
    if (q_sz > 10):
      print q_sz

  #print(max_sz)




main()
