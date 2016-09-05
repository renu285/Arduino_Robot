/*
 * internal commands
 * 
 */
#define ic_1 0b001
#define ic_2 0b010
#define ic_3 0b011
#define ic_4 0b100
#define ic_5 0b101
#define ic_6 0b110
#define ic_7 0b111
#define tcp 0b0
#define adhoc 0b1
#define res1 0b000
#define fwd 0b0
#define ack 0b1
#define res2 0b000

/*
 * 
 * external commands
 */

#define movefwd 0x01
#define mvfwdtime 0x02
#define movebck 0x03
#define movebcktime 0x04
#define rotatelft 0x05
#define rotatergt 0x06
#define stoploc 0x07
#define returnu1 0x08
#define returnu2 0x09
#define returnu3 0x0A
#define movefwdist 0x0B
#define movebkdist 0x0C
#define GET_MAG_DATA 0x0D
#define GET_RSSI 0x0E
#define GET_ID 0x0F
#define SET_ID 0x10

/*setting packet location
 * 
 * 
 */
#define start_byte_location 0
#define srcdst_location 1
#define isc_location 2
#define ic_location 3
#define ch_location 4
#define cl_location 5
#define length_location 6


/*
 * matrix/
 * 
 * 
 */

#define MATRIX_DIM 3
char matrix[3][3]={{0,1,0},{1,0,1},{0,1,0}};

 
