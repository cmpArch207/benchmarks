#!/usr/bin/python
#==========================================================================
# summary.py
#==========================================================================
#
# -h --help       Display this message
# -v --verbose    Verbose mode
# -p --prefetcher Type of Prefetcher
# -s --stride     number of strides
# -1 --l1-size    size of L1 cache in KB
# -2 --l2-size    size of L2 cache in KB
# -d --directory  name of the directory to save the run
#
# Author  : Matheus Ogleari
# Date    : May 5, 2015

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
  #"sht64k2To14.log",
  #"sht64k2To16.log",
  #"sht64k2To18.log",
  #"sht64k2To20.log",
  #"sht64k2To22.log",
  #"sht128k2To14.log",
  #"sht128k2To16.log",
  #"sht128k2To18.log",
  #"sht128k2To20.log",
  #"sht128k2To22.log",
  #"sht256k2To14.log",
  #"sht256k2To16.log",
  #"sht256k2To18.log",
  #"sht256k2To20.log",
  #"sht256k2To22.log",
  #"sht512k2To14.log",
  #"sht512k2To16.log",
  #"sht512k2To18.log",
  #"sht512k2To20.log",
  #"sht512k2To22.log",
  #"sht1M2To14.log",
  #"sht1M2To16.log",
  #"sht1M2To18.log",
  #"sht1M2To20.log",
  #"sht1M2To22.log",
  #"sht2M2To14.log",
  #"sht2M2To16.log",
  #"sht2M2To18.log",
  #"sht2M2To20.log",
  #"sht2M2To22.log",
  #"sht4M2To14.log",
  #"sht4M2To16.log",
  #"sht4M2To18.log",
  #"sht4M2To20.log",
  #"sht4M2To22.log",
  ## another order
  #"sht64k2To09.log",
  #"sht128k2To09.log",
  #"sht256k2To09.log",
  #"sht512k2To09.log",
  #"sht1M2To09.log",
  #"sht2M2To09.log",
  #"sht4M2To09.log",
  #"sht8M2To09.log",
  #"sht16M2To09.log",
  #"sht64k2To13.log",
  #"sht128k2To13.log",
  #"sht256k2To13.log",
  #"sht512k2To13.log",
  #"sht1M2To13.log",
  #"sht2M2To13.log",
  #"sht4M2To13.log",
  #"sht8M2To13.log",
  #"sht16M2To13.log",
  #"sht64k2To14.log",
  #"sht128k2To14.log",
  #"sht256k2To14.log",
  #"sht512k2To14.log",
  #"sht1M2To14.log",
  #"sht2M2To14.log",
  #"sht4M2To14.log",
  #"sht8M2To14.log",
  #"sht64k2To16.log",
  #"sht128k2To16.log",
  #"sht256k2To16.log",
  #"sht512k2To16.log",
  #"sht1M2To16.log",
  #"sht2M2To16.log",
  #"sht4M2To16.log",
  #"sht64k2To18.log",
  #"sht128k2To18.log",
  #"sht256k2To18.log",
  #"sht512k2To18.log",
  #"sht1M2To18.log",
  #"sht2M2To18.log",
  #"sht4M2To18.log",
  #"sht8M2To18.log",
  #"sht64k2To20.log",
  #"sht128k2To20.log",
  #"sht256k2To20.log",
  #"sht512k2To20.log",
  #"sht1M2To20.log",
  #"sht2M2To20.log",
  #"sht4M2To20.log",
  #"sht64k2To22.log",
  #"sht128k2To22.log",
  #"sht256k2To22.log",
  #"sht512k2To22.log",
  #"sht1M2To22.log",
  #"sht2M2To22.log",
  #"sht4M2To22.log",
  #"sht1w2To18.log",
  #"sht2w2To18.log",
  #"sht4w2To18.log",
  #"sht8w2To18.log",
  #"sht16w2To18.log",
  "stm64k2To18.log",
  "stm128k2To18.log",
  "stm256k2To18.log",
  "stm512k2To18.log",
  "stm1M2To18.log",
  "stm2M2To18.log",
  "stm4M2To18.log",
  "stm8M2To18.log",
]


#-------------------------------------------------------------------------------
# Command line processing
#-------------------------------------------------------------------------------

class OptionParserWithCustomError( optparse.OptionParser ):
  def error( self, msg = "" ):
    if ( msg ): print("\n ERROR: %s" % msg )
    print("")
    for line in fileinput.input(sys.argv[0]):
      if ( not re.match( "#", line ) ): sys.exit(msg != "" )
      if ( ( fileinput.lineno() == 3 ) or ( fileinput.lineno() > 4 ) ):
        print( re.sub( "^#", "", line.rstrip( "\n" ) ) )

def parse_cmdline():
  p = OptionParserWithCustomError( add_help_option=False )
  (opts,args) = p.parse_args()
  if ( help == True ): p.error()
  if args: p.error( "found extra positional arguments" )
  return opts

