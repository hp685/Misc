#!/bin/bash
# 
# This wrapper to the OPRA_UI.c takes in arguments
# 1) Ticker
# 2) Date
# 3) Frequency of fetch
# 4) Delimiter to be used for the file. 
# 5) Output location: When not provided, default to /ise/opradata/opra-taq-merged
###################################################################################

SCRIPT="/tesladata/isetools/cdISE.bash"
OPRADIR="/tesladata/isetools/opra-trades-ui"
OUTFILE="$OPRADIR/OUTFILE"
OUTDIR="/bigise/ise/opraDataExtracted/trades-taq-sampled/"
STARTDATE=""
ENDATE=""
TICKERS=""
INTERVAL=""
DELIMITER=""

usage()
{
    echo 
    echo "Usage: OPRA_UI.sh -s <Start Date> -e <End Date> -t <Tickers File>, -i <Frequency Fetch in seconds> -d <Delimiter Type>"
    echo "Dates should be in YYYYMMDD format"
    echo "Tickers file can either be a file-name or, specify a '*' for all Tickers"
}


#
#
processArgs()
{
    while getopts "s:e:t:i:d:" option 
      do
      case $option in 
	  s) 
	      STARTDATE=$OPTARG
	      ;;
	  
	  e) 
	      ENDATE=$OPTARG  
	      ;;
	  
	  t) 
	      TICKERS=$OPTARG
	      ;;
	  
	  i) 
	      INTERVAL=$OPTARG
	      ;;
	  
	  d) 
	      DELIMITER=$OPTARG
	      ;;
	  
	  ?) 
	      clear
	      echo 
	      usage
	      exit
	      ;;
      esac
    done
}


#Error checks 
checkErrors()
{
#Not enough arguments?
#Data for the start date does not exist.
    
    TEMP=`sh $SCRIPT $STARTDATE`
      if [ $TEMP == ""  ]
	then
	echo
	echo "No Data Available for the specified Start Date"
	exit
    fi
    
#Data for the end date does not exist.
    TEMP=`sh $SCRIPT $ENDATE`
    if [ $TEMP == "" ]
	then
	echo
	echo "No Data Available for the specified End Date"
	exit
    fi
}

#Main

if [ $# -lt 10 ] 
    then 
    usage
    exit
fi 

processArgs "$@"
checkErrors

#Loop through the start and end dates querying the data with the specified time interval for all tickers.
if [ "$TICKERS" == '*' ]
    then 
    TICKERS="#"
fi
start=`date --date=${STARTDATE} +%s`
end=`date --date=${ENDATE} +%s`
while [ "$start" -le "$end" ]
  do
  #date -d @epoch converts the date in seconds to the required output format.YYYYMMDD, in this case.  
  DATE=`date -d @"$start" +%Y%m%d`
  #Delete the outputfile if it exists. 
  #rm $OUTFILE
  #Get the location of the input file. 
  LOCATION=`sh ${SCRIPT} $DATE` 
  #Generate a Unique number for the filename
  TIMESTAMP=`date +%s`
  g++ -o OPRA "$OPRADIR"/OPRA-UI.cpp 
  ./OPRA  $LOCATION/trades-taq-merged.txt $TICKERS $DATE $INTERVAL $DELIMITER      
  cat $OUTFILE >> $OUTDIR$STARTDATE-$ENDATE-$INTERVAL-$TIMESTAMP-$TICKERS.txt
  rm $OUTFILE
  
  start=$(($start+3600))
done

