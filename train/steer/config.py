'''
Created on 01.12.2015

@author: markusfasel
'''

import os, getpass, json

gConfig = None
gMode = ""
gTrainRoot = ""

class ConfigHandler(object):
    
    @staticmethod
    def LoadConfiguration(mode):
        global gConfig
        if not gConfig and mode != gMode:
            gConfig = Config(os.path.join(gTrainRoot, "train", "config", "config_%s.json" %mode))
    
    @staticmethod
    def GetConfig():
        global gConfig
        if not gConfig:
            raise Exception("Config not set")
        return gConfig 
    
    @staticmethod
    def GetTrainRoot():
        global gTrainRoot
        return gTrainRoot
    
    @staticmethod
    def SetTrainRoot(trainroot):
        global gTrainRoot
        gTrainRoot = trainroot
    
class Config(object):
    
    def __init__(self, filename = None):
        self.__name                     = ""
        self.__version                  = "V1"
        self.__outputdir                = "/project/projectdirs/alice/"
        self.__splitlevel               = -1
        self.__treename                 = ""
        self.__handlers                 = []
        self.__mergesize                = 10
        if filename:
            self.Initialize(filename)
        
    def Initialize(self, jsonfile):
        jsontree = json.loads(self.__ReadConfigFile(jsonfile))
        for k, v in jsontree.iteritems():
            key = str(k).upper().lstrip().rstrip()
            if key == "NAME":
                self.__name = v
            elif key == "VERSION":
                self.__version = v
            elif key == "OUTPUT":
                self.__outputdir = v
            elif key == "SPLITLEVEL":
                self.__splitlevel = v
            elif key == "MERGE_CHUNKSIZE":
                self.__mergesize = v
            elif key == "HANDLERS":
                for en in v:
                    self.__handlers.append(str(en)) 
            elif key == "TREENAME":
                self.__treename = v
        self.Print()
    
    def __ReadConfigFile(self, configfile):
        jsonstring = ""
        reader = open(configfile, 'r')
        for line in reader:
            jsonstring += line.replace("\n","")
        reader.close()
        return jsonstring

    def GetTrainOutputPath(self, doGlobal):
        userdir = os.path.join(getpass.getuser(), "train") if not doGlobal else "train"
        return os.path.join(self.__outputdir, userdir)
    
    def GetName(self):
        return self.__name
    
    def GetVersion(self):
        return self.__version
    
    def GetSplitLevel(self):
        return self.__splitlevel
    
    def GetTreename(self):
        return self.__treename
    
    def GetHandlers(self):
        return self.__handlers
    
    def GetMergeSize(self):
        return self.__mergesize
    
    def Print(self):
        print "Configuration:"
        print "Name:          %s" %self.__name
        print "Version:       %s" %self.__version
        print "Tree:          %s" %self.__treename
        print "Splitlevel:    %d" %self.__splitlevel
        print "output:        %s" %self.__outputdir
        print "Handlers:"
        for handler in self.__handlers:
                print "        %s" %handler

