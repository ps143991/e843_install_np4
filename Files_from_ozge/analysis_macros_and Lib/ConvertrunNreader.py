#!/usr/bin/python2.7
import os, time, sys, multiprocessing, glob
runs = open("lists_of_runlist.txt").readlines()
while len(runs) > 0:
  core_tot = multiprocessing.cpu_count()
  core_used = os.getloadavg()[0]
  core_ratio = float(core_used)/float(core_tot)
  mem_str = os.popen("free -m | awk \'/Mem/{print $3}\'").read()
  mem = int(mem_str[:-1])
  irun = ""
  if (mem > 20000) or (core_ratio > 0.5):
    core_tot = multiprocessing.cpu_count()
    core_used = os.getloadavg()[0]
    core_ratio = float(core_used)/float(core_tot)
    mem_str = os.popen("free -m | awk \'/Mem/{print $3}\'").read()
    mem = int(mem_str[:-1])
    sys.stdout.write("\rCurrent Memory occupation : {0}  Current Core occupation : {1}".format(mem,core_ratio))
    sys.stdout.flush()
    time.sleep(1)
  else:
    runs_list = runs.pop(0).strip()
    command = " npreader -C Calibrationnew.txt -E Reactions/68Ni_dp_gs_18MeVu.reaction -D DetectorConfiguration/mugast_e843.detector -O "+runs_list+" -R "+runs_list+"&"
    print "Starting conversion of runlist "+runs_list
    os.system(command)
    time.sleep(20)


