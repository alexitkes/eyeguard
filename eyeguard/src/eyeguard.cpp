#include "eyeguard.h"
#include "settingsdialog.h"

EyeGuard::EyeGuard ( QWidget * parent /* = 0 */ ):
    QWidget (parent)
{
    setWindowTitle (tr ("EyeGuard v") + tr (VERSION_STRING));

    p_TitleLabel = new QLabel (tr ("EyeGuard"));
    p_TrayIcon = 0;

    p_SliderGroup = new QGroupBox (tr ("Options"), this);

    p_MajorSliderLabel = new QLabel (tr ("Display first warning message after:"), p_SliderGroup);
    p_MinorSliderLabel = new QLabel (tr ("Display further warning messages every:"), p_SliderGroup);

    p_AboutButton = new QPushButton (tr ("&About"), this);
    p_SettingsButton = new QPushButton (tr ("Advanced &options..."), this);
    p_HideButton = new QPushButton (tr ("&Hide"), this);
    p_QuitButton = new QPushButton (tr ("&Quit"), this);

    connect (p_AboutButton, SIGNAL (clicked ()), this, SLOT (slotAbout ()));
    connect (p_SettingsButton, SIGNAL (clicked ()), this, SLOT (slotSettings ()));
    connect (p_HideButton, SIGNAL (clicked ()), this, SLOT (hide ()));
    connect (p_QuitButton, SIGNAL (clicked ()), this, SLOT (slotQuit ()));

    QGridLayout * g_layout = new QGridLayout ();

    g_layout->addWidget (p_TitleLabel, 0, 0, 1, 4);
    g_layout->addWidget (p_SliderGroup, 1, 0, 1, 4);
    g_layout->addWidget (p_AboutButton, 2, 0);
    g_layout->addWidget (p_SettingsButton, 2, 1);
    g_layout->addWidget (p_HideButton, 2, 2);
    g_layout->addWidget (p_QuitButton, 2, 3);

    setLayout (g_layout);

    p_MajorTimeout = 3600;
    p_MinorTimeout = 300;
    p_TimerId = 0;
    p_SecondsPassed = 0;
    p_FailCount = 0;
    p_Language = LNG_English;
    p_PromptOnExit = false;
    p_Beep = false;

    p_MajorSlider = new QTimeoutSlider (p_SliderGroup);
    p_MinorSlider = new QTimeoutSlider (p_SliderGroup);

    p_MajorSlider->addValue (900);
    p_MajorSlider->addValue (1200);
    p_MajorSlider->addValue (1500);
    p_MajorSlider->addValue (1800);
    p_MajorSlider->addValue (2100);
    p_MajorSlider->addValue (2400);
    p_MajorSlider->addValue (2700);
    p_MajorSlider->addValue (3000);
    p_MajorSlider->addValue (3600);
    p_MajorSlider->addValue (4500);
    p_MajorSlider->addValue (5400);
    p_MajorSlider->addValue (6300);
    p_MajorSlider->addValue (7200);
    p_MajorSlider->addValue (9000);
    p_MajorSlider->addValue (10800);

    p_MinorSlider->addValue (60);
    p_MinorSlider->addValue (120);
    p_MinorSlider->addValue (180);
    p_MinorSlider->addValue (240);
    p_MinorSlider->addValue (300);
    p_MinorSlider->addValue (600);
    p_MinorSlider->addValue (900);
    p_MinorSlider->addValue (1200);
    p_MinorSlider->addValue (1500);
    p_MinorSlider->addValue (1800);

    QGridLayout * sg_layout = new QGridLayout ();

    sg_layout->addWidget (p_MajorSliderLabel, 0, 0);
    sg_layout->addWidget (p_MajorSlider, 0, 1);
    sg_layout->addWidget (p_MinorSliderLabel, 1, 0);
    sg_layout->addWidget (p_MinorSlider, 1, 1);

    p_SliderGroup->setLayout (sg_layout);

    connect (p_MajorSlider, SIGNAL (signalValueChanged (int)), this, SLOT (slotChangeMajorTimeout (int)));
    connect (p_MinorSlider, SIGNAL (signalValueChanged (int)), this, SLOT (slotChangeMinorTimeout (int)));

    p_MainMenu = new QMenu (this);

    p_AboutAction = p_MainMenu->addAction (tr ("&About"), this, SLOT (slotAbout ()));
    p_ShowAction = p_MainMenu->addAction (tr ("&Show main window"), this, SLOT (show ()));
    p_SettingsAction = p_MainMenu->addAction (tr ("Advanced &options..."), this, SLOT (slotSettings ()));
    p_QuitAction = p_MainMenu->addAction (tr ("&Quit"), this, SLOT (slotQuit ()));

    loadSettings ();

    QTimer::singleShot (1000, this, SLOT (slotStart ()));
}

