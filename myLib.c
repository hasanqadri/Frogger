#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;
//Set a pixel
void setPixel(int row, int col, unsigned short color)
{
  videoBuffer[OFFSET(row,col, 240)] = color;
}
//Draw a rectangle
void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
  for(int r=0; r<height; r++)
  {
    REG_DMA3SAD = (u32)&color;
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
  }
}
//Next frame
void waitForVblank()
{
  while(SCANLINECOUNTER > 160);
  while(SCANLINECOUNTER < 160);
}

//Draws an image
void drawImage3(int x, int y, int width, int height, const unsigned short *image) {
  
  for (int k = 0; k < height; k++) {
    REG_DMA3SAD = (u32)(&image[OFFSET(k, y, width)]);
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(x+k, y, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
  }
}


void drawFrog(int x, int y, int width, int height, const unsigned short *image) {
  
  for (int k = 0; k < height; k++) {
    REG_DMA3SAD = (u32)(image);
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(x+k, y, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
    image += width;
  }
}
//Draw subimage
void drawSubImage(int x, int y, int width, int height, int imgWidth, const unsigned short *image) {
  for (int k = 0; k < height; k++) {
	REG_DMA3SAD = (u32)(&image[OFFSET(x+k, y, imgWidth - 6)]);
	REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(x+k, y, 240)]);
	REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
  }

}

//Detects object collision
int objectCollision(Fro fro, Car cr, Car crt, Car crte) {
  
  if ((fro.row == cr.row) && (fro.col >= cr.col && fro.col + fro.width <= cr.col + cr.width)) { 
    return 1;
  }	
  
  if ((fro.row == crt.row) && (fro.col >= crt.col && fro.col + fro.width <= crt.col + crt.width)) { 
    return 1;
  }
  
  if ((fro.row == crte.row) && (fro.col >= crte.col && fro.col + fro.width <= crte.col + crte.width)) { 
    return 1;
  }
  return 0;
}

//Moves frog along a log
int frogLog(struct fro *fro, struct log *lg, struct log *lge, struct log *lgoe) {
  if (((fro->row == lg->row) || (fro->row == lge->row) || (fro->row == lgoe->row)) && ((fro->col > lg->col && fro->col + fro->width < lg->col + lg->width)
  || (fro->col > lge->col && fro->col + fro->width < lge->col + lge->width)
  || (fro->col > lgoe->col && fro->col + fro->width < lgoe->col + lgoe->width))) { 
    if (fro  -> row == lgoe-> row) {
      fro->col = fro->col + 1;
      return 0;
    } else {
      fro->col = fro->col - 1;
      return 0;
    }
  }
  return 1;
}
//Kills frog
void death(Fro *fro, int *lives, int num, int MAPEZ_WIDTH, int MAPEZ_HEIGHT, const unsigned short *mapez, int FROGE_WIDTH, int FROGE_HEIGHT, const unsigned short *froge) {
  
  if(num == 1) {
    *lives = *lives - 1;
    fro->row = 122;
    fro->col = 118;
    drawImage3(3, 0, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez);
    drawImage3(fro->row, fro->col, FROGE_WIDTH, FROGE_HEIGHT, froge);
    } else {
      fro->row = 122;
      fro->col = 118;
      drawImage3(3, 0, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez);
      drawImage3(fro->row, fro->col, FROGE_WIDTH, FROGE_HEIGHT, froge);
    }
  }
  
//Draws the frog occupying landmass
void drawOccupied(int *occupied1, int *occupied2, int *occupied3, int *occupied4, const unsigned short *froge, int *occ1row, int *occ1col, int *occ2row, int *occ2col, int *occ3row, int *occ3col, int *occ4row, int *occ4col) {
	if (*occupied1 == 1) {
		drawFrog(*occ1row, *occ1col, 8, 8, froge);
	} 
	if (*occupied2 == 1) {
		drawFrog(*occ2row, *occ2col, 8, 8, froge);
	} 	
	if (*occupied3 == 1) {
		drawFrog(*occ3row, *occ3col, 8, 8, froge);
	} 
	if (*occupied4 == 1) {
		drawFrog(*occ4row, *occ4col, 8, 8, froge);
	} 	
}
