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

reports = [
  "ht.log", 
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

  #def __init__( self, l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc, ops, cycles, pre_reqs, mem_rd, mem_wr ):
  def __init__( self, l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc ):
    self.l1_RDmr       = l1_RDmr #l1 RD miss rate
    self.l1_WRmr       = l1_WRmr #l1 WR miss rate
    self.l2_RDmr       = l2_RDmr #l2 RD miss rate
    self.l2_WRmr       = l2_WRmr #l2 WR miss rate
    self.ipc           = ipc
    #self.pre_reqs      = pre_reqs
    #self.ops           = ops
    #self.op_throughput = float( ops ) / float( cycles )
    #self.mem_rd        = mem_rd
    #self.mem_wr        = mem_wr
  
class Experiment:

  # bmark: the name of the benchmark (e.g., hash, sps, rbtree)
  # l1_missrate: Miss Rate for the L1 cache
  # l2_missrate: Miss Rate for the L2 Cache
  # ipc: Instructions per Cycle result
  def __init__( self, bmark ):
    self.bmark  = bmark
    self.result = None

  def print_results( self ):
    #output = "%13s\t\t%60s\t\t\t%60s\n"                                                     %( "",              "No Persistent Memory",                                                                                                                                     "Persistent Memory" )  \
    #       + "%13s\t%8s%8s\t%4s\t%13s\t%13s\t%7s\t%7s\t%8s%8s\t%4s\t%13s\t%13s\t%7s\t%7s\n" %( self.bmark,      "L1 MR",                 "L2 MR",                 "IPC",           "Op Throughput",           "Prefetch Reqs",      "Mem RD",           "Mem WR",           "L1 MR",                "L2 MR",                "IPC",          "Op Throughput",          "Prefetch Reqs",     "Mem RD",          "Mem WR"          ) \
    #       + "%13s\t%8s%8s\t%4s\t%13e\t%13s\t%7d\t%7d\t%8s%8s\t%4s\t%13e\t%13s\t%7d\t%7d\n" %( "No Prefetcher", self.npmnpf.l1_missrate, self.npmnpf.l2_missrate, self.npmnpf.ipc, self.npmnpf.op_throughput, self.npmnpf.pre_reqs, self.npmnpf.mem_rd, self.npmnpf.mem_wr, self.pmnpf.l1_missrate, self.pmnpf.l2_missrate, self.pmnpf.ipc, self.pmnpf.op_throughput, self.pmnpf.pre_reqs, self.pmnpf.mem_rd, self.pmnpf.mem_wr ) \
    #       + "%13s\t%8s%8s\t%4s\t%13e\t%13s\t%7d\t%7d\t%8s%8s\t%4s\t%13e\t%13s\t%7d\t%7d\n" %( "Prefetcher",    self.nresult.l1_missrate,  self.nresult.l2_missrate,  self.nresult.ipc,  self.nresult.op_throughput,  self.nresult.pre_reqs,  self.nresult.mem_rd,  self.nresult.mem_wr,  self.result.l1_missrate,  self.result.l2_missrate,  self.result.ipc,  self.result.op_throughput,  self.result.pre_reqs,  self.result.mem_rd,  self.result.mem_wr  ) \

    #        bmark ipc memrd memwr MR MR thrpt prerqs
    #output = "%12s\t%4s\t%8d\t%8d\t%7s%7s\t%13e\t%9s" %( self.bmark, self.result.ipc, self.result.mem_rd,  self.result.mem_wr, self.result.l1_missrate,  self.result.l2_missrate, self.result.op_throughput,  self.result.pre_reqs ) 

    output = "%12s\t%4s\t%7s\t%7s\t%7s\t%7s" %( self.bmark,self.result.ipc, self.result.l1_RDmr, self.result.l1_WRmr, self.result.l2_RDmr, self.result.l2_WRmr ) 

    print( output )

  def set_result( self, l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc ):
    self.result = Result( l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc )
    #self.result = Result( l1_missrate, l2_missrate, ipc, cycles, ops, pre_reqs, mem_rd, mem_wr )


#-------------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------------

def main():
  opts = parse_cmdline()

  #experiments = [Experiment] * len(reports)
  header = "%12s\t%4s\t%7s\t%7s\t%7s\t%7st" %( "bmark", "IPC", "L1 RDMR", "L1_WRMR", "L2 RDMR", "L2 RDMR")
  print( header )
  
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
    re14='( )'
    re15='(\\w+.\\w+%)'

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)


    l2_results = rg.findall( report_txt )

    #print(len(l2_results))
    #for i in range(len(l2_results)):
      #print(l2_results[i])

    l2_RDmr = l2_results[0][14]
    #print(l2_RDmr)


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
    re14='( )'
    re15='(\\w+.\\w+%)'

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l2_results = rg.findall( report_txt )

    #print(len(l2_results))
    #for i in range(len(l2_results)):
      #print(l2_results[i])

    l2_WRmr = l2_results[0][14]
    #print(l2_WRmr)

    

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
    re14='( )'
    re15='(\\w+.\\w+%)'

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l1_results = rg.findall( report_txt )

    #print(len(l1_results))
    #for i in range(len(l1_results)):
      #print(l1_results[i])

    l1_RDmr = l1_results[0][14]
    #print(l1_RDmr)


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
    re14='( )'
    re15='(\\w+.\\w+%)'

    rg=re.compile(re1+re2+re3+re4+re5+re6+re7+re8+re9+re10+re11+re12+re13+re14+re15,re.IGNORECASE|re.DOTALL)
    l1_results = rg.findall( report_txt )

    #print(len(l1_results))
    #for i in range(len(l1_results)):
      #print(l1_results[i])

    l1_WRmr = l1_results[0][14]
    #print(l1_WRmr)
   

      # Checks for result results in all 5 bmarks
    experiment.set_result(  l1_RDmr, l1_WRmr, l2_RDmr, l2_WRmr, ipc)
    experiment.print_results()
  #for experiment in experiments:

main()
