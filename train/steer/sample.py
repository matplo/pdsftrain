'''
Created on 09.12.2015

@author: markusfasel
'''

import json

class Sample(object):
    '''
    classdocs
    '''

    def __init__(self, filename = None):
        '''
        Constructor
        '''
        self.__name = ""
        self.__year = 0
        self.__splitlevel = 10
        self.__filelists = []
        if filename:
            self.__Initialize(filename)
        
    def __Initialize(self, filename):
        jsonstring = ""
        reader = open(filename)
        for line in reader:
            line = line.replace("\n", "")
            line = line.rstrip().lstrip()
            if not len(line):
                continue
            jsonstring += line
        reader.close()
        jsontree = json.loads(jsonstring)
        for k, v in jsontree.iteritems():
            if k.upper() == "NAME":
                self.__name = str(v)
            elif k.upper() == "SPLITLEVEL":
                self.__splitlevel = int(v)
            elif k.upper() == "YEAR":
                self.__year = int(v)
            elif k.upper() == "FILELISTS":
                for l in v:
                    self.__filelists.append(l)
                    
    def GetName(self):
        return self.__name
    
    def GetYear(self):
        return self.__year
    
    def GetFilelists(self):
        return self.__filelists
    
    def GetSplitLevel(self):
        return self.__splitlevel
