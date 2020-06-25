//----------------------------------------------------
// strpos
//----------------------------------------------------
int strpos(char *needle, char *haystack) {
char *offset;
        offset=strstr(haystack,needle);
        if(offset !=NULL) {
                return offset-haystack;
        }
        return -1;
}
