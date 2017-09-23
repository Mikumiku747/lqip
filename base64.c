/**
 * @file: base64.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Zouhir Chahoud @student_id: 11763745
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @achieved:
 *
 * @pending:
 *
 * @note:
 *
 * @TODO: Check return types from other developers
 *
 */

 #include "base64.h"

 /**
 * 64 character table for converting to Base64
 */
char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Implementation of decode
 *
 * @notes:
 */
 void decode(char *b64_input, char *output_str) {
    int c, phase, i;
    unsigned char in[4];
    char *p;

    output_str[0] = '\0';
    phase = 0; i=0;
    while(b64_input[i]) {
        c = (int) b64_input[i];
        if(c == '=') {
            decodeChunk(in, output_str);
            break;
        }
        p = strchr(b64_table, c);
        if(p) {
            in[phase] = p - b64_table;
            phase = (phase + 1) % 4;
            if(phase == 0) {
                decodeChunk(in, output_str);
                in[0]=in[1]=in[2]=in[3]=0;
            }
        }
        i++;
    }
}

/**
 * Implementation of decodeChunk
 *
 * @notes:
 */
void decodeChunk(unsigned char in_chunk[], char *output_chunk) {
    unsigned char out[4];
    out[0] = in_chunk[0] << 2 | in_chunk[1] >> 4;
    out[1] = in_chunk[1] << 4 | in_chunk[2] >> 2;
    out[2] = in_chunk[2] << 6 | in_chunk[3] >> 0;
    out[3] = '\0';
    strncat(output_chunk, out, sizeof(out));
}


/**
 * Implementation of encode
 *
 * @notes:
 */
void encode(char *input_str, char *b64_output) {
    unsigned char in[3];
    int i, len = 0;
    int j = 0;

    b64_output[0] = '\0';
    while(input_str[j]) {
        len = 0;
        for(i=0; i<3; i++) {
            in[i] = (unsigned char) input_str[j];
            if(input_str[j]) {
                len++; j++;
            }
            else in[i] = 0;
        }
        if( len ) {
            encodeChunk( in, b64_output, len );
        }
    }
}


/**
 * Implementation of encodeChunk
 *
 * @notes:
 */
void encodeChunk( unsigned char in_chunk[], char b64_out_chunk[], int len ) {
    unsigned char out[5];
    out[0] = b64_table[ in_chunk[0] >> 2 ];
    out[1] = b64_table[ ((in_chunk[0] & 0x03) << 4) | ((in_chunk[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64_table[ ((in_chunk[1] & 0x0f) << 2) |
                                                   ((in_chunk[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64_table[ in_chunk[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64_out_chunk, out, sizeof(out));
}



/**
 * Example usage please keep commented
 * @return
 */

//int main() {
//    char srcstr[] = "hello world";
//    char tobase64[1024] = "";
//    char tostr[1024] = "";
//
//    encode(srcstr, tobase64);
//    printf("encoded:\n[%s]\n", srcstr, tobase64);
//    printf("\n");
//    decode(tobase64, tostr);
//    printf("decoded:\n[%s]\n", tobase64, tostr);
//
//    return 0;
//}