# Disease-Aggregator
        Folders

        Parent Folder: Disease-Monitor-master

        Source folder     : Disease-Aggregator-master/src
        Header folder     : Disease-Aggregator-master/include
        Build folder      : Disease-Aggregator-master/build         (This folder holds .o files)
        Etc folder        : Disease-Aggregator-master/etc           (This fodler holds the input files, logfiles, fifo files)
        Output folder     : Disease-Aggregator-master/output        (This folder holds the output from > for better file organization, In default compilation/execution this folder is not used)



-----> Compilation & Execution <-----

        Compile         : In general folder open a terminal and execute this command:          make rebuild
        Execution       : In src/scripts folder open a terminal and execute this command:      ./create_infiles.sh ../../etc/diseaseFile.txt ../../etc/countriesFile.txt input_dir 5 400           (~10 sec execution time)
                        : In general folder open a terminal and execute this command:          ./diseaseAggregator -w 5 -b 50 -i etc/input_dir


        User Input Examples:

                        /listCountries
                        /numPatientAdmissions COVID-2019 10-10-2010 10-10-2020
                        /diseaseFrequency COVID-2019 10-10-2010 10-10-2020
                        /diseaseFrequency MERS-COV 10-10-2010 10-10-2020
                        /diseaseFrequency MERS-COV 10-10-2010 10-10-2020 Greece

                        /topk-AgeRanges 3 Greece COVID-2019 10-10-2010 10-10-2020
                        /topk-AgeRanges 4 China COVID-2019 10-10-2010 10-10-2020
                        /topk-AgeRanges 3 USA COVID-2019 10-10-2010 10-10-2020
                        /topk-AgeRanges 4 China MERS-COV 10-10-2010 10-10-2020

                        /searchPatientRecord 10



-----> Explanation <-----

       Every data structure is a part from Disease-Monitor. In this project, I added a multi-process functionality by using named pipes with non-blocking. The method that I have followed is master and slave ( or main/secondary),
        main is the parent process and secondary is the worker process. I have split the files in worker"Files" and parent"Files". For example in parentNamedPipesFifo.c you can find the functions that parent is going to use to comm$
        Also in parentFunctions you can find the functions/signals that parent is going to use. Workers follow the same pattern respectively.

-----> Extra Tools <-----

        Valgrind        : For memory leaks and errors
        GDB             : For bugs
