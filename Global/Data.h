
#ifndef __DATA_H__
#define __DATA_H__

#define MAP_INSIDE_X   ((INT8U)30)
#define MAP_INSIDE_Y   ((INT8U)16)

#define PAUSE_BMP_LENGTH ((INT8U)60)
#define PAUSE_BMP_HIGHT ((INT8U)32)
#define PAUSE_BMP_X    ((INT8U)35)
#define PAUSE_BMP_Y    ((INT8U)16)

#define LETTER_LENGTH ((INT8U)7)
#define LETTER_HIGHT  ((INT8U)8)

#define MINI_MAP_LENGTH ((INT8U)32)
#define MINI_MAP_HIGHT ((INT8U)16)
#define MINI_MAP_X     ((INT8U)46)
#define MINI_MAP_Y     ((INT8U)34)

#define MINUTE_X ((INT8U)9)
#define CURSOR_MINUTE_X ((INT8U)12)
#define SECOND_X ((INT8U)14)
#define SCORE_X  ((INT8U)6)
#define LEVEL_X	 ((INT8U)14)

#define CHOOSEMAP_X ((INT8U)0)
#define CHOOSEMAP_Y ((INT8U)0)

#define INPUT_NAME_X ((INT8U)55)
#define INPUT_NAME_Y ((INT8U)40)

#define HERO_BOARD_NAME_X  ((INT8U)20)
#define HERO_BOARD_NAME_Y  ((INT8U)16)

#define HERO_BOARD_SCORE_X ((INT8U)80)
#define HERO_BOARD_SCORE_Y ((INT8U)16)

#define SCREEN_LENGTH ((INT8U)128)
#define SCREEN_HIGHT  ((INT8U)64)

#define NODE_LENGTH   ((INT8U)4)
#define NODE_HIGHT    ((INT8U)4)

#define APPEND 	     ((INT8U)1)
#define REPLACE      ((INT8U)0)

#define DRAWNODE     ((INT8U)1)
#define CLEARNODE    ((INT8U)0)			 

extern INT8U ucGameState;

extern const Size_t ucGameOverBmpSize;
extern const Size_t ucYouWinBmpSize;
extern const Size_t ucWelcomeBmpSize;
extern const Size_t ucPauseBmpSize;
extern const Size_t ucHeroBoardSize;
extern const Size_t ucCongratulationsBmpSize;
extern const Size_t MiniMapSize;
extern const Size_t MapSize;
extern const Size_t LetterSize;

extern const Size_t ucCursorBmpSize;
extern const Size_t ucContinueBmpSize;
extern const Size_t ucTopThreeBmpSize;
extern const Size_t ucChooseMapBmpSize;
extern const Size_t ucTwinkleBlankBmpSize;

#endif

