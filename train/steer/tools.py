'''
Created on 01.12.2015

@author: markusfasel
'''

import os, time
from train.steer.config import ConfigHandler
from train.steer.sample import Sample

def GetTag():
    config = ConfigHandler.GetConfig()
    return "%s.%s/%s" %(config.GetVersion(), config.GetName(), time.strftime("%y%m%d_%H%M%S", time.localtime()));

def FindList(listname):
    currentdir = ConfigHandler.GetTrainRoot()
    files = FindFiles(os.path.join(currentdir, "train", "filelists"))
    hasfound = False
    for f in files:
        if listname in f:
            hasfound = True
            break
    return hasfound

def FindFiles(inputdir):
    result = []
    for mypath, mydirs, myfiles in os.walk(inputdir):
        for f in myfiles:
            result.append(os.path.join(os.path.abspath(mypath), f))
    return result


def GetSamplesForConfig(configname):
    result = []
    inputdir = os.path.join(ConfigHandler.GetTrainRoot(), "train", "config", "samples")
    for mypath, midirs, myfiles in os.walk(inputdir):
        for l in myfiles:
            if str(l).startswith(configname):
                result.append(Sample(os.path.join(mypath, l)))
    return result
    
def DecodeSGEResponse(sgeresponse):
    tokens = sgeresponse.split(" ")
    runstring = tokens[2]
    return int(runstring.split(".")[0])
        
def GetWorkdir(doGlobal):
    return os.path.join(ConfigHandler.GetConfig().GetTrainOutputPath(doGlobal), GetTag())
