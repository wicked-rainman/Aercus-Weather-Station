//-----------------------------------------------------
// printchar - Output a character to matrix one or two
//-----------------------------------------------------

void printchar(int fd, int x, int chr) {

        int i, y, z, odd;
        char buf[7], reg;
        uint8_t cmd[2];
        uint8_t matrix;
        int rval;
        if(fd<0) return;

        //Pick which driver chip is going to get the data,

        if(x<2) matrix=0x61;
        else if(x<4) matrix = 0x62;
        else matrix = 0x63;

        //Select the chosen chip
        rval=ioctl(fd, I2C_SLAVE,matrix);
        if(rval<0) {
                syslog(LOG_NOTICE,"ioctl (printchar.c) returned %d\n",rval);
                exit(EXIT_FAILURE);
        }

        //Pick which display register is going to be
        //written to

        if((x % 2)==1) {
                reg=0x0e;
                odd=1;
        }
        else {
                reg=0x01;
                odd=0;
        }

        //If the character is out of printable range,
        //ditch writing it.

        if(chr<33 || chr>126) return;

        //Subtract 33 from the character value to
        // give a base index into the font array

        chr=chr-33;

        //Now load the bit pattern for the character into
        //the chosen display.
        if (odd==1) {
                // Odd numbered character out of the string
                // Reverse the bit pattern row by row.

                memset(buf,0,7);
                for (i = 0; i < 8; i++) {
                        for (y = 0; y < 7; y++) {
                                buf[i] |= font[chr][y] & (1 << i) ? 1 << y : 0;
                        }
                }
                for (i = 0; i < 7; i++) {
                        cmd[0]=reg+i;
                        cmd[1]=buf[i];
                        rval=write(fd, cmd, 2);
                        if(rval<0) {
                                syslog(LOG_NOTICE,"write (printchr.c) returned %d\n",rval);
                                exit(EXIT_FAILURE);
                        }
                }
        }
        else {
                // Even numbered character out of the string

                for (i = 0; i < 7; i++) {
                        cmd[0]=reg+i;
                        cmd[1]=font[chr][i];
                        rval=write(fd, cmd, 2);
                        if(rval<0) {
                                syslog(LOG_NOTICE,"write (printchr.c) returned %d\n",rval);
                                exit(EXIT_FAILURE);
                        }
                }
        }
}
