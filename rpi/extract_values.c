//---------------------------------------------------
// Extract_values
//---------------------------------------------------
void extract_values(char *input_line, char *start_needle, char *end_needle, char *ret_value) {
//char fieldname[20];
char field_value[20];
int fieldname_start, fieldname_end;
int value_end,needle_length;
        needle_length=strlen(start_needle);
        fieldname_start=strpos(start_needle, input_line);
        if(fieldname_start>0) {
                fieldname_end=fieldname_start+needle_length;
                value_end=strpos(end_needle, input_line+fieldname_end);
                if(value_end>0) {
                        strncpy(ret_value,input_line+fieldname_end, value_end);
                        ret_value[value_end]=0;
                }
                else {
                        strcpy(ret_value,"Unk");
                }
        }
        else {
                strcpy(ret_value,"Unk");
        }
}
