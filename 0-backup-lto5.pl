#!/usr/bin/perl -w


# Automatic backup program
# Perl Script

#Author  Harsh Patel 
#        harsh@nyu.edu

#######################################



use strict;
use warnings;
use Time::Local;


# sub processArgs {
# use Getopt::Std;
# getopts('d:') or usage();
# usage() if $opt{h};

# }

#############################################################

                  #Globals

#############################################################

our $TAPEMAX = 700;
our $user='ise';
our $machine='tesla';
our $isetoolsPath='/tesladata/isetools';
our $cdISE='cdISE.bash';
our $previousWeekday="perl $isetoolsPath/getPrevWeekday.pl";
our $backupPath='/tesladata/isetools/backupprogs/pv_lto5/links';
our $pastBackupsPath='$backupPath/pastbackups';
our $bhistoryPath='/tesladata/ise/bhistory';
our $getNextDateProg='/tesladata/isetools/getNextWeekday.pl';
our $drivenum = '2';
our $drivePath = '/dev/sg4'; 
#############################################################

              #Validates user and machine

#############################################################

sub validateUserAndMachine{
   
    my  $current_user=`whoami`;
    $current_user =~ s/\s+$//g;  #Trim trailing whitespaces
    my  $current_machine=`uname -n`;
    $current_machine =~ s/\s+$//g;
    if ($current_user ne $user)	{
	print "You must be logged in as $user. Aborting... \n" ;
	exit;
	}
    elsif ($machine ne $current_machine){
	print "You must be logged into $machine. Aborting...\n";
	exit;
    }

}

##############################################################


               #Validates Start and end dates

##############################################################

sub validateStartAndEndDates{
    
    my $startDate=getStartDate();
    my $endDate=getEndDate($startDate);
    printDates($startDate,$endDate);
    return ($startDate, $endDate); # returns a list
}


##############################################################

#                   Get Start date

##############################################################

sub getStartDate{

    my $validDate = 1;
    my $startDate = 0 ;
#Prompt for a start date until user enters a valid date 
#for which data exists 
    do{
	print "Enter a valid start date in YYYYMMDD format or CTRL-C to abort:";
        $startDate = <STDIN>;
	my $dirToBackup= `"$isetoolsPath/$cdISE" $startDate`;
	print $dirToBackup;
	if(length($dirToBackup) == 0){
	    print "No Data for the start Date specified.\n";
	    $validDate=1;
	}
	else{
	    $validDate = 0;
	}
    }until($validDate == 0);

    return $startDate;
}


###############################################################

#                    Get End Date & Validation


###############################################################

sub getEndDate{
    my $startDate = $_[0];
    my $validDate = 1;
    my $endDate = 0;
    do{
	print "Enter a valid end date in YYYYMMDD format or 
     CTRL-C to abort:";
        $endDate = <STDIN>;
	my $dirToBackup = `"$isetoolsPath/$cdISE" $endDate`;
	if($endDate eq "\n"){ # User entered RETURN
	    #Default Selection
	    print "Making a default selection...\n";
	    print  "End Date Provided: $endDate \n";
	    # Start with the current date and loop backwards until a date with existing data is found.
	    $endDate = `date +%Y%m%d`;
	    do {
		my $prev = `$previousWeekday $endDate`;
		$dirToBackup = `"$isetoolsPath/$cdISE" $prev`;
		$endDate = $prev;
	    }until( length($dirToBackup) != 0 ); 
	    $validDate = 0;
	}
	elsif($endDate < $startDate ) # Bad Case
	{
	    print "\n";
	    print "Start Date is larger than the End Date! \n";
	    print "\n";
	    $validDate = 1; #Redo the until statement
	}
	else{ #User entered an end date
	    
	    $dirToBackup= `"$isetoolsPath/$cdISE" $endDate`;
	    print $dirToBackup;
	    
	    #If Zero => Data does not exist.
	    #Choose the nearest date, for which there is data, prior to the date provided. 
	    if(length($dirToBackup) == 0) {
		print "No Data for this Date.\n";
		print "Making a selection for an existing date prior to the entered date...\n";
		do {
		    my $prev = `$previousWeekday $endDate`;
		    $dirToBackup = `"$isetoolsPath/$cdISE" $prev`;
		    $endDate = $prev;
		}until( length($dirToBackup) != 0 );
	    }
	    $validDate = 0;
	}
    }until($validDate == 0);
    
    return $endDate;
}

#################################################################

#  Prints Dates for the user to verify. 

#################################################################
sub printDates{

    my $startDate = $_[0];
    my $endDate = $_[1];
    print "\n";
    print "Start Date: $startDate  End Date: $endDate \n";

}

#################################################################

# Roll Back and Exit
# Clean up the sym links from the links directory
 
#################################################################

sub rollBackAndExit{

    print " Rolling back changes...\n";
    `rm -f $backupPath/lnkd*`; 
    print " Changes undone...";
    print "";
}


#################################################################

# Create a list of directories to backup.

#################################################################

