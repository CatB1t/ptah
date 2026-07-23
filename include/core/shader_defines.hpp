#pragma once

// Helper macros
#define PTAH_STRING_IM(x) #x              // Get macro name
#define PTAH_STRING(x) PTAH_STRING_IM(x)  // Convert macro value to string
#define PTAH_EXPAND(x) #x " " PTAH_STRING_IM(x)  // expand to MACRO_NAME VALUE

#define PTAH_N_POINT_LIGHTS 8
