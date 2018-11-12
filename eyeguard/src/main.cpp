#include "defs.h"
#include "eyeguard.h"

int main (int argc, char ** argv)
{
    QApplication app (argc, argv);
    EyeGuard eg;

    app.setQuitOnLastWindowClosed (false);

    eg.hide ();

    return (app.exec ());
}
