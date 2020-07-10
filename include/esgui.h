#ifndef _es_gui_h_
#define _es_gui_h_

#include <stddef.h>
#include <stdbool.h>

/// Common esgui header file. it's sufficient to include this single file in source;
/// All neccessary dependent headers will be included automatically
///
#include <esfwxe/target.h>

#include <esgui/include/esgui_defs.h>

/// GUI type defs
#include <esgui/include/esgui_types.h>

#if defined(ESGUI_USE_BASE_COMPONENTS) && 1 == ESGUI_USE_BASE_COMPONENTS
# include <esgui/include/esgui_color.h>
#endif

/// GUI utilities
#include <esgui/include/esgui_utils.h>

#if defined(ESGUI_USE_BASE_COMPONENTS) && 1 == ESGUI_USE_BASE_COMPONENTS

/// GUI Common components
# include <esgui/include/esgui_point.h>
# include <esgui/include/esgui_rect.h>
# include <esgui/include/esgui_math.h>

# if defined(ESGUI_USE_STD_COLORS) && 0 != ESGUI_USE_STD_COLORS
/// Standard color definitions
#   include <esgui/include/esgui_stdcolors.h>
# endif

/// GUI ll components
///
# include <esgui/include/esgui_bit_macros.h>
# include <esgui/include/esgui_bitmap.h>
# include <esgui/include/esgui_font.h>

#endif // ESGUI_USE_BASE_COMPONENTS

/// GUI driver services
#include <esgui/include/esgui_screen_driver.h>

#if defined(ESGUI_USE_BASE_COMPONENTS) && 1 == ESGUI_USE_BASE_COMPONENTS

/// GUI low-level drawing, built on top of screen context & driver services
# include <esgui/include/esgui_canvas.h>

# if defined(ESGUI_USE_WIDGETS) && 1 == ESGUI_USE_WIDGETS

/// EsGUI widgets
#   include <esgui/include/esgui_widgets.h>
#   include <esgui/include/esgui_widget_std.h>
#   include <esgui/include/esgui_widget_checkbox.h>
#   include <esgui/include/esgui_widget_spinctl.h>
#   include <esgui/include/esgui_widget_progress.h>
#   include <esgui/include/esgui_widget_listview.h>
#   include <esgui/include/esgui_widget_dtedit.h>
#   include <esgui/include/esgui_widget_msgbox.h>

#   if defined(ESGUI_USE_CHART) && 1 == ESGUI_USE_CHART

/// EsGUI chart
#     include <esgui/chart/esgui_chart.h>
#     if (ESGUI_USE_CHART_CARTESIAN2D == (ESGUI_USE_CHART & ESGUI_USE_CHART_CARTESIAN2D))
#       include <esgui/chart/esgui_chart_cartesian2d.h>
#     endif
#   endif

# endif // ESGUI_USE_WIDGETS

#endif // ESGUI_USE_BASE_COMPONENTS

#endif // _es_gui_h_

