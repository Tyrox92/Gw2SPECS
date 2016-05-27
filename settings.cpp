#include "settings.h"

using namespace GW2;
const QString Settings::s_Version = "2.5";
const QString Settings::s_protocolVersion = "1.0";
const QString Settings::s_Product = "Gw2SPECS" + (" " + Settings::s_Version);

//Default Shortcut Binds
const QString Settings::shortcut_reset = "Ctrl+R";
const QString Settings::shortcut_combatmode = "Ctrl+C";
const QString Settings::shortcut_globalreset = "Ctrl+Shift+R";
const QString Settings::shortcut_savelog = "Ctrl+L";
const QString Settings::shortcut_opacity = "Ctrl+O";

Settings::Settings()
{
}
