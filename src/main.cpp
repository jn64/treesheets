
#include "stdafx.h"

#ifndef __WXMSW__
#define SIMPLERENDER
#endif

//#define SIMPLERENDER // for testing

static const int g_grid_margin = 1;
static const int g_cell_margin = 2;
static const int g_margin_extra = 2;  // TODO, could make this configurable: 0/2/4/6
static const int g_line_width = 1;
static const int g_selmargin = 2;
static const int g_scrollratecursor = 240;  // FIXME: must be configurable
static const int g_scrollratewheel = 2;     // relative to 1 step on a fixed wheel usually being 120
static const int g_max_launches = 20;
static const int g_deftextsize_default = 12;
static const int g_mintextsize_delta = 8;
static const int g_maxtextsize_delta = 32;

static int g_deftextsize = g_deftextsize_default;
static int g_mintextsize() { return g_deftextsize - g_mintextsize_delta; }
static int g_maxtextsize() { return g_deftextsize + g_maxtextsize_delta; }

static const std::array<uint, 42> celltextcolors = {
    0xFFFFFF,  // CUSTOM COLOR!
    0xFFFFFF, 0x000000, 0x1B1B1B, 0x2E2E2E, 0x454545, 0x5C5C5C, 0x757575, 0x919191, 0xADADAD,
    0xC9C9C9, 0xE6E6E6, 0x030A8B, 0x0722E5, 0xAEB8FD, 0x122378, 0x0049CF, 0x78BCFF, 0x0A415C,
    0x386F93, 0x00CEFA, 0x0B4A2F, 0x007D6A, 0x0AD3C5, 0x214C15, 0x178800, 0x7BE170, 0x5C4F0E,
    0x808400, 0xF2DA52, 0x80441A, 0xD67600, 0xFFD3A1, 0xBF2F22, 0xFF6648, 0xFFC8B8, 0x9D2C3D,
    0xE95F74, 0xFFBFD5, 0x6C1E71, 0xD032BE, 0xFFB2F4,
};
enum { CUSTOMCOLORIDX = 0 };

enum { TS_VERSION = 23, TS_TEXT = 0, TS_GRID, TS_BOTH, TS_NEITHER };

static const uint TS_SELECTION_MASK = 0x80;

