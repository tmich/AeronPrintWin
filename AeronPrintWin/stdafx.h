#pragma once
//#define WIN32_LEAN_AND_MEAN

// Predefinitions for windows.h go here
#include "targetver.h"			// Set the supported window features

#include <cpprest/http_client.h>	// HTTP Client
#include <cpprest/json.h>			// JSON library
#include <cpprest/uri.h>            // URI library
#include <string>
#include <vector>
#include <exception>
#include <filesystem>
#include <algorithm>
#include <locale>

#include "resource.h"
#include "sqlite_db.h"

#include <wxx_appcore.h>
#include <wxx_commondlg.h>
#include <wxx_dialog.h>
#include <wxx_frame.h>
#include <wxx_listview.h>
#include <wxx_printdialogs.h>
#include <wxx_statusbar.h>
