#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myLib.h"
#include "text.h"
#include "mapez.h"
#include "start.h"
#include "end.h"
#include "froge.h"

// State enum definition


int main()
{
enum GBAState {
  BEGAN,
  GAME,
  END
};
  REG_DISPCTL = MODE3 | BG2_ENABLE;
  enum GBAState state = BEGAN;

  int gameStart = 1;
  int lives = 3;
  int score = 0;
  int counter = 3600;
  int timer = counter / 60;
  //Checks if the frog occupies a land block
  int occupied1 = 0;
  int occ1row = 0;
  int occ1col = 0;
  int occupied2 = 0;
  int occ2row = 0;
  int occ2col = 0;
  int occupied3 = 0;
  int occ3row = 0;
  int occ3col = 0;
  int occupied4 = 0;
  int occ4row = 0;
  int occ4col = 0;
  //Create frog, car, and log objects
  Fro f = {122, 118, FROGE_WIDTH, FROGE_HEIGHT};
  Car cr = {77, 5, 8, 18, RED};
  Car crte = {92, 234, 8, 18, YELLOW};
  Car crt = {107, 5, 8, 18, BLUE};
  Log lg = {47, 234, 8, 30, BROWN};
  Log lgo = {32, 5, 8, 30, BROWN};
  Log lgoe = {17, 5, 8, 30, BROWN};
  int pressed = 0;
  int prevUp = 0;
  int prevDown = 0;
  int prevLeft = 0;
  int prevRight = 0;
  while(1)
  {
    //check if any of the buttons were previously pressed
    pressed = (KEY_DOWN_NOW(BUTTON_SELECT)) ? 1 : 0;
    prevUp = (KEY_DOWN_NOW(BUTTON_UP)) ? 1 : 0;
    prevDown = (KEY_DOWN_NOW(BUTTON_DOWN)) ? 1 : 0;
    prevLeft = (KEY_DOWN_NOW(BUTTON_LEFT)) ? 1 : 0;
    prevRight = (KEY_DOWN_NOW(BUTTON_RIGHT)) ? 1 : 0;

    waitForVblank();

    //timer
    counter = counter - 1;
    timer = counter / 60;

    //states
    switch(state) {

      //start screen
      case BEGAN:
      drawImage3(0, 0, START_WIDTH, START_HEIGHT, start);
      while(!KEY_DOWN_NOW(BUTTON_SELECT)){}
      while(KEY_DOWN_NOW(BUTTON_SELECT)) {
        state = GAME;
      }
      break;

      //start game
      case GAME: ;
      //first time game starts
      if (gameStart) {
        drawImage3(3, 0, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez);
        drawFrog(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, froge);
        drawRect(lg.row, lg.col, lg.height, lg.width, lg.color);
        drawRect(lgo.row, lgo.col, lgo.height, lgo.width, lgo.color);
        drawRect(lgoe.row, lgoe.col, lgoe.height, lgoe.width, lgoe.color);
        drawRect(cr.row, cr.col, cr.height, cr.width, cr.color);
        drawRect(crte.row, crte.col, crte.height, crte.width, crte.color);
        drawRect(crt.row, crt.col, crt.height, crt.width, crt.color);
        gameStart = 0;
      }
      //if collision with cars or froge goes out of bounds
      if (objectCollision(f, cr, crt, crte) || f.col < 0 || f.col > 220) {
        death(&f, &lives, 1, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez, FROGE_WIDTH, FROGE_HEIGHT, froge);
      }
      //moves froge with a log
      int notLogging = frogLog(&f, &lg, &lgo, &lgoe);

      //checks if froge is in a win position at the top of the screen, otherwise froge loses a life
      if (f.row == 3) {
        if (f.col >=21 && f.col <= 45) {
          if (occupied1) {
            death(&f, &lives, 1, 234, 140, mapez, 8, 8, froge);
          } else {
            occ1row = f.row;
            occ1col = f.col;
            death(&f, &lives, 0, 234, 140, mapez, 8, 8, froge);
            occupied1 = 1;

          }
        } else if (f.col >= 70 && f.col <= 104) {
          if (occupied2) {
            death(&f, &lives, 1, 234, 140, mapez, 8, 8, froge);
          } else {
            occ2row = f.row;
            occ2col = f.col;
            death(&f, &lives, 0, 234, 140, mapez, 8, 8, froge);
            occupied2 = 1;

          }
        } else if (f.col >=130 && f.col <= 156) {
          if (occupied3) {
            death(&f, &lives, 1, 234, 140, mapez, 8, 8, froge);
          } else {
            occ3row = f.row;
            occ3col = f.col;
            death(&f, &lives, 0, 234, 140, mapez, 8, 8, froge);
            occupied3 = 1;

          }
        } else if (f.col >= 182 && f.col <= 208) {
          if (occupied4) {
            death(&f, &lives, 1, 234, 140, mapez, 8, 8, froge);
          } else {
            occ4row = f.row;
            occ4col = f.col;
            death(&f, &lives, 0, 234, 140, mapez, 8, 8, froge);
            occupied4 = 1;

          }

        }else {
          death(&f, &lives, 1, 234, 140, mapez, 8, 8, froge);
        }
      }
      //if froge is on the water, then it dies
      if (notLogging && (f.row <= 50  && f.row >= 10)){
        death(&f, &lives, 1, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez, FROGE_WIDTH, FROGE_HEIGHT, froge);
      }

      //if timer = 0, froge dies
      if (timer == 0){
        death(&f, &lives, 1, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez, FROGE_WIDTH, FROGE_HEIGHT, froge);
        state = END;
      }

      //print lives and score on screen
      char str[] = "Lives: ";
      char cstr[20];
      sprintf(cstr,"%s%d", str, lives);
      drawRect(150, 5, cr.height + 10, cr.width +40, BLACK);
      drawString(150, 5, cstr, RED);
      char bf[] = "Score: ";
      char cbf[20];
      sprintf(cbf,"%s%d", bf, score);
      drawRect(150, 100, cr.height + 10, cr.width +40, BLACK);
      drawString(150, 100, cbf, YELLOW);
      char cter[] = "Timer: ";
      char cster[20];
      sprintf(cster, "%s%d", cter, timer);
      drawRect(150, 165, cr.height + 10, cr.width +60, BLACK);
      drawString(150, 165, cster, WHITE);


      //draw old pos of objects
      drawRect(cr.row, cr.col, cr.height, cr.width, BLACK);
      drawRect(crte.row, crte.col, crte.height, crte.width, BLACK);
      drawRect(crt.row, crt.col, crt.height, crt.width, BLACK);


      drawRect(lg.row, lg.col, lg.height, lg.width, BLACK);
      drawRect(lgo.row, lgo.col, lgo.height, lgo.width, BLACK);
      drawRect(lgoe.row, lgoe.col, lgoe.height, lgoe.width, BLACK);



      //car update
      cr.col = cr.col + 1;
      crt.col = crt.col + 1;
      crte.col = crte.col - 1;
      if (cr.col == 230) {  //red
        drawRect(cr.row, cr.col-1, cr.height, cr.width, BLACK);
        cr.col = 0;
      }
      if (crt.col == 234) {  //blue
        drawRect(crt.row, crt.col-1, crt.height, crt.width, BLACK);
        crt.col = 0;
      }
      if (crte.col == 0) {  //yellow
        drawRect(crte.row, crte.col+1, crt.height, crt.width, BLACK);
        crte.col = 234;
      }

      //log update
      lg.col = lg.col - 1;
      lgo.col = lgo.col - 1;
      lgoe.col = lgoe.col + 1;
      if (lg.col == 0) {
        drawRect(lg.row, lg.col, lg.height, lg.width, BLACK);
        lg.col = 234;
      }
      if (lgo.col == 0) {
        drawRect(lgo.row, lgo.col, lgo.height, lgo.width, BLACK);
        lgo.col = 234;
      }
      if (lgoe.col == 234) {
        drawRect(lgoe.row, lgoe.col, lgoe.height, lgoe.width, BLACK);
        lgoe.col = 0;
      }

   
      //draw new pos of objects

      drawRect(cr.row, cr.col, cr.height, cr.width, cr.color);
      drawRect(crte.row, crte.col, crte.height, crte.width, crte.color);
      drawRect(crt.row, crt.col, crt.height, crt.width, crt.color);

      drawRect(lg.row, lg.col, lg.height, lg.width, lg.color);
      drawRect(lgo.row, lgo.col, lgo.height, lgo.width, lgo.color);
      drawRect(lgoe.row, lgoe.col, lgoe.height, lgoe.width, lgoe.color);
      drawOccupied(&occupied1, &occupied2, &occupied3, &occupied4, froge, &occ1row, &occ1col, &occ2row, 		&occ2col, &occ3row, &occ3col, &occ4row, &occ4col);

      drawFrog(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, froge);
   

	//move up
      if (KEY_DOWN_NOW(BUTTON_UP)) {
        if (!prevUp) {
          drawSubImage(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, MAPEZ_WIDTH, mapez);
          drawOccupied(&occupied1, &occupied2, &occupied3, &occupied4, froge, &occ1row, &occ1col, 			&occ2row, &occ2col, &occ3row, &occ3col, &occ4row, &occ4col);
          f.row = (f.row == 17) ? f.row - 14 : f.row - 15;
        }
      }

      //move down
      if (KEY_DOWN_NOW(BUTTON_DOWN)) {
        if(!prevDown) {
          if (f.row != 122) {
            drawSubImage(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, MAPEZ_WIDTH, mapez);
            drawOccupied(&occupied1, &occupied2, &occupied3, &occupied4, froge, &occ1row, &occ1col, 			&occ2row, &occ2col, &occ3row, &occ3col, &occ4row, &occ4col);
            f.row = f.row + 15;
          }
        }
      }

      //move left
      if (KEY_DOWN_NOW(BUTTON_LEFT)) {
        if (!prevLeft) {
          drawSubImage(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, MAPEZ_WIDTH, mapez);
          drawOccupied(&occupied1, &occupied2, &occupied3, &occupied4, froge, &occ1row, &occ1col, 			&occ2row, &occ2col, &occ3row, &occ3col, &occ4row, &occ4col);
          f.col = f.col - 10;
        }
      }

      //move right
      if ( KEY_DOWN_NOW(BUTTON_RIGHT)) {
        if (!prevRight) {
          drawSubImage(f.row, f.col, FROGE_WIDTH, FROGE_HEIGHT, MAPEZ_WIDTH, mapez);
          drawOccupied(&occupied1, &occupied2, &occupied3, &occupied4, froge, &occ1row, &occ1col, 			&occ2row, &occ2col, &occ3row, &occ3col, &occ4row, &occ4col);
          f.col = f.col + 10;
        }
      }

      //If lives are 0, end game
      if (lives == 0) {
        state = END;
      }

      //If all spots are taken, increase score and reset game
      if ((occupied1 == 1) && (occupied2 == 1) && (occupied3 == 1) && (occupied4 == 1)) {
        occupied1 = 0;
        occupied2 = 0;
        occupied3 = 0;
        occupied4 = 0;
        score = score + 1;
        counter = 3600;
        drawImage3(3, 0, MAPEZ_WIDTH, MAPEZ_HEIGHT, mapez);

     }

      //change state
      if(KEY_DOWN_NOW(BUTTON_SELECT)) {
        if (pressed) {
          state = END;
        }
      }
      break;

      //retry game
      case END:
      drawImage3(0, 0, END_WIDTH, END_HEIGHT, end);
      drawString(130, 100, cbf, YELLOW);
      while(!KEY_DOWN_NOW(BUTTON_SELECT)){}
      while(KEY_DOWN_NOW(BUTTON_SELECT)) {
        if (!pressed) {
          state = BEGAN;
          gameStart = 1;
          lives = 3;
        }
      }
      occupied1 = occupied2 = occupied3 = occupied4 = 0;
      occ1row = occ1col = occ2row = occ2col = 0;
      occ3row = occ3col = occ4row = occ4col = 0;
      score = 0;
      f.row = 122;
      f.col = 118;
      cr.row = 77;
      cr.col = 5;
      crte.row = 92;
      crte.col = 230;
      crt.row = 107;
      crt.col = 5;
      counter = 3600;
      break;

    }
  }
  return 0;
}

