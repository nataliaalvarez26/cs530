/*
Natalia Alvarez
CS570 
Professor Carroll
Due Date: 11/29/2020
Program 4 will mimic a basic shell and handles ~, <, >, |, <<, $. More information is found in comments below
*/
#include "p2.h"
#define FILENAME "/etc/passwd"

/* Declarations of all variables used*/
int output_fd;
int input_fd;
FILE *lessless_fd;
char *tempchar;
char *delimiter;
int cdflag;
char *cdstring;
char *newargv[MAXITEM];
int newargc;
char bigbuff[SIZE];
char cp[20];
char *dirctoget;
char *dirctochange;
char newhome[]="//";
char *we = "/";
char *semi= ":";
int dirc;
FILE *fp;
bool varUn=false;
bool nohere=false;
bool mutipleless= false;
int dollarptr;
int peak;
int forkcount;
int wordsize;
int devnull;
int kidpid;
int fildes[20];
int newoff=1;
int pipelocation[10];
int locationcount=0;
int pipecount;
int environtrig;
char *ban[10];
int bancount;
char *bantok;
char *save;
char *cdarray[20];
int cdarraycount;
char *token;
char *tiltok;
char *tilarray[20];
int tilcount;
bool firstcom=false;
bool tilcheck=false;
char *tilname;
char *nextline;
char *less;
char *copy;
int buffptr;
char *tempfd= "tempfile";
char *outptr;
char *inptr;
int pipeptr;
pid_t pid;
char meta;
int linesize;
int linesize2;
size_t longersize= 100;
size_t size = 10;
char *line;
char *line2;
int num;
char *prevcd;
char prompt[]= ":570: ";
int i,j,z, w, h;
pid_t first, second, third;

/* Handles the killpg call but is empty because we don't want it to preform anything*/
void myhandler(int signum){

}

