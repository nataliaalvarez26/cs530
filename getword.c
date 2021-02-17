/*
Natalia Alvarez
CS570 
Professor Carroll
*/
#include "getword.h"
#include <stdio.h>
#include <stdbool.h>
 #include <stdio.h>
#include <stdlib.h>
#include "p2.h"
int greaterthan;
int pipecheck;
int doublelessthan;
int tildecheck;
int lessthan;
int lesscount;
int thanless; 
int dollarcheck;
int lastamp;
int backslashtrig;
int firstamp;
int backamp;

int currChar;
int nextChar;
char *name = "HOME";
char *homedir;
bool backslash =false;
bool tilde=false;
bool triggered= false;
bool dollar= false;
bool foundDol= false;
int n=0;
bool TEST; 
int getword(char *w)
{
        lesscount=0;
        ///int currChar;
        //int nextChar;
        name = "HOME";
        homedir = getenv(name);
        backslash =false;
        tilde=false;
        triggered= false;
        dollar= false;
        foundDol= false;
        n=0;
        
        /*declaring the home variable for when we encounter a ~, getenv also gets the path of the variable we look for in this case we are looking for the HOME path. */
 
        TEST=true; 
        while ( ( currChar = getchar() ) != EOF ){
                if(n==254){
                        *w='\0';
                        return n;       
                }
                /*checking if theres only one <*/
                if(*(w-1) == '<' && n==1){
                        lessthan=1;
                }
                /*stops the metacharacter if theres is a double backslash*/
                if(backslash==true && currChar=='\\'){
                                backslash=false;
                                n++;
                               *w=currChar; 
                                w++;
                                continue;
                        }
                /*0 checking for space*/   
                if(currChar == ' '){
                        /*if \ before then we just add to w*/
                        if(backslash==true){
                                backslash=false;
                                n++;
                                *w= currChar;
                                w++;
                        }      
                        /*if n>0 then its a word*/
                        if(n>0){
                                *w=('\0');
                                /*if $ found then the word is negative*/
                                if(dollar){
                                        *w='\0';
                                        return n*(-1);
                                }
                                return n;
                        }
                        /* if boolean triggered and size is 0 then we just return n*/
                        else if( foundDol && n==0){
                                dollarcheck=1;
                                *w=('\0');
                                return n;
                        }
                        else{
                                continue;
                        }
                }
                /*checking for a tab*/
                else if(currChar =='\t' ){
                        if(n>0){
                                *w=('\0');
                                /*checking if word is negative*/
                                if(dollar){
                                        *w='\0';
                                        return n*(-1);
                                }
                                return n;
                        }
                         else if( foundDol && n==0){
                                *w=('\0');
                                return n;
                        }
                        else{
                                continue;
                        }
                }
                /*checking for a new line*/
                else if(currChar == '\n'){
                         /*checking for backslash+newline*/
                        if(backslash==true ){
                                backslash=false;
                                *w='\0';
                                continue;
                        }
                        if(*(w-1) == '&'){
                                if(backamp==1){
                                        *w='\0';
                                        lastamp=0;
                                        return n;
                                }
                                else{
                                        *(w-1)='\0';
                                        lastamp=1;
                                }
                        }
                        if((foundDol) && n==0){
                                /*ungetc will push back the currChar back to the input to read again*/
                                ungetc('\n',stdin);
                                *(w)= '\0';
                                return n;
                                break;
                        }
                        /*checking negative word*/
                        if(dollar){
                                *w='\0';
                                ungetc('\n',stdin);
                                return n*(-1);
                        }
                        if(n==0){
                                *(w)= '\0';
                                return 0; 
                                break;
                        }
                        else if(n>0){
                                *w='\0';
                                ungetc('\n', stdin);
                                return n;
                        }
                        *(w)= '\0';
                        /* will unget the newline if it is not triggered in the if loops*/
                        ungetc('\n',stdin);
                        return n;
                }
                /*checking if previous is $*/
                else if(foundDol){
                        /* checks for $$*/
                         if(currChar ==36){
                                if(n==0){
                                        dollar =true;
                                        n++;
                                        *w= currChar;
                                        w++;
                                }
                        }
                        /*it will make sure that the metacharacter isnt triggered if $ before it*/
                        else if(currChar =='~'){
                                dollar=true;
                                n++;
                                *w=currChar;
                                w++;
                                foundDol=false;
                                continue;
                        }      
                        else{
                                dollar =true;
                                n++;
                                *w= currChar;
                                w++;
                        
                        }
                        foundDol=false;
                }      
                /*checking for >*/
                else if(currChar== '>'){
                        /*makes sure that the metacharacter isnt triggered and just added to w*/
                        if(backslash==true){
                               backslash=false;
                               n++;
                               *w=currChar;
                               w++;
                               continue;
                        }
                        if(n>0){
                                
                                *w='\0';
                                ungetc('>', stdin);
                               return n;
                                 break;
                        }
                        else if(n==0){
                                greaterthan=1;
                        }
                }
                /*checkig for < */
                else if(currChar== '<'){
                        nextChar=getchar();
                        if(lessthan==1){
                                if(*(w-1)!= '<'){
                                         lessthan=1;
                                 //      printf("lessthan check= %d", lessthan);
                                        continue;
                                }
                        }
                        /*if theres a \ before then metacharacter ignored*/
                        if(backslash==true){
                                ungetc(nextChar, stdin);
                                /*set to false to it wont trigger again*/
                                backslash=false;
                                triggered=true;
                                n++;
                                *w=currChar;
                                w++;
                                continue;
                        }
                        /*check to see if theres something behind it and whether its a < or a non-metacharacter*/
                        if(n>0){
                                 if(*(w-1)== '<'){
                                        if(triggered == true){
                                                *w='\0';
                                                ungetc(currChar, stdin);
                                                return n;
                                        }
                                        else{
                                        ungetc(nextChar, stdin);
                                        *w=currChar;
                                        n++;
                                        w++;
                                        }
                                }
                                else{
                                        if(nextChar=='<'){
                                                thanless=1;
                                        }
                                        ungetc(nextChar, stdin);
                                //      printf("theres a word");
                                        *w='\0';
                                        ungetc(currChar, stdin);
                                        return n;
                                }
                        }
                        /*if n=0 then it is the first < we are seeing*/
                        else if(n==0){
                                if(nextChar=='<'){
                                        thanless=1;
                                        ungetc(nextChar, stdin);
                                        doublelessthan=1;
                                //      printf("double check= %d", thanless); 
                                }
                                else{
                                        ungetc(nextChar, stdin);
                                        lessthan=1;
                                }
                                continue;
                        }      
                }      
                /*checcks for the first backslash to set boolean*/
                else if(currChar =='\\'){
                                if(tilde ==true){
                                        continue;
                                }if(n>0){
                                        *w='\0';
                                        ungetc(currChar, stdin);
                                        return n;
                                        break;
                                }
                                else{
                                        backslashtrig= 1;
                                        backslash=true;
                                        continue;
                                }
                }
                /*checking for |*/
                else if(currChar == '|'){
                        /*checking whether we see a | or a non-metacharacter and will terminate*/
                        if(n>0){
                                if(*(w-1)== '|'){
                                        *w='\0';
                                        return 1;
                                }
                                else{
                                        *w='\0';
                                        ungetc('|', stdin);
                                        return n;
                                        break;
                                }
                        }
                        else if(n==0){
                                *w=currChar;
                                n=1;
                                w++;
                        }
                }
               /*chechking if the previous is | so then return is triggered*/
                else if(*(w-1) == '|'){
                        pipecheck=1;
                        *w='\0';
                        ungetc(currChar, stdin);
                        return 1;
                }
                /*checking for &*/
                else if(currChar == '&'){
                        if(backslash==true){
                                n++;
                                *w=currChar;
                                w++;
                                backslash=false;
                                backamp=1;
                                continue;

                        }
                        if(n>0){
                                *w='\0';
                                ungetc(currChar,stdin);
                                return n;
                                break;
                        }
                        else if(n==0){
                                firstamp=1;
                                *w=currChar;
                                n=1;
                                w++;            
                        }
                } 
                /*chechking if the previous is & so then return is triggered*/
                else if(*(w-1) == '&'){
                        if(backslash==true){
                                ungetc(currChar, stdin);
                                return 1;
                        }
                        else{
                                ungetc(currChar, stdin);
                                continue;
                        }
                }
                /*checking for ~ so it will append the string*/
                else if(currChar == '~'){
                        tilde=true;
                        /*will stop metacharacter if \ is true*/
                        if(backslash==true){
                                backslash=false;
                                n++;
                                *w=currChar;
                                w++;
                                continue;
                        }
                        /* is n=0 then its the first ~ we are seeing*/
                        if(n==0){
                                tildecheck=1;
                                continue;
                        }
                        else if(n>0){
                                n++;
                                *w=currChar;
                                w++;
                                continue;
                        }
                }      
                /*catches anything but a space, tab or newline*/
                else{
                        /*checks if currChar is a $*/
                        if(currChar == 36) {
                                if(backslash==true){
                                        backslash=false;
                                        n++;
                                        *w=currChar;
                                        w++;
                                        continue;
                                }
                                /*if n==0 then first $*/
                                if(n==0){
                                        foundDol=true;
                                        continue;
                                }
                                else{
                                        n++;
                                        *w= currChar;
                                        w++;
                                }
                        }
                        else{
                                if(backslash==true && currChar == 65){
                                        n++;
                                        *(w-1)=currChar;
                                        continue;
                                }
                                else{
                                n++;
                                *w= currChar;
                                w++;
                                }
                        }
                }
                /*making sure that the length of the string isnt >STORAGE*/
                if(n==(STORAGE-1)){
                        *w=='\0';
                        ungetc(currChar, stdin);
                        break;
                }
        }
        /*checks to see if $ is true so negative word*/
        if(dollar){
                /*makes sure words get terminated at the end*/
                *w='\0';
                return n*(-1);
        }
        /*checking for EOF*/
        if(((currChar = getchar()) == EOF)){
                 if(foundDol == true){
                        return n;
                }
                if(n==0){
                        *w= ('\0');
                        return -255;
                }
        }
        return n;
}
