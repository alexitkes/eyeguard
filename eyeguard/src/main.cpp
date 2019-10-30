#include "defs.h"
#include "eyeguard.h"

/**
 * @brief The main function.
 *
 * The main function just creates a QApplication instance, the main window
 * and starts the event loop.
 */
int main (int argc, char ** argv)
{
    QApplication app (argc, argv);
    EyeGuard eg;

    app.setQuitOnLastWindowClosed (false);

    eg.hide ();

    return (app.exec ());
}
