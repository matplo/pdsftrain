#! /usr/bin/env python

import os, shutil
import ROOT

def merge_iteration(inputlist, splitlevel, outputfilename, workdir):
    result = []
    outputfilebase = outputfilename.split(".")[0]
    nbucket = 0
    nchunk = 0
    bucketfilename = os.path.join(workdir, "%s_%d.root" %(outputfilebase, nchunk))
    merger = ROOT.TFileMerger()
    merger.OutputFile(bucketfilename)
    for line in inputlist:
        merger.AddFile(line)
        nbucket += 1
        if nbucket >= splitlevel:
            print "Merging bunch %d" %nchunk
            merger.Merge()
            result.append(bucketfilename)

            #prepare next
            nbucket=0
            nchunk += 1
            bucketfilename = os.path.join(workdir, "%s_%d.root" %(outputfilebase, nchunk))
            merger = ROOT.TFileMerger()
            merger.OutputFile(bucketfilename)

    # Merge last bunch
    if nbucket:
        merger.Merge()
        result.append(bucketfilename)
    return result

def find_mergefiles(inputdir, rootfilename):
        result=[]
        for mypath, mydirs, myfiles in os.walk(inputdir):
            for f in myfiles:
                if rootfilename in f:
                    result.append(os.path.join(mypath, f))
        return result

def merge(inputdir, rootfilename, chunksperiter):
    # Assume we are in the output directory := working direcktory
    splitlevel=10
    workdirbase = os.getcwd()

    iteration = 0
    workdir_source = None
    workdir_target = None
    filelist = find_mergefiles(inputdir, rootfilename)
    basename = rootfilename.split(".")[0]

    isDone = True if len(filelist) <= 1 else False
    while not isDone:
        if workdir_source:
            shutil.rmtree(workdir_source)
        if workdir_target:
            workdir_source = workdir_target
        workdir_target = os.path.join(workdirbase, "%s_iteration_%d" %(basename, iteration))
        os.makedirs(workdir_target, 0744)
        filelist = merge_iteration(filelist, splitlevel, rootfilename, workdir_target)
        # prepare next recursion step
        iteration += 1
        isDone = True if len(filelist) <= 1 else False

    # Move output to its destination
    shutil.copy(os.path.join(workdir_target, filelist[0]), os.path.join(workdirbase, rootfilename))
    # find leftover iterations
    dircontent = os.listdir(workdirbase)
    for mydir in dircontent:
        if "iteration" in mydir:
            shutil.rmtree(os.path.join(workdirbase, mydir))
    # We are done
    print "Merging file %s completed after %d iterations" %(rootfilename, iteration)