#-------------------------------------------------------------------------------
# Helper Class
#-------------------------------------------------------------------------------

class Result:

  def __init__( self, l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc, cycles ):
    self.l1_RDmr       = l1_RDmr #l1 RD miss rate
    self.l1_WRmr       = l1_WRmr #l1 WR miss rate
    self.l2_RDmr       = l2_RDmr #l2 RD miss rate
    self.l2_WRmr       = l2_WRmr #l2 WR miss rate
    self.ipc           = ipc
    self.cycles = cycles
  
class Experiment:

  # bmark: the name of the benchmark (e.g., hash, sps, rbtree)
  # l1_missrate: Miss Rate for the L1 cache
  # l2_missrate: Miss Rate for the L2 Cache
  # ipc: Instructions per Cycle result
  def __init__( self, bmark ):
    self.bmark  = bmark
    self.result = None

  def print_results( self ):
    choice = self.bmark[:2]
    bmarks = {'ht': 'hashtable', 
              'ph': 'p-hashtable',
              'ps': 'parsec:blackscholes',
              'sh': 'hashtable-str',
              'da': 'double apps'}

    if (choice in bmarks):
      bm = (bmarks[choice])
    else:
      bm = 'benchmark'

    choice = self.bmark[-7:-5]
    cacheSize   = {'4k': '64k', 
                   '8k': '128k',
                   '6k': '256k',
                   '2k': '512k'}
    if (choice in cacheSize):
      cs = (cacheSize[choice])
    else:
      cs = choice

    ht_sz = 2**int(self.bmark[-2:])

    output = "%12s\t%4s\t%4s\t%4s\t%4s\t%7s\t%7s\t%.2f\t%.2f\t%10s" %( bm, cs,
                                                                    1024,
                                                                    ht_sz,
                                                     self.result.ipc,
                                                     self.result.l1_RDmr,
                                                     self.result.l1_WRmr,
                                                     self.result.l2_RDmr,
                                                     self.result.l2_WRmr,
                                                               self.result.cycles) 

    print( output )

  def set_result( self, l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc, cycles ):
    self.result = Result( l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc, cycles )


def drawGraph(array, numX):
  xs = list(range(numX))
  #print(xs)

  ys = [[0 for x in range(numX)] for x in range(len(array)/numX)]
  #print(ys)

  #generate y coordinates
  for i in range(len(array)):
    ys[i/numX][i%numX] = array[i]

  ind = np.arange(numX)
  xLab = ['64k', '128k', '256k', '512k', '1M', '2M', '4M']
  #plt.figure(1)
  #plt.figure(figsize=(20,5))
  plt.plot(xs, ys[0], label='2**14')
  plt.plot(xs, ys[1], label='2**16')
  plt.plot(xs, ys[2], label='2**18')
  plt.plot(xs, ys[3], label='2**20')
  plt.xlabel('Cache Size')
  plt.ylabel('IPC')
  plt.xticks(ind, xLab)
  plt.legend()
  plt.title('IPC')

  plt.show()

  #pp = PdfPages('htIPC.pdf')
  #pp.savefig()
  #plt.close()





#-------------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------------

def main():
  opts = parse_cmdline()

  #experiments = [Experiment] * len(reports)
  #header = "%12s\t%4s\t%7s\t%7s\t%7s\t%7st" %( "bmark", "IPC", "L1 RDMR", "L1_WRMR", "L2 RDMR", "L2 RDMR")

  header = "%12s\t%4s\t%4s\t%4s\t%4s\t%7s\t%7s\t%7s\t%7s\t%10s" %(
    "benchmark","L2Sz", "loops", "ht_sz", "IPC", "L1_RDMR","L1_WRMR", "L2_RDMR", "L2_WDMR", "cycles")

  print( header )
  
  #a list to store the results
  res = []
  for report in reports:
    experiment = Experiment( "%s" %(report[:-4]) )

    if os.path.exists( report ):
      report_file = open( report )
    else:
      continue 

    report_txt = report_file.read()

    re1='(total)' # Word 1
    re2='( )' # White Space 1
    re3='(number)'  # Word 2
    re4='( )' # White Space 2
    re5='(of)'  # Word 3
    re6='( )' # White Space 3
    re7='(fetched)' # Word 4
    re8='( )' # White Space 4
    re9='(instructions)'  # Word 5
    re10='.*?'  # Non-greedy match on filler
    re11='(\\d+)' # Integer Number 1
    re12='.*?'  # Non-greedy match on filler
    re13='([+-]?\\d*\\.\\d+)(?![-+0-9\\.])' # Float 1

    rg = re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13,re.IGNORECASE|re.DOTALL)
    ipc = rg.findall( report_txt )

    #print(ipc[0])

    ninst = ipc[0][9]
    ipc = ipc[0][10]
    ncycles = int( int( ninst ) / float( ipc ) )

    #print( ninst )
    #print( ipc )
    #print( ncycles )

