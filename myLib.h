typedef unsigned short u16;
typedef unsigned int u32;

#define NUMOBJS 7

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

extern unsigned short *videoBuffer;


#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,16)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define GREY COLOR(25, 25, 25)
#define ROAD COLOR(40, 40, 40)
#define BLACK 0
#define BROWN COLOR(32, 15, 15)
#define WATER COLOR(0, 16, 31)
// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4 		// source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)


#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

typedef struct {
	int row;
	int col;
	int rd;
	int cd;
	int size;
	unsigned short color;
} MOVOBJ;

typedef struct car Car;
struct car {
	int row;
	int col;
	int height;
	int width;
	unsigned short color;
};

typedef struct log Log;
struct log {
	int row;
	int col;
	int height;
	int width;
	unsigned short color;
};

typedef struct fro Fro;
struct fro {
	int row;
	int col;
	int height;
	int width;
};

enum GBAState {
  BEGAN,
  GAME,
  END
};

// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void waitForVblank();
void drawImage3(int x, int y, int width, int height, const unsigned short *image);
int objectCollision(Fro fro, Car cr, Car crt, Car crte);
int frogLog(Fro *f, struct log *lg, struct log *lge, struct log *lgoe);
void death(Fro *fro, int *lives, int num, int MAPEZ_WIDTH, int MAPEZ_HEIGHT, const unsigned short *mapez, int FROGE_WIDTH, int FROGE_HEIGHT, const unsigned short *froge);
void drawOccupied(int *occupied1, int *occupied2, int *occupied3, int *occupied4, const unsigned short *froge, int *occ1row, int *occ1col, int *occ2row, int *occ2col, int *occ3row, int *occ3col, int *occ4row, int *occ4col);
void drawSubImage(int x, int y, int width, int height, int imgWidth, const unsigned short *image);
void drawFrog(int x, int y, int width, int height, const unsigned short *image);