EyeGuard::~EyeGuard ()
{
    saveSettings ();

    killTimer (p_TimerId);
}

void EyeGuard::loadSettings (void)
{
    QSettings prefs (QDir::homePath () + QString ( "/.eyeguard/eyeguard.ini" ), QSettings::IniFormat);

    p_MajorTimeout = prefs.value ("FirstMessageTimeout", QVariant (3600)) .toInt ();
    p_MinorTimeout = prefs.value ("NextMessageTimeout", QVariant (300)) .toInt ();

    p_MajorSlider->setValue (p_MajorTimeout);
    p_MinorSlider->setValue (p_MinorTimeout);

    QString ls = prefs.value ("Language", QVariant ("English")) .toString ();

    if (ls == QString ("Russian")) {
        slotRussianLanguage ();
    } else {
        slotEnglishLanguage ();
    }

    p_PromptOnExit = prefs.value ("PromptOnExit", QVariant (0)) .toInt ();
    p_Beep = prefs.value ("Beep", QVariant (0)) .toInt ();
}

void EyeGuard::saveSettings (void)
{
    QSettings prefs (QDir::homePath () + QString ( "/.eyeguard/eyeguard.ini" ), QSettings::IniFormat);

    prefs.setValue ("FirstMessageTimeout", p_MajorTimeout);
    prefs.setValue ("NextMessageTimeout", p_MinorTimeout);

    if (p_Language == LNG_Russian) {
        prefs.setValue ("Language", "Russian");
    } else {
        prefs.setValue ("Language", "English");
    }

    prefs.setValue ("PromptOnExit", (int) p_PromptOnExit);
    prefs.setValue ("Beep", (int) p_Beep);
}

void EyeGuard::slotStart (void)
{
    if (QSystemTrayIcon::isSystemTrayAvailable ()) {
        p_TimerId = startTimer (1000);

        p_SecondsPassed = 0;

        p_TrayIcon = new QSystemTrayIcon ( QIcon ( QPixmap (":/resources/eyeguard.png") ), this );

        connect (p_TrayIcon,
                 SIGNAL (activated (QSystemTrayIcon::ActivationReason)),
                 this,
                 SLOT (slotHandleTrayIconActivated (QSystemTrayIcon::ActivationReason)));

        slotUpdateTitleLabel ();

        p_TrayIcon->setContextMenu (p_MainMenu);
        p_TrayIcon->show ();
    } else {
        p_FailCount++;

        if (p_FailCount >= 10) {
            QMessageBox::critical (0,
                                   tr ("EyeGuard"),
                                   tr ("Fatal error occured at EyeGuard start: system tray not available!"),
                                   QMessageBox::Abort);

            qApp->quit ();
        } else {
            QTimer::singleShot (1000, this, SLOT (slotStart ()));
        }
    }
}

void EyeGuard::slotSettings (void)
{
    SettingsDialog sd (this);

    sd.setLanguage (p_Language);
    sd.setPromptOnExit (p_PromptOnExit);
    sd.setBeepNeeded (p_Beep);
    sd.exec ();

    if (sd.result () == QDialog::Accepted) {

        if (sd.language () == LNG_Russian) {
            slotRussianLanguage ();
        } else {
            slotEnglishLanguage ();
        }

        p_PromptOnExit = sd.promptOnExit ();
        p_Beep = sd.beepNeeded ();

    }
}

void EyeGuard::slotEnglishLanguage (void)
{
    p_Language = LNG_English;

    SettingsDialog::setApplicationLanguage (p_Language);
}

void EyeGuard::slotRussianLanguage (void)
{
    p_Language = LNG_Russian;

    SettingsDialog::setApplicationLanguage (p_Language);
}

