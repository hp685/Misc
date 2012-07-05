/***************************************************************************************************
Description: This program takes in input arguments as 
1) Input File Location
2) Ticker
3) Date 
4) Frequency of data fetch
5) Location of output
6) Delimiter to be used in the output file. 

and outputs the data matched according to the matching criteria with the columns delimited by the
specified delimiter. 

This program has default output location. The shell wrapper can, then, take it from there and put it 
in, where the user needs it. 
***************************************************************************************************/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


/*Field offsets*/
#define OUTPUT_SYMBOL_OFFSET 0
#define OUTPUT_SEQUENCE_OFFSET 5
#define OUTPUT_DATE_OFFSET 15
#define OUTPUT_TIME_OFFSET 23
#define OUTPUT_PUT_CALL_OFFSET 32
#define OUTPUT_EXPIRATION_OFFSET 33
#define OUTPUT_STRIKE_OFFSET 41
#define OUTPUT_VOLUME_OFFSET 48
#define OUTPUT_PREMIUM_OFFSET 54
#define OUTPUT_EXCHANGE_OFFSET 63
#define OUTPUT_UNDERLYING_PRICE_OFFSET 64
#define OUTPUT_UNDERLYING_TIME_OFFSET 76

/*ABSOLUTE Sizes*/
#define OUTPUT_SYMBOL_SIZE 5 
#define OUTPUT_SEQUENCE_SIZE 10
#define OUTPUT_DATE_SIZE 8
#define OUTPUT_TIME_SIZE 9
#define OUTPUT_PUT_CALL_SIZE 1
#define OUTPUT_EXPIRATION_SIZE 8
#define OUTPUT_STRIKE_SIZE 7
#define OUTPUT_VOLUME_SIZE 6
#define OUTPUT_PREMIUM_SIZE 9
#define OUTPUT_EXCHANGE_SIZE 1
#define OUTPUT_UNDERLYING_PRICE_SIZE 12
#define OUTPUT_UNDERLYING_TIME_SIZE 9

/*Auxiliary definitions*/
#define TIME_REQ 6 /*First 6 characters of the time. Don't care about the milli-seconds. */
#define TRUE 1
#define FALSE 0
#define FIXED_STRING_LENGTH 87
#define NUM_FIELDS 15
#define TRIM_TIME 3

/*Custom Functions*/


/***********************************************************

Description : Takes as input a timestamp in hhmmss and converts
it into seconds, which can then be used to check for a match.
Arguments: integer - A timestamp

Return Values: Integer- hhmmss converted all into seconds only. 

************************************************************/
int toseconds(int timestamp){
  int seconds = timestamp % 100;
  timestamp /= 100;
  int minutes = timestamp % 100;
 timestamp /= 100;
  int hours = timestamp;
  int timestamp_inseconds = seconds + (minutes * 60) + (hours * 3600);
 
  return timestamp_inseconds; 

}

/*TRIM TRAILING WHITESPACE FROM THE TICKER*/
char *trim(char *ticker_symbol){
  
  int ticker_len = strlen(ticker_symbol);
  int i;
  while(ticker_symbol[ticker_len - 1] == ' '){
    ticker_len--;
  }
  ticker_symbol[ticker_len] = '\0';
  return ticker_symbol;
}


/**************************************************************
 We get here, only if there's a match. 
 *************************************************************/

