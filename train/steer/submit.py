import os, commands
from train.steer.config import ConfigHandler
from train.steer.tools import DecodeSGEResponse
    
class Submitter():
    
    def __init__(self, inputlist, jobtrainroot, outputdir, splitlevel):
        self.__inputlist = inputlist
        self.__jobtrainroot = jobtrainroot
        self.__outputdir = outputdir
        self.__nchunk = -1
        self.__splitlevel = splitlevel
        self.__user = "all"
        self.__jobid = -1
        
    def SetUser(self, user):
        self.__user = user
        
    def SetNchunk(self, nchunk):
        self.__nchunk = nchunk

    def GetJobID(self):
        return self.__jobid
    
    def Submit(self):
        jobs = self.__nchunk
        if jobs < 0:
            jobs = int(self.GetNfiles()) / self.__splitlevel + 1
        for j in range(1, jobs+1):
            os.makedirs(os.path.join(self.__outputdir, "job%d" %j))
        qsub = "qsub -l \"projectio=1,h_vmem=6G\" -P alice -t 1:%d" %jobs
        qsub += " " + self.__GetLogging()
        qsub += " " + self.__GetExecutable()
        #print "Here I would do %s" %qsub
        response  = commands.getstatusoutput(qsub)[1]
        print response
        self.__jobid = DecodeSGEResponse(response)
        
    def __GetExecutable(self):
        return "%s %s %s %s %s %s %s" %(os.path.join(self.__jobtrainroot, "train", "steer", "jobscript.sh"), self.__jobtrainroot, ConfigHandler.GetConfig().GetName(), self.__outputdir, self.__user, self.__inputlist, self.__splitlevel)
    
    def __GetLogging(self):
        return "-j y -o %s/job\$TASK_ID/joboutput.log" %self.__outputdir
    
    def GetNfiles(self): 
        nfiles = 0
        # path = "%s/train/filelists/%s" %(self.__jobtrainroot, self.__inputlist)
        # reader = open(path, 'r')
        reader = open(os.path.join(self.__jobtrainroot, "train", "filelists", self.__inputlist), 'r')
        for line in reader:
            newline = line.replace("\n", "").lstrip().rstrip()
            if not len(newline):
                continue
            nfiles += 1
        reader.close()
        return nfiles