int parse(){
        /*reseting necesssary variables*/
        lessthan=0;
        outptr=NULL;
        inptr=NULL;
        lastamp=0;
        pipeptr=0;
        pipecount=0; 
        pipecheck=0;
        buffptr=0;
        tilcount=0;
        bancount=0;
        tilcheck=false;
        we = "/"; 
        i=0;
        newargc=0;
        for(;;) {
                /* will check if the << flag is set so it can getline and not just getword*/
                if(doublelessthan==2 ){
                        linesize2= getline(&line2, &longersize, stdin); 
                }
                /*will getword for any other case*/
                else{
                        wordsize=0;
                        wordsize=getword(bigbuff+buffptr);
                }
                /*set metacharacters after getting word*/
                meta= *(bigbuff+buffptr);
                /*will break if EOF*/ if(wordsize==-255) break;
                if(wordsize<0){
                        wordsize*=-1;
                        if(greaterthan==1){
                                greaterthan=0;
                                /*sets the output redirection pointer for the redirection later*/
                                outptr=getenv(bigbuff+buffptr);
                                wordsize=strlen(outptr);
                                buffptr+=(wordsize+1);
                                continue;
                        }
                        newargv[i]=getenv(bigbuff+buffptr);
                        /*checks is $ variable is undefined returns an error*/
                        if(newargv[i] == NULL){
                                varUn=true;
                                fprintf(stderr, "%s: Undefined variable.\n", bigbuff+buffptr);
                        }
                        i++;
                        newargc++;
                        buffptr+=(wordsize+1);
                        continue;
                }
                /*sets pipeline flag*/
                if(meta== '|'){
                        pipecount++;
                        pipecheck=1;
                }
                /*deals with << and adding to tempfile*/
                if(doublelessthan==2){
                        /*will add to the temp file*/
                        while(linesize2!= (strlen(delimiter)+1) ){
                                /*if linesize empty continue*/
                                if(linesize2==0){
                                        continue;
                                }
                                /* adds to the tempfile*/
                                fprintf(lessless_fd, "%s",line2);
                                linesize2= getline(&line2, &longersize, stdin);
                        }       
                        /* will not add to file if the string has more than just delimiter*/
                        if(( strstr(line2,delimiter) == NULL)){
                                fprintf(lessless_fd, "%s",line2);
                                doublelessthan=2;
                                continue;
                        }
                        else{
                                doublelessthan=0;
                        }
                }
                /*first deals with << and sets appropriate flags*/
                if(doublelessthan==1){
                        /* if there is < on line then error thrown*/
                        if(inptr!=NULL){
                                mutipleless=true;                                       
                                break;
                        }
                        inptr=tempfd;
                        lessthan=0;
                        delimiter = bigbuff+buffptr;
                        if(strcmp(delimiter, "-f")==0){
                                nohere= true;
                                doublelessthan=0;
                                lessthan=0;
                                break;
                        }
                        /* will break if the delimiter is NULL*/
                        if(strlen(delimiter)==0){
                                nohere=true;
                                doublelessthan=0;
                                lessthan=0;
                                break;  
                        }
                        /*flag for later so it keeps going through current line*/
                        doublelessthan=3;
                        /*will create and open file*/
                        if((lessless_fd= fopen(tempfd,"w")) < 0 ){
                                                perror(" fgt oops, open failed!");
                                                exit(1);
                                } 
                        else{
                        }       
                        buffptr+=(wordsize+1);
                        continue;
                }
                /*sets flag if there is a backslash+ampersand*/
                if(backamp==1){
                        /*breaks if its the last ampersand in line*/
                        if(newargc>0){
                                backamp=0;
                                newargv[i]=bigbuff+buffptr;
                                i++;
                                newargc++;
                                buffptr+=(wordsize+1);
                                break;
                        }
                        /*continues if its not the last ampersand*/
                        else {
                                backamp=0;
                                newargv[i]=bigbuff+buffptr;
                                i++;
                                newargc++;
                                buffptr+=(wordsize+1);
                                continue;
                        }
                }
                /*checks to see if there a & at the end of the line to trigger background children*/
                if(lastamp==1){
                        if(newargc>=1){
                                lastamp=2;
                                continue;
                        }
                        else{
                                newargv[newargc-1]=NULL;
                                lastamp=2;
                                continue;
                        }
                }
               /*will deal with ~*/
                if(tildecheck==1){
                        /* will get the word after tilde, ex ~cs570, tilname=cs570*/
                        tilname=bigbuff+buffptr;
                        strsep(&tilname, "/");
                        if(tilname!=NULL){
                                bantok= strtok((bigbuff+buffptr), "/");
                                ban[0]=bantok;
                                tilcheck=true;  
                        }
                        /*throw error is file isnt valuable*/
                        if ((fp = fopen(FILENAME, "r")) == NULL){
                                printf("Error! opening file");
                                 exit(1);
                        }
                        linesize= getline(&line, &size, fp);
                        /*will add the : back to ter*/
                        if(tilcheck!=true){
                                strcat(bigbuff+buffptr, semi);
                        }
                /* will go through /etc/passwd file to find username*/
                while(linesize>=0){
                        /*error thrown in linesize==0*/
                        if(linesize==0){
                                fprintf(stderr,"lookup failed, not valid username\n");
                                exit(2);
                        }
                        /* will check if line contains username*/
                        if((nextline= strstr(line, bigbuff+buffptr))!=NULL){
                                /*will search through line to find 6th element needed*/
                                tiltok =strtok(line, ":");
                                tilarray[0]=tiltok;
                                tilcount++;
                                /*will tokenize line*/
                                while( tiltok != NULL){
                                        tiltok= strtok(NULL, ":");
                                        if(tilcount==5){
                                                tilarray[tilcount]=tiltok;
                                        }
                                        tilcount++;
                                }
                                tilarray[4]=NULL;
                                break;
                        }
                        linesize= getline(&line, &size, fp); 
                        /*will throw error is user doesnt exist*/
                        if(linesize==-1){
                                fprintf(stderr," lookup failed, not valid username\n");
                                exit(2);
                        }
                }       
                        /*if we have cs570/Data4 it will concatinate 6th element to /Data4*/
                        if(tilcheck==true){
                                strcat(tilarray[5],we);
                                strcat(tilarray[5], tilname);
                                /*adds to newargv properly*/
                                newargv[i]=tilarray[5];
                                i++;
                                newargc++;
                                wordsize=sizeof(tilarray[5]);
                                buffptr+=(wordsize);
                        }
                        /*else will just add to newargv*/
                        else{
                                newargv[i]=tilarray[5];
                                i++;
                                newargc++;
                                wordsize=sizeof(tilarray[5]);
                                buffptr+=(wordsize);
                        }
                        /* will reset array for future use*/
                        for(h=0; h<=6;h++){
                                tilarray[h]=NULL;
                        }tilname=NULL;
                        tildecheck=0;
                        continue;
                }
                /* checks for < for input redirection*/
                if(lessthan==1){
                        lessthan=0;
                        /*sets the input redirection pointer for the redirection later*/
                        inptr= bigbuff+buffptr;
                        buffptr+=wordsize+1;
                        continue;
                }
                /* checks for > for output redirection*/
                if(greaterthan==1 ){
                        greaterthan=0;
                        /*sets the output redirection pointer for the redirection later*/
                        outptr=bigbuff+buffptr;
                        buffptr+=wordsize+1;
                        continue;
                }
                /*sets the pipeline flag and sets the place in newargv NULL*/
                if(pipecheck==1){
                        /* if \| then it will just add to newargv*/
                        if(backslashtrig==1){
                                backslashtrig=0;
                                newargv[i]="|";
                                i++;
                                newargc++;
                                buffptr+=2;
                                pipecheck--;
                                continue;
                        }
                        newargv[i]=NULL;
                        newargc++;
                        i++;
                        pipelocation[pipeptr]=i;
                        pipeptr++;
                        buffptr+=wordsize+1;
                        /*sets a different pipeline flag*/
                        pipecheck++;
                        continue;
                }
                /*checks to see if wordsize is empty*/
                if(wordsize==0){
                        /*will sent flag so that it can continue and not break since wordsize=0*/
                        if(doublelessthan==3){
                                doublelessthan=2;
                                continue;
                        }
                        meta= *( bigbuff+buffptr-2);
                        /*checks if the & is at the end of line and NULLs it*/
                        if(meta == '&'){
                                newargv[newargc-1]=NULL;
                                lastamp=2;
                        }
                        break;
                }
                /*will add the word to newargv*/
                else{
                        backslashtrig=0;
                        /*adds word to newargv*/
                        newargv[i]=bigbuff+buffptr;
                        i++;
                        newargc++;
                }
                /*increments buffer pointer according to wordsize and then plus one for the null*/
                buffptr+=(wordsize+1);
        }
        /*sets the last word to null*/
        newargv[newargc]=NULL;
}
int main() {
        /*signal handler set up*/
        (void) signal(SIGTERM, myhandler);
        setpgid(0,0);
        for(;;){
                /*prompt changes if directory is changed*/
                if(cdflag==1){
                        printf("%s", cdstring);
                }
                else{
                        printf("%s",prompt);
                }
                parse();
               /*reseting doublelessthan*/
                if(lessless_fd !=NULL){
                        doublelessthan=0;       
                }
                /*this will break for loop when EOF*/
                if(wordsize==-255 && newargc==0){
                        break;
                }
                /*next 3 if loops are flags for certain errors*/
                /*if $ variable undefined*/
                if(varUn==true){
                        varUn=false;
                        continue;
                }
                /* if < and << on same line*/
                if(mutipleless==true){
                        mutipleless=false;
                         fprintf(stderr, "can't have < and << on same line\n");
                        continue;       
                }       
                /* if there is no << delimiter*/
                if(nohere==true){
                        nohere=false;
                         fprintf(stderr, "hereis deliminator not properly set\n");
                        continue;      
                } 
                /*checks if line is empty*/
                if(newargc==0){
                        continue;
                }
                /*checks for change directory word*/
                if(strcmp(newargv[0], "cd") ==0 ){
                        /*checks if theres too many arguments for CD*/
                        if(newargc>2){
                                 fprintf(stderr, " too many arguments after cd\n ");
                        }
                        /*if word is just cd it will change to home directory*/
                        if(newargc==1){
                                dirc= chdir(getenv("HOME"));
                                cdflag=0;
                                /*will print effor if cd cant work*/
                                if(dirc==-1){
                                        perror("changing directory failed");
                                        continue;
                                }
                                /* specific case of "/" */
                                if(environtrig==1){
                                        cdstring="/:570: ";
                                        cdflag=1;
                                        environtrig=0;

                                }
                                /*will get current working directory so that it can concatinate to new prompt*/
                                else{
                                        if(getcwd(cp, sizeof(cp)) != NULL){
                                                cdflag=1;
                                                token =strtok(cp, "/");
                                                cdarray[0]=token;
                                                cdarraycount++;
                                                while( token != NULL){
                                                        token= strtok(NULL, "/");
                                                        cdarray[cdarraycount]=token;
                                                        cdarraycount++;
                                                }
                                                strcat(cdarray[cdarraycount-2], prompt);
                                                cdstring=cdarray[cdarraycount-2];
                                                cdarraycount=0;
                                                environtrig=0;
                                        
                                        }
                                        else{
                                                perror("getcwd () error");
                                        }
                                }
                        }
                        /*sets the correct path for cd if argument is given*/
                        if(newargc==2){
                                dirc=chdir(newargv[1]);
                                /*will print effor if cd cant work*/
                                if(dirc==-1){
                                        perror("changing directory failed");
                                        continue;
                                }
                                /* will concatinate new directory since it was changed*/
                                cdflag=1;
                                token =strtok(newargv[1], "/");
                                cdarray[0]=token;
                                cdarraycount++;
                                while( token != NULL){
                                        token= strtok(NULL, "/");
                                        cdarray[cdarraycount]=token;
                                        cdarraycount++;
                                }
                                strcat(cdarray[cdarraycount-2], prompt);
                                cdstring=cdarray[cdarraycount-2];
                                cdarraycount=0;
                        }
                        continue;
                }
                /* will handle environ*/
                if(strcmp(newargv[0], "environ") == 0){
                        /*will ignore &*/
                        if(lastamp==2){
                                lastamp=0;
                        }
                        /*if it is one argument*/
                        if(newargc==2){
                                /*will get environment to change*/
                                if(getenv(newargv[1])== NULL){
                                        fprintf(stderr, " \n");
                                }
                                else{
                                        fprintf(stdout, "%s\n", getenv(newargv[1]));
                                }
                        }
                        /* if 2 arguments*/
                        else if (newargc==3){
                                /*will set appropriate flags*/
                                if(strcmp(newargv[2], "/")==0){
                                        fprintf(stdout, "%s\n", newargv[2]); 
                                        environtrig=1;
                                }else{
                                        environtrig=2;
                                }
                                dirctochange= newargv[2];
                                /*will set new environment*/
                                if(setenv(newargv[1], newargv[2],1)==-1){
                                        fprintf(stderr, " \n");
                                }
                        }
                        else if(newargc >3){
                                fprintf(stderr, "too many arguments for environ ");
                        }
                        continue;
                }
                /*flushes stdout and stderr so that theres not unnecessary content */
                fflush(stdout);
                fflush(stderr);
/*pipeline section*/
if(pipecount==1){
if((first= fork())==0){
                        CHK(pipe(fildes+0));
                        CHK(second=fork());
                        if(second ==0){
                                CHK(dup2(fildes[1], STDOUT_FILENO));
                                CHK(close(fildes[0]));
                                CHK(close(fildes[1]));
                                 /*checks if input redirection is set and then will open file*/
                                if(inptr != NULL){
                                  /*open files and sets appropriate flags for opening files, if not error is thrown*/
                                    if((input_fd= open(inptr,O_RDONLY,S_IRUSR| S_IWUSR)) < 0 ){
                                              perror(" pgclt oops, open failed!");
                                              exit(1);
                                    }
                                  /*resets input pointer and then does dup2 on input redirection and closes the input_fd */
                                    inptr=NULL;
                                    dup2(input_fd, STDIN_FILENO);
                                   /*will remove tempfile created by <<*/
                                if(tempfd!=NULL){
                                        doublelessthan=0;
                                        remove(tempfd);
                                    }
                                    close(input_fd);
                               }
                                CHK(execvp(newargv[0], newargv));
                        }
                        CHK(dup2(fildes[0],STDIN_FILENO));
                        CHK(close(fildes[0]));
                        CHK(close(fildes[1]));
                         /*checks if output redirection is set and then will open file*/
                                if(outptr!= NULL){
                                /*open files and sets appropriate flags for opening files, if not error is thrown*/
                                       if((output_fd= open(outptr, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR | S_IWUSR)) < 0 ){
                                                 perror(" pgcgt oops, open failed!");
                                                 exit(1);
                                       }
                               /*resets output pointer and then does dup2 on output redirection and closes the output_fd */
                                       outptr=NULL;
                                       dup2(output_fd, STDOUT_FILENO);
                                       close(output_fd);
                                }
                                /*after all the pipelineing will do execvp in grand child*/
                                CHK(execvp(newargv[pipelocation[pipeptr-1]], newargv+pipelocation[pipeptr-1]));
        }
}
else if(pipecount>1){
    if((first= fork())==0){
        pipe(fildes+0);
        CHK(second= fork());
        /* grand child*/
        if(second==0){ 
            /*the loop of the grandchild*/
            for( j= 1; j<= pipecount; j++){
                pipe(fildes+(2*j)); /*will open approprate file descriptors as it loops*/
                if(j < pipecount){ 
                    CHK(third= fork());
                    /* if pid is 0 then it is the child so keep go back up to for loop to make another process*/
                    if(third==0){
                        continue; 
                    }
                    else{
                        /*dup2 all the necessary file descriptors*/
                        CHK(dup2(fildes[j*2],STDIN_FILENO));
                        CHK(dup2(fildes[j*2-1], STDOUT_FILENO));
                        /*close all the file descriptors*/
                        for(h=0; h < (j*2)+2; h++){
                            close(fildes[h]);
                        }
                        /*execvp the middle child*/
                        CHK(execvp(newargv[pipelocation[(pipecount-1)-j]], newargv+pipelocation[(pipecount-1)-j]));
                        }
                }
                /* the last child aka the first command code*/
                else{ 
                        /*checks if input redirection is set and then will open file*/
                    if(inptr != NULL){
                        /*open files and sets appropriate flags for opening files, if not error is thrown*/     
                        if((input_fd= open(inptr,O_RDONLY,S_IRUSR| S_IWUSR)) < 0 ){
                            perror(" pgclt oops, open failed!");
                            exit(1);
                        }
                        /*resets input pointer and then does dup2 on input redirection and closes the input_fd */
                         inptr=NULL;
                        dup2(input_fd, STDIN_FILENO);
                        close(input_fd);
                    }
                    CHK(dup2(fildes[j*2-1], STDOUT_FILENO));
                    for(h=0; h < (j*2+2); h++){
                        close(fildes[h]);
                    }
                    CHK(execvp(newargv[0],newargv));
                    }
            }
        }
        else{ 
            /*checks if output redirection is set and then will open file*/
            if(outptr!= NULL){
            /*open files and sets appropriate flags for opening files, if not error is thrown*/
                if((output_fd= open(outptr, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR | S_IWUSR)) < 0 ){
                    perror(" pgcgt oops, open failed!");
                    exit(1);
                }
            /*resets output pointer and then does dup2 on output redirection and closes the output_fd */
            outptr=NULL;
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            }
            CHK(dup2(fildes[0],STDIN_FILENO));
            CHK(close(fildes[0]));
            CHK(close(fildes[1]));
            CHK(execvp(newargv[pipelocation[pipecount-1]], newargv+pipelocation[pipecount-1]));
        }
    }
    else{
        for(;;){
            /*uses wait to wait for child to be NULL to then break*/
            CHK(pid=wait(NULL));
            if(pid==first){
                break;
            }
        }
    }
}

              else{
                        if (-1 == (kidpid = fork())) {
                        /* -1 indicates the fork was unsuccessful */
                                perror("Cannot fork");
                                exit(1);
                        }
                        /* fork() returns 0 to the newly-created child process */
                        else if (0 == kidpid) {
                                 /*checks if output redirection is set and then will open file*/
                                if(outptr!= NULL){
                                        /*open files and sets appropriate flags for opening files, if not error is thrown*/
                                        if((output_fd= open(outptr,  O_WRONLY|O_CREAT|O_EXCL, S_IRUSR | S_IWUSR)) < 0 ){
                                                perror(" fgt oops, open failed!");
                                                exit(1);
                                        }
                                        /*resets output pointer and then does dup2 on output redirection and closes the output_fd */
                                        outptr=NULL;
                                        dup2(output_fd, STDOUT_FILENO);
                                        close(output_fd);
                                }
                                /*checks if input redirection is set and then will open file*/
                                if(inptr != NULL){
                                        /*open files and sets appropriate flags for opening files, if not error is thrown*/
                                        if((input_fd= open(inptr, O_RDONLY,S_IRUSR| S_IWUSR)) < 0 ){
                                                perror(" flt oops, open failed!");
                                                exit(1);
                                        }
                                       /*resets input pointer and then does dup2 on input redirection and closes the input_fd */
                                         inptr=NULL;
                                        dup2(input_fd, STDIN_FILENO);
                                       /*will remove tempfile created by <<*/
                                         if(tempfd!=NULL){
                                                doublelessthan=0;
                                                remove(tempfd);
                                        }
                                        close(input_fd);
                                }
                                /*will do excevp on child, will throw error if failed*/
                                if(execvp(newargv[0], newargv)==-1){
                                        perror("execvp failed");
                                        exit(1);
                                }
                 /*if there is a background child and the input redirection is null, meaning we dont want background children returnign to the keyboard*/
                                if(lastamp ==2 && inptr==NULL){
                                        int devnull;
                                        /*checks to make sure it can be redirected to /dev/null*/
                                        if((devnull=open("/dev/null", O_RDONLY)) <0 ){
                                                perror( "Could not open /dev/null \n ");
                                                exit(1);
                                        }
                                        dup2(devnull, STDIN_FILENO);
                                        lastamp=0;
                                }
                        }
                        /*checks for & to continue parent process without waiting for child, prints out the process id*/
                        else if(lastamp ==2) {
                                lastamp=0;
                                printf( "%s [%d]\n",*newargv, kidpid);
                                continue;
                        }
                        /*lastly will wait for child process to complete before parent is done*/
                        else{
                                for(;;){
                                        /*uses wait to wait for child to be NULL to then break*/
                                        CHK(pid=wait(NULL));
                                        if(kidpid==pid){
                                                break;
                                        }
                                }
                        }
                }
        }
        /*kills all processes in shell and uses SIGTERM to end*/
        killpg(getpgrp(), SIGTERM);
        printf("p2 terminated.\n");
        exit(0);
}