sub createListing{

    my $startDate = $_[0];
    my $endDate = $_[1];
    my $nextDate = $startDate; #Start with the start date provided
    my @dates =(); 
    my @paths = ();
    # Determine the yearmonth directory name
    my $yearMonth = substr $nextDate, 0, 6; #YYYYMMDD => YYYYMM
    
    # Previous backups?                                              !
    my $sumTotal = 0;
    
    # Current Date
    my ($mday,$mon,$year) = (localtime)[3,4,5];

    # Perl quirks 
    $year += 1900; 
    $mon  += 1;
    if(length($mday) == 1) {$mday = "0" . $mday;}
    if(length($mon) == 1 ) {$mon = "0" . $mon;}
    #
    my $today = "$year$mon$mday";
    print " Today is $today\n";             # Needs Verification.
    
    do{
	my $dirToBackup = `$isetoolsPath/$cdISE $nextDate`;
	#If data exists for the date -- Do we need to inform the user ?  !
	if(length($dirToBackup) != 0 ){
	    print "DirToBackup : $dirToBackup\n";
	    #Compute the disk size
	    my $dirToBackupSize = `du -sb $dirToBackup`;
	    # Can't think of a better way for the following couple lines.
	    my @temp = split(/\s+/ , $dirToBackupSize);
	    $dirToBackupSize = $temp[0];

	    $sumTotal += ($dirToBackupSize / (1024 * 1024 * 1024) ); # Magic Numbers!
	    print " Total BackupSize is  $sumTotal \n ";
	    
	    if($sumTotal < $TAPEMAX ){
		#Make sure that all files in the directory (to be backed) have read permissions
	
		#my @files = readdir $dirToBackup or die print "Cannot read directory $dirToBackup $!";
		#Open Dir before reading
		
		$dirToBackup =~ s/\s+$//g;  #Trim trailing whitespaces. Needed!
		print "dirToBackup : $dirToBackup\n ";
		opendir(DIR , "$dirToBackup") or die print " Cannot open $dirToBackup: $!\n";
		while (my $file = readdir(DIR)){
		    if (!(-r "$dirToBackup/$file") ) {
			print "\n";
			print "$dirToBackup/$file does not have the necessary read permissions\n";
			print "Please correct this and re-execute the program. Aborting...\n";
			rollBackAndExit();
		    }
		}
		#Create a link to the data.
		#symlink "$dirToBackup","$backupPath/lnkd_$nextDate"  or die print " Cannot create a symlink $!\n";
		`ln -s $dirToBackup $backupPath/lnkd_$nextDate`;
		chomp $nextDate;
		chomp $dirToBackup;
		push @dates, $nextDate;
		push @paths, $dirToBackup;
		closedir(DIR);
	    }
	}
	
	#Get next Date. 
	$nextDate = `perl $getNextDateProg $nextDate`;
	print "nextDate is $nextDate\n";
    }until($sumTotal >= $TAPEMAX || $nextDate eq $today || $nextDate gt $endDate );
    
    print "Total size = $sumTotal\n";

    # Doesn't fill the tape, Abort. 
    if ($sumTotal < $TAPEMAX) {
	print " Not enough Data to fill the tape. Aborting...\n";
	exit;
    }

#     # Prepare for next tape. Start where last left off. 
#     if($nextDate < $endDate) 
# 	{
# 	    $startDate = $endDate; 
# 	}
     # Return references
    return (\@dates, \@paths); 
}

################################################################

# Display listing of the sym links  

################################################################

sub displayListing{
    
    # Get dates and paths arrays-- Perhaps, a map would've made better sense  
    my($dateref, $pathref) = @_; # Get the references
    my @dates = @{$dateref }; # Dereference
    my @paths = @{$pathref};

    print "List of dates:\n";
    #Display the listing of dates to the user  
    foreach my $date(@dates){
	print "$date\n";
    }   
    my $logFileName = "$bhistoryPath/tape_$dates[0]-$dates[-1]";
    print "logFileName : $logFileName\n";
    #Listing of Paths
    foreach my $path(@paths){
	print "$path\n";
    }
    
    return $logFileName;    
}
      



################################################################
#
#                       Main
##
#
################################################################

#processArgs "$@"

validateUserAndMachine();
my ($startDate, $endDate) = validateStartAndEndDates();

my ($dateref, $pathref) = createListing($startDate,$endDate);
my @dates = @{$dateref};
my @paths = @{$pathref};
#Handle the abort in createListing 

my $logFileName = displayListing(\@dates, \@paths);


# Tape Backup Process

my $CHOICE ='';

do{
    print "Insert tape into the drive $drivenum\n";
    printf "Ready to continue? [Y, <or N to abort>]:";
    $CHOICE = <STDIN>;
    chomp $CHOICE;
    if ($CHOICE eq "Y" || $CHOICE eq  "y" || $CHOICE eq "1"){
	$CHOICE = "y";
    }
    elsif($CHOICE eq "N" || $CHOICE eq "n" || $CHOICE eq "0"){
	print " Aborting on user request... \n";
	exit;
    }
    else{
	print "Could not understand your choice. Retrying...\n";
    }

}until($CHOICE eq "y");


# Mount and rewind tape 
print "Rewinding tape...";
`sudo mt -f $drivePath rewind`; 
print "Rewinding done\n\n";


print "Backing up: This may take several hours...\n";

# For every entry in the backup directory, backup
## Compress files
chomp $backupPath;
my $i = 1;
opendir(DIR,$backupPath);

while(my $link =  readdir(DIR)){
    next if $link =~ /^\./;
    print "Link : $link\n";
    `sudo tar -cvhf $drivePath "$backupPath/$link" 2>&1 | tee -a $logFileName`;
    print "$paths[$i]\n";
    `sudo touch "$paths[$i]/backedup"`;
    `sudo  chown ise:ise "$paths[$i]/backedup"`;
    $i += 1;
} 
closedir(DIR);
#Log the results
open FILE, ">>", $logFileName or die print"Cannot open log $!";
print FILE "Backup finished normally";
print FILE "Log has been created.";
print FILE "Remove the tape from drive $drivenum and label...";
print FILE "Done";
close FILE;

