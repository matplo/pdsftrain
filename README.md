# The PDSF train framework

The pdsf train is a simple framework to run centralized ALICE trains
on the NERSC computing system PDSF over the data available at NERSC.
The train framework is organized in 

* User directories
* basic directory (with tender stuff, physics selection ...)
* train steering scripts

## Become part of it

In order to become part of the train, you have to fork the repoository, 
create a dev branch, and create a user dir. Within the user dir, a file
config.json is used for user configuration based on JSON syntax. In the
configuration file, a JSON-array with the list of tasks is exected for 
each data type in

* PbPb
* pPb
* pp
* MC_PbPb
* MC_pPb
* MC_pp

A task has the following properties:
* Name
* Add macro
* Status (active or inactive)

Add macro paths are usually given relative to the user directory. In case the 
keyword ALICE_PHYSICS is found in the macro string, the Add macro is taken 
directly from AliPhysics.

An example config looks like this:

    {
        "PbPb":
        [
            {
                "NAME":"ClusterRef",
                "MACRO":"AddTaskClustersRef.C",
                "STATUS":"Active"
            }
        ] ,
        "pp":
        [
        ],
        "pPb":
        [
        ],
        "MC_PbPb":
        [
            {
                "NAME":"ClusterRef",
                "MACRO":"AddTaskClustersRef.C",
                "STATUS":"Inactive"
            }
        ],
        "MC_pp":
        [
        ],
        "MC_pPb":
        [
        ]
    }

Once the config is available, users have to add their directory in the the file 
train/config/users. Afterwards, just put the necessary add macros and have fun!

## Run the train

The train can run in interactive mode, batch mode or train mode. The following 
options can be set:

 * -u / --user:  Run for a special user 
 * -c / --config: Configuration 
 * -l / --list: Run on a special file list
 * -s / --splitlevel: Number of files per job
 * -n / --nchunk: Number of chunks
 * -m / --minchunk: Minimum chunk (local running mode)
 * -i / --inputdir: Inputdir (for merging)
 * -f / --filename: Filename: (for merging)
 * -d / --debug: Debug mode (printing debug messages)
 * -h / --help: Print help

Note that the parameter -c is mandatory as the train configuration is loaded 
according to this parameter.

The interactive mode should be used to test your code on a small subset of 
files, provided by the list specified with the -l argument. The range is specified
by the parameters -n and -m, where -m defines the minimum entry in the list
and -n the number of files to process. An interactive command may look like 
this:

    ./run.py local -u <user> -c <configuration> -l <List, relatige to train/filelists> -n <number of files to process> -m <first file>
    
In order to run the train on the PDSF cluster, users should select the modes batch
or train. The difference is that the batch command submits only one list, to be
specified by the user, while the train mode submits the train on all files for a
given configuration, for all users which are enabled.

Once the train is done, you find the output under

* in batch mode: 

    /project/projectdirs/alice/[username]/train/V[Version].[configuration]/[tag]
    
* in train mode

    /project/projectdirs/alice/train/V[Version].[configuration]/[tag]
    
The tag always consists of the date and the time in seconds. Merging is currently 
done up to run level.
