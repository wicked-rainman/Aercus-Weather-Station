#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <syslog.h>
#define MAXRECVSTRING 255
#define BROADCASTPORT 5678
#define FIELDCOUNT 11
#define GREEN 1
#define WHITE 2
#define RED 3
#define ORANGE 4
#define YELLOW 5
#define BLUE 6
#define sleeptime 50000

static int strpos(char *, char *);
static void load2dArray(int, int, /*@out@*/ char*, char*);
static void updateScreen(int ,int ,char *,char *,int , int );

int main(/*@unused@*/ int argc,  /*@unused@*/ char *argv[]) {

        int i,j,sock,startpos,recvStringLen,humidity;
        char recvString[MAXRECVSTRING+1];
        char blankLine[70];
        char values[FIELDCOUNT+1][100];
        char names[FIELDCOUNT+1][100];
        float temp,pressure,windspeed;
        uint16_t broadcastPort=BROADCASTPORT;
        struct sockaddr_in broadcastAddr;
        struct sockaddr_in iDontKnow;
        socklen_t iDontCare = (socklen_t) sizeof(iDontKnow);
        int linePos=1;
        int colOffset=0;
        memset(blankLine,32,70);
        load2dArray(100, FIELDCOUNT,names[0],"Version: ,StationID: ,DTG: \
        ,Temperature: ,Humidity: ,Pressure: ,WindSpeed: ,WindDir: \
        ,Rainfall: ,UVA: ,NotWanted:");

        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                fprintf(stderr,"Could not open listening port\n");
                exit(EXIT_FAILURE);
        }
        memset(&broadcastAddr, 0, sizeof(broadcastAddr));
        memset(&recvString,0,MAXRECVSTRING+1);
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        broadcastAddr.sin_port = htons(broadcastPort);
	if (bind(sock, (struct sockaddr *) &broadcastAddr, (socklen_t) sizeof(broadcastAddr)) < 0) {
                fprintf(stderr,"Could not bind to listening port\n");
                exit(EXIT_FAILURE);
        }
        (void) initscr();
        (void) start_color();
        (void) init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        (void) init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
        (void) init_pair(RED, COLOR_RED, COLOR_BLACK);
        (void) init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
        (void) init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        (void) init_pair(ORANGE,203, COLOR_BLACK);
        (void) clear();
        while( (bool) 1) {
                recvStringLen=0;
                recvStringLen = (int) recvfrom(sock,recvString, MAXRECVSTRING, 0,(struct sockaddr*)&iDontKnow ,&iDontCare );
                if(recvStringLen>0) {
                        startpos=0;
                        recvString[recvStringLen] = '\0';
                        for(j=0;j<FIELDCOUNT-1;j++) {
                                memset(values[j],0,100);
                                i=strpos(",",recvString+startpos);
                                strncpy(values[j],recvString+startpos,(size_t) i);
                                startpos+=(i+1);
                        }
                        strcpy(values[j],recvString+startpos);

                        if(strcmp(values[1],"002") ==0) colOffset=35;
                        else colOffset=0;

                        //Station ID
                        updateScreen(linePos++,colOffset,names[0],values[0], (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));

                        //Software Version
                        updateScreen(linePos++,colOffset,names[1],values[1],(int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));

                        //DTG
                        updateScreen(linePos++,colOffset,names[2],values[2],(int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));


                        //Temperature
                        temp=strtof(values[3],NULL);
                        if(temp>30) updateScreen(linePos++,colOffset,names[3],values[3],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(RED));
                        else if(temp>25) updateScreen(linePos++,colOffset,names[3],values[3],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(ORANGE));
                        else if(temp>20) updateScreen(linePos++,colOffset,names[3],values[3],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(YELLOW));
                        else if(temp>10) updateScreen(linePos++,colOffset,names[3],values[3],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));
                        else updateScreen(linePos++,colOffset,names[3],values[3],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(BLUE));

                        //Humidity
                        humidity=atoi(values[4]);
                        if(humidity>90) updateScreen(linePos++,colOffset,names[4],values[4],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(BLUE));
                        else if(humidity>50) updateScreen(linePos++,colOffset,names[4],values[4],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));
                        else updateScreen(linePos++,colOffset,names[4],values[4],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(YELLOW));

                        //Pressure
                        pressure=strtof(values[5],NULL);
                        if(pressure>999) updateScreen(linePos++,colOffset,names[5],values[5],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(WHITE));
                        else updateScreen(linePos++,colOffset,names[5],values[5],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(RED));

                        //Windspeed
                        windspeed=strtof(values[6],NULL);
                        if(windspeed>20) updateScreen(linePos++,colOffset,names[6],values[6],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(RED));
                        else updateScreen(linePos++,colOffset,names[6],values[6],
                                (int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(WHITE));

                        //Wind direction
                        updateScreen(linePos++,colOffset,names[7],values[7],(int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));

                        //Rain
                        (void) snprintf(values[8],10,"%.1f",(strtof(values[8],NULL)*25.4));
                        updateScreen(linePos++,colOffset,names[8],values[8],(int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(WHITE));

                        //UVA
                        updateScreen(linePos++,colOffset,names[9],values[9],(int) COLOR_PAIR(GREEN),(int) COLOR_PAIR(GREEN));

                        //Input line (Debug)
                        //attron(COLOR_PAIR(WHITE));
                        //move(linePos,1);
                        //mvaddstr(linePos,1,blankLine);
                        //mvaddstr(linePos,1,recvString);

                        (void) refresh();
                        linePos=1;
                }

        }
}
//---------------------------------------------------------
int strpos(char *needle, char *haystack) {
char *offset;
        offset=strstr(haystack,needle);
        if(offset !=NULL) return offset-haystack;
        return -1;
}
//---------------------------------------------------------
void load2dArray(int len, int fieldcount,char *ary ,char* fields) {
int k,i;
int startpos=0;
char *ptr;
        for(k=0;k<fieldcount-1;k++) {
                i=strpos(",",fields+startpos);
                memset(ary+(k*len),0,(size_t) len);
                ptr=ary+(k*len);
                //strncpy(ary+(k*len),fields+startpos,(size_t) i);
                strncpy(ptr,fields+startpos,(size_t) i);
                startpos+=(i+1);
        }
        ptr=ary+(k*len);
        strcpy(ptr,fields+startpos);
}
//----------------------------------------------------------
void updateScreen(int linePos,int colOffset,char *name,char *value,int atr1, int atr2) {
        (void) attron(atr1);
        mvaddstr(linePos,colOffset+1,name);
        (void) attron(atr2);
        mvaddstr(linePos,colOffset+15,value);
        (void) refresh();
        (void) usleep(sleeptime);
}


