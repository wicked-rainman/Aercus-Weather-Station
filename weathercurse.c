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

int strpos(char *, char *); 
void load2dArray(int, int, char*, char*);
void updateScreen(int ,int ,char *,char *,int , int );

int main(int argc, char *argv[]) {

        int i,j,sock,startpos,recvStringLen,humidity;
        WINDOW *wnd;
	char recvString[MAXRECVSTRING+1];
	char blankLine[70];
	char values[FIELDCOUNT+1][100];
	char names[FIELDCOUNT+1][100];
	char progressBar[22][40];
	float temp,pressure,windspeed;
 	unsigned short broadcastPort=BROADCASTPORT;
	struct sockaddr_in broadcastAddr;
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
    	broadcastAddr.sin_family = AF_INET;                 
    	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    	broadcastAddr.sin_port = htons(broadcastPort);   
	if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0) {
		fprintf(stderr,"Could not bind to listening port\n");
		exit(EXIT_FAILURE);
	}
        wnd=initscr();
	start_color();
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(ORANGE,203, COLOR_BLACK);
        clear();
	while(1) {
		recvStringLen=0;
                recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0);
                if(recvStringLen>0) {
			startpos=0;
                        recvString[recvStringLen] = '\0';
	        	for(j=0;j<FIELDCOUNT-1;j++) {
                		memset(values[j],0,100);
                		i=strpos(",",recvString+startpos);
                		strncpy(values[j],recvString+startpos,i);
                		startpos+=(i+1);
        		}
        		strcpy(values[j],recvString+startpos);

			if(strcmp(values[1],"002") ==0) colOffset=35;
			else colOffset=0;
		
			//Station ID
			updateScreen(linePos++,colOffset,names[0],values[0],COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));

                        //Software Version
			updateScreen(linePos++,colOffset,names[1],values[1],COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));

                        //DTG
			updateScreen(linePos++,colOffset,names[2],values[2],COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));

			//Temperature
			temp=strtof(values[3],NULL);
			if(temp>30) updateScreen(linePos++,colOffset,names[3],values[3],
				COLOR_PAIR(GREEN),COLOR_PAIR(RED));
			else if(temp>25) updateScreen(linePos++,colOffset,names[3],values[3],
				COLOR_PAIR(GREEN),COLOR_PAIR(ORANGE));
			else if(temp>20) updateScreen(linePos++,colOffset,names[3],values[3],
				COLOR_PAIR(GREEN),COLOR_PAIR(YELLOW));
			else if(temp>10) updateScreen(linePos++,colOffset,names[3],values[3],
				COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));
			else updateScreen(linePos++,colOffset,names[3],values[3],
				COLOR_PAIR(GREEN),COLOR_PAIR(BLUE));

			//Humidity
			humidity=atoi(values[4]);
			if(humidity>90) updateScreen(linePos++,colOffset,names[4],values[4],
				COLOR_PAIR(GREEN),COLOR_PAIR(BLUE));
			else if(humidity>50) updateScreen(linePos++,colOffset,names[4],values[4],
				COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));
			else updateScreen(linePos++,colOffset,names[4],values[4],
				COLOR_PAIR(GREEN),COLOR_PAIR(YELLOW));

                      	//Pressure
                        pressure=strtof(values[5],NULL);
                        if(pressure>999) updateScreen(linePos++,colOffset,names[5],values[5],
				COLOR_PAIR(GREEN),COLOR_PAIR(WHITE));
                        else updateScreen(linePos++,colOffset,names[5],values[5],
				COLOR_PAIR(GREEN),COLOR_PAIR(RED));

			//Windspeed
			windspeed=strtof(values[6],NULL);
			if(windspeed>20) updateScreen(linePos++,colOffset,names[6],values[6],
				COLOR_PAIR(GREEN),COLOR_PAIR(RED));
                        else updateScreen(linePos++,colOffset,names[6],values[6],
				COLOR_PAIR(GREEN),COLOR_PAIR(WHITE));

			//Wind direction
			updateScreen(linePos++,colOffset,names[7],values[7],COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));

                        //Rain
			sprintf(values[8],"%.1f",(strtof(values[8],NULL)*25.4));
			updateScreen(linePos++,colOffset,names[8],values[8],COLOR_PAIR(GREEN),COLOR_PAIR(WHITE));

			//UVA
			updateScreen(linePos++,colOffset,names[9],values[9],COLOR_PAIR(GREEN),COLOR_PAIR(GREEN));

			//Input line (Debug)
			//attron(COLOR_PAIR(WHITE));
			//move(linePos,1);
			//mvaddstr(linePos,1,blankLine);
			//mvaddstr(linePos,1,recvString);

			refresh();
			linePos=1;
                }

	}
	endwin();
	return 0;
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
        for(k=0;k<fieldcount-1;k++) {
                i=strpos(",",fields+startpos);
                memset(ary+(k*len),0,len);
                strncpy(ary+(k*len),fields+startpos,i);
                startpos+=(i+1);
        }
        strcpy(ary+(k*len),fields+startpos);
}
//----------------------------------------------------------
void updateScreen(int linePos,int colOffset,char *name,char *value,int atr1, int atr2) {
        attron(atr1);
	mvaddstr(linePos,colOffset+1,name);
	attron(atr2);
        mvaddstr(linePos,colOffset+15,value);
	refresh();
	usleep(sleeptime);
}

