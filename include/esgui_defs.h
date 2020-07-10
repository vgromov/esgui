#ifndef _esgui_defs_h_
#define _esgui_defs_h_

/// Chart components defined, for USE mask in esguiConfig
#define ESGUI_USE_CHART_CARTESIAN2D 0x0001
#define ESGUI_USE_CHART_POLAR2D     0x0002

// Standard colors subsets
#define ESGUI_USE_STD_COLORS_MONO   0x0001
#define ESGUI_USE_STD_COLORS_BASIC_MIN 0x0002

/// Esgui configuration file, all config-dependent files should follow
#include <esgui_config.h>

#ifndef ESGUI_MODALS_MAX_CNT
# define ESGUI_MODALS_MAX_CNT       4
#endif

// Standartize configuration definitions
//
#if defined(ESGUI_USE_FONT)
# if 0 != ESGUI_USE_FONT
#   undef ESGUI_USE_FONT
#   define ESGUI_USE_FONT           1
#   if !defined(ESGUI_USE_BASE_COMPONENTS) || 0 == ESGUI_USE_BASE_COMPONENTS
#     undef ESGUI_USE_BASE_COMPONENTS
#     define ESGUI_USE_BASE_COMPONENTS 1
#     undef ESGUI_USE_COLOR
#     define ESGUI_USE_COLOR        1
#   endif
# endif
#else
# define ESGUI_USE_FONT             0
#endif

#if defined(ESGUI_USE_TEXT_DRAWING)
# if 0 != ESGUI_USE_TEXT_DRAWING
#   undef ESGUI_USE_TEXT_DRAWING
#   define ESGUI_USE_TEXT_DRAWING   1
#   if !defined(ESGUI_USE_BASE_COMPONENTS) || 0 == ESGUI_USE_BASE_COMPONENTS
#     undef ESGUI_USE_BASE_COMPONENTS
#     define ESGUI_USE_BASE_COMPONENTS 1
#     undef ESGUI_USE_COLOR
#     define ESGUI_USE_COLOR        1
#   endif
# endif
#else
# define ESGUI_USE_TEXT_DRAWING     0
#endif

#if 1 == ESGUI_USE_TEXT_DRAWING
# undef ESGUI_USE_FONT
# define ESGUI_USE_FONT             1
# undef ESGUI_USE_COLOR
# define ESGUI_USE_COLOR            1
#endif

#if defined(ESGUI_USE_BITMAP)
# if 0 != ESGUI_USE_BITMAP
#   undef ESGUI_USE_BITMAP
#   define ESGUI_USE_BITMAP         1
#   if !defined(ESGUI_USE_BASE_COMPONENTS) || 0 == ESGUI_USE_BASE_COMPONENTS
#     undef ESGUI_USE_BASE_COMPONENTS
#     define ESGUI_USE_BASE_COMPONENTS 1
#     undef ESGUI_USE_COLOR
#     define ESGUI_USE_COLOR        1
#   endif
# endif
#else
# define ESGUI_USE_BITMAP           0
#endif

#if defined(ESGUI_USE_BASE_COMPONENTS) && 1 == ESGUI_USE_BASE_COMPONENTS
# ifndef ESGUI_USE_COLOR
#   define ESGUI_USE_COLOR          1
# endif
#endif

#if !defined(ESGUI_USE_STD_COLORS)
# define ESGUI_USE_STD_COLORS       0
#endif

#if defined(ESGUI_BUFF_LEN) && ESGUI_BUFF_LEN > 0

#ifdef __cplusplus
  extern "C" {
#endif 

// Access globally defined ESGUI string buffer used internally by ESGUI for
// string formatting. May be used by third-party ESGUI usage code for
// string formatting as well
//
char* esguiStrBuffGet(void);

#ifdef __cplusplus
  }
#endif 

#endif

/// Standard widgets dependencies
///
#if !defined(ESGUI_WIDGET_USE_CHECKBOX)
# define ESGUI_WIDGET_USE_CHECKBOX  0
#endif

#if !defined(ESGUI_WIDGET_USE_SPINCTL)
# define ESGUI_WIDGET_USE_SPINCTL   0
#endif

#if !defined(ESGUI_WIDGET_USE_PROGRESS)
# define ESGUI_WIDGET_USE_PROGRESS  0
#endif

#if !defined(ESGUI_WIDGET_USE_LISTVIEW)
# define ESGUI_WIDGET_USE_LISTVIEW  0
#endif

#if !defined(ESGUI_WIDGET_USE_DTEDIT)
# define ESGUI_WIDGET_USE_DTEDIT    0
#endif

#if !defined(ESGUI_WIDGET_USE_MSGBOX)
# define ESGUI_WIDGET_USE_MSGBOX    0
#endif

#if 1 == ESGUI_WIDGET_USE_CHECKBOX  || \
    1 == ESGUI_WIDGET_USE_SPINCTL   || \
    1 == ESGUI_WIDGET_USE_PROGRESS  || \
    1 == ESGUI_WIDGET_USE_LISTVIEW  || \
    1 == ESGUI_WIDGET_USE_DTEDIT    || \
    1 == ESGUI_WIDGET_USE_MSGBOX

# if !defined(ESGUI_USE_WIDGETS) || 0 == ESGUI_USE_WIDGETS
#   undef ESGUI_USE_WIDGETS
#   define ESGUI_USE_WIDGETS        1
# endif

# if !defined(ESGUI_WIDGET_USE_STD) || 0 == ESGUI_WIDGET_USE_STD
#   undef ESGUI_WIDGET_USE_STD
#   define ESGUI_WIDGET_USE_STD      1
# endif

#endif

#endif //< _esgui_defs_h_
