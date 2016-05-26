#include "settings.h"

using namespace GW2;
const QString Settings::s_Version = "2.5";
const QString Settings::s_protocolVersion = "1.0";
const QString Settings::s_Product = "Gw2SPECS" + (" " + Settings::s_Version);

//Default Shortcut Binds
const QString Settings::shortcut_reset = "Ctrl+R";

Settings::Settings()
{
}