void EyeGuard::slotQuit (void)
{
    if (p_PromptOnExit) {
        if (QMessageBox::question (this,
                                   tr ("EyeGuard"),
                                   tr ("Do you really want to quit EyeGuard?"),
                                   QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            return;
        }
    }
    qApp->quit ();
}

void EyeGuard::slotAbout (void)
{
    QMessageBox::about (this, tr ("About EyeGuard"),
                        tr ("EyeGuard v%1 by Alex Itkes, 2018\n").arg (VERSION_STRING) +
                        tr ("EyeGuard is a simple program used to prevent you from working (and keeping your eyes tense) for too long time.") +
                        QString (" ") +
                        tr ("It will begin to display annoying warning messages unless you have a break in a specified amount of time."));
}

void EyeGuard::slotChangeMajorTimeout (int value)
{
    p_MajorTimeout = value;

    p_MajorSlider->blockSignals (true);
    p_MajorSlider->setValue (p_MajorTimeout);
    p_MajorSlider->blockSignals (false);
}

void EyeGuard::slotChangeMinorTimeout (int value)
{
    p_MinorTimeout = value;

    p_MinorSlider->blockSignals (true);
    p_MinorSlider->setValue (p_MinorTimeout);
    p_MinorSlider->blockSignals (false);
}

void EyeGuard::slotUpdateTitleLabel (void)
{
    int	hr = p_SecondsPassed / 3600,
        min = (p_SecondsPassed / 60) % 60,
        sec = p_SecondsPassed % 60;

    p_TitleLabel->setText (tr ("EyeGuard v") +
                           tr (VERSION_STRING) +
                           tr (" - you are working for %1 hour(s), %2 minute(s) and %3 second(s).")
                           .arg (hr) .arg (min) .arg (sec) );

    if ( p_TrayIcon ) {
        p_TrayIcon->setToolTip ( tr ("You are working for %1 hour(s), %2 minute(s) and %3 second(s)") .arg (hr) .arg (min) .arg (sec) );
    }
}

void EyeGuard::slotDisplayFirstWarning (void)
{
    int	hr = p_SecondsPassed / 3600,
        min = (p_SecondsPassed / 60) % 60,
        sec = p_SecondsPassed % 60;

    p_TrayIcon->showMessage (tr("EyeGuard Warning"),
                             tr ("You are working for %1 hour(s), %2 minute(s) and %3 second(s).")
                             .arg (hr) .arg (min) .arg (sec) +
                             QString (" ") +
                             tr ("You should have a break as soon as possible."),
                             QSystemTrayIcon::Warning, 10000);

    if (p_Beep) {
        qApp->beep ();
    }
}

void EyeGuard::slotDisplayNextWarning (void)
{
    int	hr = p_SecondsPassed / 3600,
        min = (p_SecondsPassed / 60) % 60,
        sec = p_SecondsPassed % 60;

    p_TrayIcon->showMessage (tr ("EyeGuard Warning"),
                             tr ("You are working for %1 hour(s), %2 minute(s) and %3 second(s).")
                             .arg (hr).arg (min).arg (sec) +
                             QString (" ") +
                             tr ("You should have a break as soon as possible."),
                             QSystemTrayIcon::Warning, 10000);

    if (p_Beep) {
        qApp->beep ();
    }
}

void EyeGuard::slotHandleTrayIconActivated ( QSystemTrayIcon::ActivationReason reason )
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        show ();
    }
}

void EyeGuard::slotUpdateLanguage (void)
{
    setWindowTitle ( tr ("EyeGuard v") + tr (VERSION_STRING) );

    p_AboutButton->setText (tr ("&About"));
    p_SettingsButton->setText (tr ("More &options..."));
    p_HideButton->setText (tr ("&Hide"));
    p_QuitButton->setText (tr ("&Quit"));

    p_SliderGroup->setTitle (tr ("Options"));

    p_MajorSliderLabel->setText (tr ("Display first warning message after:"));
    p_MinorSliderLabel->setText (tr ("Display further warning messages every:"));

    p_MajorSlider->setFormatString (tr ("%1 hour(s) %2 minute(s) %3 second(s)"));
    p_MinorSlider->setFormatString (tr ("%1 hour(s) %2 minute(s) %3 second(s)"));

    p_AboutAction->setText (tr ("&About"));
    p_ShowAction->setText (tr ("&Show main window"));
    p_SettingsAction->setText (tr ("Advanced &options..."));
    p_QuitAction->setText (tr ("&Quit"));

    slotUpdateTitleLabel ();
}

void EyeGuard::closeEvent (QCloseEvent * event)
{
    hide ();

    event->ignore ();
}

void EyeGuard::timerEvent (QTimerEvent * event)
{
    if (event->timerId () == p_TimerId) {
        p_SecondsPassed++;

        slotUpdateTitleLabel ();

        if (p_SecondsPassed == p_MajorTimeout) {
            slotDisplayFirstWarning ();
        }

        if (p_SecondsPassed > p_MajorTimeout && ! ((p_SecondsPassed - p_MajorTimeout) % p_MinorTimeout)) {
            slotDisplayNextWarning ();
        }

        event->accept ();
    } else {
        event->ignore ();
    }
}

bool EyeGuard::event (QEvent * evt)
{
    if (evt->type () == QEvent::LanguageChange) {
        slotUpdateLanguage ();

        return true;
    } else {
        return (QWidget::event (evt));
    }
}