enum {
    A_NEW = 500,
    A_OPEN,
    A_CLOSE,
    A_SAVE,
    A_SAVEAS,
    A_CUT,
    A_COPY,
    A_PASTE,
    A_NEWGRID,
    A_UNDO,
    A_ABOUT,
    A_RUN,
    A_CLRVIEW,
    A_MARKDATA,
    A_MARKVIEWH,
    A_MARKVIEWV,
    A_MARKCODE,
    A_IMAGE,
    A_EXPIMAGE,
    A_EXPXML,
    A_EXPHTMLT,
    A_EXPHTMLO,
    A_EXPHTMLB,
    A_EXPTEXT,
    A_ZOOMIN,
    A_ZOOMOUT,
    A_TRANSPOSE,
    A_DELETE,
    A_BACKSPACE,
    A_DELETE_WORD,
    A_BACKSPACE_WORD,
    A_LEFT,
    A_RIGHT,
    A_UP,
    A_DOWN,
    A_MLEFT,
    A_MRIGHT,
    A_MUP,
    A_MDOWN,
    A_SLEFT,
    A_SRIGHT,
    A_SUP,
    A_SDOWN,
    A_ALEFT,
    A_ARIGHT,
    A_AUP,
    A_ADOWN,
    A_SCLEFT,
    A_SCRIGHT,
    A_SCUP,
    A_SCDOWN,
    A_DEFFONT,
    A_IMPXML,
    A_IMPXMLA,
    A_IMPTXTI,
    A_IMPTXTC,
    A_IMPTXTS,
    A_IMPTXTT,
    A_HELP,
    A_MARKVARD,
    A_MARKVARU,
    A_SHOWSBAR,
    A_SHOWTBAR,
    A_LEFTTABS,
    A_TRADSCROLL,
    A_HOME,
    A_END,
    A_CHOME,
    A_CEND,
    A_PRINT,
    A_PREVIEW,
    A_PAGESETUP,
    A_PRINTSCALE,
    A_EXIT,
    A_NEXT,
    A_PREV,
    A_BOLD,
    A_ITALIC,
    A_TT,
    A_UNDERL,
    A_SEARCH,
    A_CASESENSITIVESEARCH,
    A_CLEARSEARCH,
    A_CLEARREPLACE,
    A_REPLACE,
    A_REPLACEONCE,
    A_REPLACEONCEJ,
    A_REPLACEALL,
    A_REPLACEF,
    A_SELALL,
    A_CANCELEDIT,
    A_BROWSE,
    A_ENTERCELL,
    A_ENTERCELL_JUMPTOEND,
    A_PROGRESSCELL, // see https://github.com/aardappel/treesheets/issues/139#issuecomment-544167524
    A_CELLCOLOR,
    A_TEXTCOLOR,
    A_BORDCOLOR,
    A_INCSIZE,
    A_DECSIZE,
    A_INCWIDTH,
    A_DECWIDTH,
    A_ENTERGRID,
    A_LINK,
    A_LINKREV,
    A_LINKIMG,
    A_LINKIMGREV,
    A_SEARCHNEXT,
    A_CUSTCOL,
    A_COLCELL,
    A_SORT,
    A_SEARCHF,
    A_MAKEBAKS,
    A_TOTRAY,
    A_AUTOSAVE,
    A_FULLSCREEN,
    A_ZEN_MODE,
    A_SCALED,
    A_SCOLS,
    A_SROWS,
    A_SHOME,
    A_SEND,
    A_BORD0,
    A_BORD1,
    A_BORD2,
    A_BORD3,
    A_BORD4,
    A_BORD5,
    A_HSWAP,
    A_TEXTGRID,
    A_TAGADD,
    A_TAGREMOVE,
    A_WRAP,
    A_HIFY,
    A_FLATTEN,
    A_BROWSEF,
    A_ROUND0,
    A_ROUND1,
    A_ROUND2,
    A_ROUND3,
    A_ROUND4,
    A_ROUND5,
    A_ROUND6,
    A_FILTER5,
    A_FILTER10,
    A_FILTER20,
    A_FILTER50,
    A_FILTERM,
    A_FILTERL,
    A_FILTERS,
    A_FILTEROFF,
    A_FILTERBYCELLBG,
    A_FILTERMATCHNEXT,
    A_FILTERRANGE,
    A_FILTERDIALOG,
    A_FASTRENDER,
    A_EXPCSV,
    A_PASTESTYLE,
    A_PREVFILE,
    A_NEXTFILE,
    A_IMAGER,
    A_INCWIDTHNH,
    A_DECWIDTHNH,
    A_ZOOMSCR,
    A_ICONSET,
    A_V_GS,
    A_V_BS,
    A_V_LS,
    A_H_GS,
    A_H_BS,
    A_H_LS,
    A_GS,
    A_BS,
    A_LS,
    A_RESETSIZE,
    A_RESETWIDTH,
    A_RESETSTYLE,
    A_RESETCOLOR,
    A_LASTCELLCOLOR,
    A_LASTTEXTCOLOR,
    A_LASTBORDCOLOR,
    A_OPENCELLCOLOR,
    A_OPENTEXTCOLOR,
    A_OPENBORDCOLOR,
    A_DDIMAGE,
    A_MINCLOSE,
    A_SINGLETRAY,
    A_CENTERED,
    A_SORTD,
    A_STRIKET,
    A_FOLD,
    A_FOLDALL,
    A_UNFOLDALL,
    A_IMAGESCP,
    A_IMAGESCW,
    A_IMAGESCF,
    A_IMAGESCN,
    A_IMAGESVA,
    A_SAVE_AS_JPEG,
    A_SAVE_AS_PNG,
    A_HELPI,
    A_HELP_OP_REF,
    A_REDO,
    A_FSWATCH,
    A_DEFBGCOL,
    A_THINSELC,
    A_COPYCT,
    A_COPYBM,
    A_MINISIZE,
    A_CUSTKEY,
    A_AUTOEXPORT,
    A_NOP,
    A_TAGSET = 1000,  // and all values from here on
    A_SCRIPT = 2000,  // and all values from here on
    A_MAXACTION = 3000
};

enum {
    STYLE_BOLD = 1,
    STYLE_ITALIC = 2,
    STYLE_FIXED = 4,
    STYLE_UNDERLINE = 8,
    STYLE_STRIKETHRU = 16
};

enum { DRAGANDDROP, CLIPBOARD, CLIPBOARD_CONTINUOUS_TEXT };

static const std::map<char, pair<wxBitmapType, wxString>> imagetypes = {
    { 'I', {wxBITMAP_TYPE_PNG, "image/png"} },
    { 'J', {wxBITMAP_TYPE_JPEG, "image/jpeg"} }
};

#include "script_interface.h"

using namespace script;

struct treesheets {
    struct TreeSheetsScriptImpl;

    struct Cell;
    struct Grid;
    struct Text;
    struct Evaluator;
    struct Image;
    struct Document;
    class Selection;

    struct System;

    struct MyApp;
    struct MyFrame;
    struct TSCanvas;

    static System *sys;

    #include "treesheets_impl.h"

    #include "selection.h"
    #include "text.h"
    #include "cell.h"
    #include "grid.h"
    #include "evaluator.h"

    #include "document.h"
    #include "system.h"

    #include "mywxtools.h"
    #include "mycanvas.h"
    #include "myframe.h"
    #include "myapp.h"
};

treesheets::System *treesheets::sys = nullptr;
treesheets::TreeSheetsScriptImpl treesheets::tssi;

IMPLEMENT_APP(treesheets::MyApp)

#include "myevents.h"