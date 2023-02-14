#if defined _tsubaki_menu
	#endinput
#endif
#define _tsubaki_menu


stock const MENU_KEYS_ALL = (MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_6|MENU_KEY_7|MENU_KEY_8|MENU_KEY_9|MENU_KEY_0);
	#define MENU_KEYS_BLOCK (1<<12)
	#define MENU_KEYSTROKE_0	9
	#define MENU_KEYSTROKE_1	0
	#define MENU_KEYSTROKE_2	1
	#define MENU_KEYSTROKE_3	2
	#define MENU_KEYSTROKE_4	3
	#define MENU_KEYSTROKE_5	4
	#define MENU_KEYSTROKE_6	5
	#define MENU_KEYSTROKE_7	6
	#define MENU_KEYSTROKE_8	7
	#define MENU_KEYSTROKE_9	8
	
stock const MENU_TITLE_CONTENT_SPLIT[] = "^n-------------------------^n";
stock const MENU_PREVIOUS_PAGE[] = "上一頁";
stock const MENU_NEXT_PAGE[] = "下一頁";

stock giPlyCurMenuId[MAX_PLAYERS];
stock giPlyCurMenuPage[MAX_PLAYERS];
stock giPlyCurMenuTarget[MAX_PLAYERS];

#define MAX_MENU_ITEM		7
#define PLY_MENU_ID(%0)		giPlyCurMenuId[%0]
#define PLY_MENU_PAGE(%0)	giPlyCurMenuPage[%0]
#define PLY_MENU_TARGET(%0) giPlyCurMenuTarget[%0]

    #define GetPlayerMenuOptionId(%0,%1)    (giPlyCurMenuPage[%0]*MAX_MENU_ITEM+%1)
    #define MenuHasPreviousPage(%0)         (giPlyCurMenuPage[%0]>0)
    #define MenuHasNextPage(%0,%1)          ((giPlyCurMenuPage[%0]+1)*MAX_MENU_ITEM<%1)
	
	#define BuildMenuPreviousPage(%0,%1) if(MenuHasPreviousPage(%0)) format(%1, charsmax(%1), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 8, MENU_PREVIOUS_PAGE)
	#define BuildMenuNextPage(%0,%1,%2) if(MenuHasNextPage(%0,%2)) format(%1, charsmax(%1), DOUBLE_STRING_WITH_NUMBER_FMT, menu_msg, 9, MENU_NEXT_PAGE)

ResetPlayerMenu(client)
{
    PLY_MENU_ID(client) = PLY_MENU_PAGE(client) = PLY_MENU_TARGET(client) = 0;
    menu_cancel(client);
}


stock SINGLE_STRING_NEWLINE_FMT[] = "%s^n";
stock DOUBLE_STRING_FMT[] = "%s%s";
stock DOUBLE_STRING_NEWLINE_FMT[] = "%s%s^n";
stock DOUBLE_STRING_WITH_NUMBER_FMT[] = "%s%d.%s^n";