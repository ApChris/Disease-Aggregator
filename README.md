        Disease-Aggregator
        
        Folders

        Parent Folder: Disease-Monitor-master

        Source folder     : Disease-Aggregator-master/src
        Header folder     : Disease-Aggregator-master/include
        Build folder      : Disease-Aggregator-master/build         
        Etc folder        : Disease-Aggregator-master/etc           
        Output folder     : Disease-Aggregator-master/output        



        Compilation & Execution 

        Compile         : make rebuild
        Execution       : ./create_infiles.sh ../../etc/diseaseFile.txt ../../etc/countriesFile.txt input_dir 5 400           (~10 sec execution time)
                        : ./diseaseAggregator -w 5 -b 50 -i etc/input_dir


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


        Extra Tools

        Valgrind        : For memory leaks and errors
        GDB             : For bugs
