#define PACKET_HEADER_SIZE 8
#define START_BYTE 0xFF
#define RESERVED_1 0x00
#define RESERVED_2 0x00
/*
 * internal commands
 * 
 */
#define NO_INTERNAL_COMM 0x00
#define TCP 0x01
#define ADHOC 0x00
#define ACK 0x01
#define FW 0x00
#define ic_1 0b001
#define ic_2 0b010
#define ic_3 0b011
#define ic_4 0b100
#define ic_5 0b101
#define ic_6 0b110
#define ic_7 0b111
#define tcp 0b0
//#define adhoc 0b1
#define res1 0b000
#define fwd 0b0
#define res2 0b000

/*
 * 
 * external commands
 */

#define MOVE_FWD 0x01
#define MOVE_FWD_TIME 0x02
#define MOVE_BCK 0x03
#define MOVE_BCK_TIME 0x04
#define ROTATE_LEFT 0x05
#define ROTATE_RIGHT 0x06
#define STOPLOC 0x07
#define ULTRASONIC_DST_1 0x08
#define ULTRASONIC_DST_2 0x09
#define ULTRASONIC_DST_3 0x0A
#define MOVE_FWD_DST 0x0B
#define MOVE_BCK_DST 0x0C
#define GET_MAG_DATA 0x0D
#define GET_RSSI 0x0E
#define GET_ID 0x0F
#define SET_ID 0x10

/*setting packet location
 * 
 * 
 */
#define START_BYTE_LOCATION 0
#define SRC_DST_LOCATION 1
#define ISC_LOCATION 2
#define IC_LOCATION 3
#define CH_LOCATION 4
#define CL_LOCATION 5
#define LENGTH_LOCATION 6


/*
 * matrix/
 * 
 * 
 */

#define MATRIX_DIM 3
char matrix[3][3]={{0,1,0},{1,0,1},{0,1,0}};



//Packet functions
 