char *OUTPUT(char * opradata, char *delimiter_type, char *output_record){
  /*Go through the records once again, splitting at OFFSETS and inserting the delimiter in */
  /*Need to redefine each field with an appropriate data-type? or just use chars? 
    Broken the input this way so that fields may be interchanged if needed. */
  char ticker_symbol[OUTPUT_SYMBOL_SIZE + 1];
  char message_sequence_number[OUTPUT_SEQUENCE_SIZE + 1];
  char date[OUTPUT_DATE_SIZE + 1];
  char time[OUTPUT_TIME_SIZE - TRIM_TIME + 1];
  char put_call[OUTPUT_PUT_CALL_SIZE + 1];
  char expiration[OUTPUT_EXPIRATION_SIZE + 1];
  char strike_price[OUTPUT_STRIKE_SIZE + 1];
  char trade_volume[OUTPUT_VOLUME_SIZE + 1];
  char premium_price[OUTPUT_PREMIUM_SIZE + 1];
  char exchange_indicator[OUTPUT_EXCHANGE_SIZE + 1];
  char underlying_price[OUTPUT_UNDERLYING_PRICE_SIZE + 1];
  char underlying_time[OUTPUT_UNDERLYING_TIME_SIZE - TRIM_TIME + 1];
  
  /*NULL-TERMINATE each field*/
  ticker_symbol[OUTPUT_SYMBOL_SIZE] ='\0';
  message_sequence_number[OUTPUT_SEQUENCE_SIZE]='\0';
  date[OUTPUT_DATE_SIZE] = '\0';
  time[OUTPUT_TIME_SIZE - TRIM_TIME] = '\0';
  put_call[OUTPUT_PUT_CALL_SIZE] = '\0';
  expiration[OUTPUT_EXPIRATION_SIZE] = '\0';
  strike_price[OUTPUT_STRIKE_SIZE] = '\0';
  trade_volume[OUTPUT_VOLUME_SIZE] = '\0';
  premium_price[OUTPUT_PREMIUM_SIZE] = '\0';
  exchange_indicator[OUTPUT_EXCHANGE_SIZE] = '\0';
  underlying_price[OUTPUT_UNDERLYING_PRICE_SIZE] = '\0';
  underlying_time[OUTPUT_UNDERLYING_TIME_SIZE - TRIM_TIME] = '\0';
  /*Contains delimited output*/

 
  int i;
  
  /*Character-by-Character Copy-- Prepare fields for output*/
  for( i = 0; i < OUTPUT_SYMBOL_SIZE; i++){
    ticker_symbol[i] = opradata[OUTPUT_SYMBOL_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_SEQUENCE_SIZE; i++){
    message_sequence_number[i] = opradata[OUTPUT_SEQUENCE_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_DATE_SIZE; i++){
    date[i] = opradata[OUTPUT_DATE_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_TIME_SIZE - TRIM_TIME; i++){
    time[i] = opradata[OUTPUT_TIME_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_PUT_CALL_SIZE; i++){
    put_call[i] = opradata[OUTPUT_PUT_CALL_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_EXPIRATION_SIZE; i++){
    expiration[i] = opradata[OUTPUT_EXPIRATION_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_STRIKE_SIZE; i++){
    strike_price[i] = opradata[OUTPUT_STRIKE_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_VOLUME_SIZE; i++){
    trade_volume[i] = opradata[OUTPUT_VOLUME_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_PREMIUM_SIZE; i++){
    premium_price[i] = opradata[OUTPUT_PREMIUM_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_EXCHANGE_SIZE; i++){
    exchange_indicator[i] = opradata[OUTPUT_EXCHANGE_OFFSET + i]; 
  }
  for( i = 0; i < OUTPUT_UNDERLYING_PRICE_SIZE; i++){
    underlying_price[i] = opradata[OUTPUT_UNDERLYING_PRICE_OFFSET + i];
  }
  for( i = 0; i < OUTPUT_UNDERLYING_TIME_SIZE - TRIM_TIME; i++){
    underlying_time[i] = opradata[OUTPUT_UNDERLYING_TIME_OFFSET + i];
  }
  /*Concatenate OUTPUT fields*/
  strcat(output_record, trim(ticker_symbol)); /*Trims trailing whitespaces from the TICKER*/
  strcat(output_record, delimiter_type);
  strcat(output_record, message_sequence_number);
  strcat(output_record, delimiter_type);
  strcat(output_record, date);
  strcat(output_record, delimiter_type);
  strcat(output_record, time);
  strcat(output_record, delimiter_type);
  strcat(output_record, put_call);
  strcat(output_record, delimiter_type);  
  strcat(output_record, expiration);
  strcat(output_record, delimiter_type); 
  strcat(output_record, strike_price);
  strcat(output_record, delimiter_type);  
  strcat(output_record, trade_volume);
  strcat(output_record, delimiter_type);  
  strcat(output_record, premium_price);         
  strcat(output_record, delimiter_type);
  strcat(output_record, exchange_indicator);
  strcat(output_record, delimiter_type);  
  strcat(output_record, underlying_price);
  strcat(output_record, delimiter_type);  
  strcat(output_record, underlying_time); /*No delimiter after the final field*/      
  strcat(output_record, "\n");
  /*OUTPUT output_record to a default file*/
   
  return output_record; 
}
/************************************************************************
Performs a lookup in the records, matching tickers, date and time. 
RETURN VALUE: Returns a boolean value if the record matches the criterion


*************************************************************************/
/*Fetches the records according to the criteria
Need to add a check: If no records match alert the user*/

int LookUp(char *opradata,char *ticker, char *date, int data_frequency){

  static int check_second;
  int record_match = FALSE; /*If no records match-- ALERT THE USER*/
  char fetch_ticker[OUTPUT_SYMBOL_SIZE + 1];
  char fetch_date[OUTPUT_DATE_SIZE + 1 ];
  char fetch_time[OUTPUT_TIME_SIZE + 1];
  fetch_ticker[OUTPUT_SYMBOL_SIZE]='\0';
  fetch_date[OUTPUT_DATE_SIZE]='\0';  
  fetch_time[OUTPUT_TIME_SIZE] ='\0';  
  int is_time = FALSE;
  int timestamp; 
  int i,j,k;
  int ticker_len = 0;
  /* Extract Ticker from the fixed string */
  for(i = 0 ; i < OUTPUT_SYMBOL_SIZE; i++){
    fetch_ticker[i] = opradata[OUTPUT_SYMBOL_OFFSET + i];
  }
  
  char *trimmed_ticker = trim(fetch_ticker); /*Trims the trailing white-space*/
  /* Compare Tickers*/  
  if(strcmp(trimmed_ticker,ticker) == 0 ){
    /*Extract date from the fixed string*/
    for(j = 0; j < OUTPUT_DATE_SIZE; j++){
      fetch_date[j] = opradata[OUTPUT_DATE_OFFSET + j];
    }
    /*Compare Dates */ 
    if(strcmp(fetch_date,date) == 0){
      /*Fetch the time and check for a match*/
      
      for(k = 0; k < OUTPUT_TIME_SIZE; k++){
	fetch_time[k] = opradata[OUTPUT_TIME_OFFSET + k];	
      }
      
      timestamp = atoi(fetch_time);
      timestamp /= 1000; /* Trim the milliseconds*/
      timestamp = toseconds(timestamp); /*Convert the timestamp to seconds*/
      is_time = timestamp % data_frequency;
      //printf(" IS_TIME: %d\n",is_time);
      if(!is_time && timestamp > check_second){ /*is_time is zero when there's a match*/
	check_second = timestamp; 
	return TRUE; 
      }
    }
  } 
  return FALSE;
}

/*User Input Validation*/


int main(int argc, char *argv[])
{
  char *ticker;
  char *date;
  char *data_frequency; /*Frequency of Fetch*/
  char *delimiter_type;


  if(argc == 6){    
    ticker = argv[2];
    date = argv[3];
    data_frequency = argv[4];
    delimiter_type = argv[5];
    //Validate Input

  }
  else if(argc < 5){
    printf("Error: Too few Arguments. Arguments must be Tickers,Dates,Time-Interval,Output-Location\n");
    exit(EXIT_FAILURE);
  }
  else{
    printf("Arguments to the program are Input File,Tickers,Dates,Time-Interval,Output-Location\n");
    exit(EXIT_FAILURE);
  }
 
 /* A row in the OPRA-TAQ file --fixed string format */
  char opradata[FIXED_STRING_LENGTH];
  FILE *OPRAFILE, *OUTFILE;
  int freq_fetch = atoi(data_frequency);
  char *output;  
  char *output_record; 

  /*Change later to the path provided by the bash script*/
  OPRAFILE = fopen(argv[1],"r");  /*Changed to the path provided in the input*/ 
  //  output_record = (char *)malloc(FIXED_STRING_LENGTH + NUM_FIELDS);
    OUTFILE = fopen("OPRA-UI","w");      
    while(fgets(opradata,sizeof(opradata),OPRAFILE) != NULL){
      /*      printf("%d %c\n",freq_fetch, delimiter_type);*/

      int match = LookUp(opradata,ticker,date,freq_fetch);    

      if(match){
	output_record = (char *)malloc(FIXED_STRING_LENGTH + NUM_FIELDS);
	output = OUTPUT(opradata,delimiter_type,output_record);
	/*printf(" OUTPUT: %s\n",output_record);*/
	fputs(output,OUTFILE);
	free(output_record);
      }
      memset(output_record,0,sizeof(output_record));
      memset(output,0,sizeof(output));

    }
    fclose(OUTFILE);

   return 0;
}
 




