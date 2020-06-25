//-------------------------------------------------------------------------
// bright - Change the light intensity for the display
// 0x19 is the pulse width modulation register in the IS31FL3730 chip.
// 0x0d is the Lighting Effect register (current)
// There are 3 chips on board, 0x61, 0x62 and 0x63
// Valid values for PWM are not obvious, but through trial and error, 0-7
// seem to work. Anything after 8 seems to make no difference.
// Valid values for current :
//      0 = 40ma ..... 7 =75ma
//      8 = 5ma .......14 =35ma
//
// WR
//-------------------------------------------------------------------------
void bright(int fd, int cur, int pwm) {
        uint8_t buf[2],current[2],k,chip_addr=0x61;
        int rval;
        if(fd<0) return;                                //Device hasn't been opened
        if(cur>7) cur=7;                                //Don't go over 7              
        if(pwm>7) pwm=7;                                //A guess
        buf[0]=0x19;                                    //Brightness PWM reg
        buf[1]=pwm;                                     //pwm value
        current[0]=0x0d;                                //Lighting effect reg
        current[1]=cur;                                 //0x00 is 40ma 0x07 is 75ma
        for(k=0;k<3;k++) {                              //Deal with the 3 chips
                rval=ioctl(fd, I2C_SLAVE,chip_addr+k);       //Select the chip
                if(rval<0) {
                        syslog(LOG_NOTICE,"ioctl (bright.c) returned %d writing to chip %d\n",rval,k);
                        exit(EXIT_FAILURE);
                }
                rval=write(fd,current,2);                    //Write current value
                if(rval<0) {
                        syslog(LOG_NOTICE,"write (bright.c) returned %d writing to chip %d\n",rval,k); 
                        exit(EXIT_FAILURE);
                }
                rval=write(fd,buf,2);                        //Write pwm value
                if(rval<0) {
                        syslog(LOG_NOTICE,"write (bright.c) returned %d writing to chip %d\n",rval,k);
                        exit(EXIT_FAILURE);
                }

        }
}