#===================================================
# count L1 RD miss rate
#===================================================
    re1='(L1)'  # Alphanum 1
    re2='(\\$)' # Any Single Character 1
    re3='(D)' # White Space 1
    re4='(\\[.*?\\])' 
    re5='( )' # White Space 2
    re6='(:)' # Any Single Character 2
    re7='( )' # White Space 3
    re8='(RD)'  # Word 1
    #re8='(WR)'  # Word 1
    re9='( )' # White Space 4
    re10='(\\(.*?\\))'  # Any Single Character 3
    #re10='(\\(ab*\\))'
    #re11='.*?'  # Non-greedy match on filler
    re11='(=)'
    re12='(\\(.*?\\))'  # Any Single Character 3
    re13='(=)'
    re14='(\\s+)'
    #re15='(\\w+.\\w+%)'
    re15='([+-]?\\d*\\.\\d+)(?![-+0-9\\.])' # Float 1

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l1_results = rg.findall( report_txt )

    #print(len(l1_results))
    #for i in range(len(l1_results)):
      #print(l1_results[i])

    l1_RDmr = l1_results[0][14]
    #print(l1_RDmr)
    L1_RDMR = float(l1_RDmr)
    #print(L1_RDMR)


#===================================================
# count L1 WR miss rate
#===================================================
    re1='(L1)'  # Alphanum 1
    re2='(\\$)' # Any Single Character 1
    re3='(D)' # White Space 1
    re4='(\\[.*?\\])' 
    re5='( )' # White Space 2
    re6='(:)' # Any Single Character 2
    re7='( )' # White Space 3
    re8='(WR)'  # Word 1
    #re8='(WR)'  # Word 1
    re9='( )' # White Space 4
    re10='(\\(.*?\\))'  # Any Single Character 3
    #re10='(\\(ab*\\))'
    #re11='.*?'  # Non-greedy match on filler
    re11='(=)'
    re12='(\\(.*?\\))'  # Any Single Character 3
    re13='(=)'
    re14='(\\s+)'
    #re15='(\\w+.\\w+%)'
    re15='([+-]?\\d*\\.\\d+)(?![-+0-9\\.])' # Float 1

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l1_results = rg.findall( report_txt )

    #print(len(l1_results))
    #for i in range(len(l1_results)):
      #print(l1_results[i])

    l1_WRmr = l1_results[0][14]
    #print(l1_WRmr)
    L1_WRMR = float(l1_WRmr)
#===================================================
# count L2 RD miss rate
#===================================================

    re1='(L2)'  # Alphanum 1
    re2='(\\$)' # Any Single Character 1
    re3='( )' # White Space 1
    re4='(\\[.*?\\])' 
    re5='( )' # White Space 2
    re6='(:)' # Any Single Character 2
    re7='( )' # White Space 3
    re8='(RD)'  # Word 1
    re9='( )' # White Space 4
    re10='(\\(.*?\\))'  # Any Single Character 3
    re11='(=)'
    re12='(\\(.*?\\))'  # Any Single Character 3
    re13='(=)'
    re14='(\\s+)'
    #re15='(\\w+.\\w+%)'
    re15='([+-]?\\d*\\.\\d+)(?![-+0-9\\.])' # Float 1

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)


    l2_results = rg.findall( report_txt )

    #print(len(l2_results))
    #for i in range(len(l2_results)):
      #print(l2_results[i])

    l2_RDmr = l2_results[0][14]
    #print(l2_RDmr)
    L2_RDMR = float(l2_RDmr)

    #get the global miss rate
    L2_RDMR = L2_RDMR*L1_RDMR/100


#===================================================
# count L2 WR miss rate
#===================================================
    re1='(L2)'  # Alphanum 1
    re2='(\\$)' # Any Single Character 1
    re3='( )' # White Space 1
    re4='(\\[.*?\\])' 
    re5='( )' # White Space 2
    re6='(:)' # Any Single Character 2
    re7='( )' # White Space 3
    re8='(WR)'  # Word 1
    re9='( )' # White Space 4
    re10='(\\(.*?\\))'  # Any Single Character 3
    re11='(=)'
    re12='(\\(.*?\\))'  # Any Single Character 3
    re13='(=)'
    re14='(\\s+)'
    #re15='(\\w+.\\w+%)'
    re15='([+-]?\\d*\\.\\d+)(?![-+0-9\\.])' # Float 1

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l2_results = rg.findall( report_txt )

    #print(len(l2_results))
    #for i in range(len(l2_results)):
      #print(l2_results[i])

    l2_WRmr = l2_results[0][14]
    #print(l2_WRmr)

    #get the global miss rate
    L2_WRMR = float(l2_WRmr)
    L2_WRMR = L2_WRMR*L1_WRMR/100
   


    res.append(ipc)
      # Checks for result results in all 5 bmarks
    experiment.set_result(  L1_RDMR, L1_WRMR, L2_RDMR, L2_WRMR, ipc, ncycles)
    experiment.print_results()

  #drawGraph(res, 7)


main()
