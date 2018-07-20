#include "RM.h"
#include "Menu.h"
#include "Timer.h"
#include "Key.h"

#define MENUID_DEBUG_MODE_ON 	0
#define MENUID_SIEVE_MODE_ON 	1
#define MENUID_SET_SIEVE     	3
#define MENUID_START_PLOT    	4
#define MENUID_RESET_DEVICE  	5
#define MENUID_EXIT          	6
#define MENUID_YES				7
#define MENUID_NO    			8		

#define MENU_OPTION_NUM_MAIN 	6
#define MENU_OPTION_NUM_YN	 	2

typedef struct MenuRecord *ptrToMenuRecord;
typedef ptrToMenuRecord Menu;
typedef struct MenuOp *ptrToMenuOp;

struct MenuOp{
	uchar opID;
	uchar *opString;
	Menu subMenu;
	void (*pOpFunc) (void);  
};

struct MenuRecord{
	uchar opNum;
	Menu superMenu;
	ptrToMenuOp OpArray;
};

static const struct MenuOp ynMenuOpArray[MENU_OPTION_NUM_YN]={
	{MENUID_YES,"Yes",NULL,MenuOpYN},
	{MENUID_NO,"No",NULL,MenuOpYN}
};
static const struct MenuRecord ynMenuRecord={MENU_OPTION_NUM_YN,NULL,ynMenuOpArray};
static const Menu ynMenu=&ynMenuRecord;

static const struct MenuOp MainMenuOpArray[MENU_OPTION_NUM_MAIN]={
	{MENUID_DEBUG_MODE_ON,"Debug Mode On",&ynMenuRecord,NULL},
	{MENUID_SIEVE_MODE_ON,"Sieve Mode On",&ynMenuRecord,NULL},
	{MENUID_SET_SIEVE,"Set Sieve",NULL,MenuOpSetSieve},
	{MENUID_START_PLOT,"Start Plot",NULL,MenuOpStartPlot},
	{MENUID_RESET_DEVICE,"Reset Device",NULL,MenuOpRstDev},
	{MENUID_EXIT,"Exit",NULL,MenuOpExit}
};
static const struct MenuRecord mainMenuRecord={MENU_OPTION_NUM_MAIN,NULL,MainMenuOpArray};
static const Menu mainMenu=&mainMenuRecord;

static Menu curMenu;
static uchar curOp;

static bit isExit;

extern bit isSieveOn;
extern float sieveRVal;
extern float errTolr;

static uchar rNumPart;
static uchar rUnit;
static uchar errTolrE2;

void MenuImpl(){
	
}


