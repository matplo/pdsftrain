'''
Created on 09.12.2015

@author: markusfasel
'''

import os, commands
from train.steer.tools import DecodeSGEResponse
from train.steer.config import ConfigHandler

class Mergejob(object):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.__trainroot = ""
        self.__inputdir = ""
        self.__outputdir = ""
        self.__filetomerge = ""
        self.__holdjid = []
        self.__jid = -1
        
    def SetInputDir(self, inputdir):
        self.__inputdir = inputdir
        
    def SetOutputDir(self, outputdir):
        self.__outputdir = outputdir
        
    def SetTrainRoot(self, trainroot):
        self.__trainroot = trainroot
        
    def SetFileToMerge(self, filetomerge):
        self.__filetomerge = filetomerge
        
    def AddHoldJID(self, holdjid):
        self.__holdjid.append(holdjid)
    
    def GetInputDir(self):
        return self.__inputdir
    
    def GetOutputDir(self):
        return self.__outputdir
    
    def GetTrainRoot(self):
        return self.__trainroot
    
    def GetFileToMerge(self):
        return self.__filetomerge
    
    def GetListOfHoldJobs(self):
        return self.__holdjid
    
    def GetJobID(self):
        return self.__jid
    
    def Submit(self):
        if not os.path.exists(self.__outputdir):
            os.makedirs(self.__outputdir, 0755)
        cmd = "qsub -l \"projectio=1,h_vmem=4G\" -P alice %s %s %s " %(self.__BuildDependencyString(), self.__GetLogging(), self.__GetExecutable()) 
        print "Submitting %s" %cmd
        self.__jid = DecodeSGEResponse(commands.getstatusoutput(cmd)[1])
        
    def __GetLogging(self):
        return "-j y -o %s" %os.path.join(self.__outputdir, "merge.log")
    
    def __BuildDependencyString(self):
        result = "-hold_jid "
        isFirst = True
        for j in self.__holdjid:
            if isFirst:
                isFirst = False
            else:
                result += ","
            result += "%d" %j
        return result
    
    def __GetExecutable(self):
        execname = os.path.join(self.__trainroot, "train", "steer", "mergescript.sh")
        args =  "%s %s %s %s %s" %(self.__trainroot, ConfigHandler.GetConfig().GetName(), self.__inputdir, self.__outputdir, self.__filetomerge)
        return "%s %s" %(execname,  args)
