#include "settingsdialog.h"

/**
 * @brief Advanced options dialog constructor.
 */
SettingsDialog::SettingsDialog (QWidget * parent):
    QDialog (parent)
{
    setWindowTitle (tr ("EyeGuard advanced options"));

    p_Language = LNG_English;
    p_PromptOnExit = false;

    p_TabBox = new QTabWidget (this);

    p_LanguageBox = new QFrame (this);
    p_MiscBox = new QFrame (this);

    englishButton = new QRadioButton (tr ("&English"), p_LanguageBox);
    russianButton = new QRadioButton (tr ("&Russian"), p_LanguageBox);

    p_PromptBox = new QCheckBox (tr ("&Prompt on quit"), p_MiscBox);
    p_BeepBox = new QCheckBox (tr ("&Beep when displaying warning messages"), p_MiscBox);

    QGridLayout * lang_layout = new QGridLayout ();

    lang_layout->addWidget (englishButton, 0, 0);
    lang_layout->addWidget (russianButton, 1, 0);

    p_LanguageBox->setLayout (lang_layout);

    QGridLayout * misc_layout = new QGridLayout ();

    misc_layout->addWidget (p_PromptBox, 0, 0);
    misc_layout->addWidget (p_BeepBox, 1, 0);

    p_MiscBox->setLayout (misc_layout);

    p_LanguageBox->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_MiscBox->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);

    p_TabBox->addTab (p_LanguageBox, tr ("&Language"));
    p_TabBox->addTab (p_MiscBox, tr ("&Miscellaneous"));

    okButton = new QPushButton (tr ("&OK"), this);
    cancelButton = new QPushButton (tr ("&Cancel"), this);

    connect (okButton, SIGNAL (clicked ()), this, SLOT (accept ()));
    connect (cancelButton, SIGNAL (clicked ()), this, SLOT (reject ()));

    QGridLayout * layout = new QGridLayout ();

    layout->addWidget (p_TabBox, 0, 0, 1, 2 );
    layout->addWidget (okButton, 1, 0);
    layout->addWidget (cancelButton, 1, 1);

    setLayout (layout);

    englishButton->setChecked (true);
    russianButton->setChecked (true);
    setPromptOnExit (false);
    setBeepNeeded (false);

    okButton->setFocus ();
}

SettingsDialog::~SettingsDialog ()
{
    /* The destructor does nothing... */
}

int SettingsDialog::language (void) const
{
    return (p_Language);
}

bool SettingsDialog::promptOnExit (void) const
{
    return (p_PromptOnExit);
}

bool SettingsDialog::beepNeeded (void) const
{
    return (p_Beep);
}

void SettingsDialog::setLanguage (int lang)
{
    p_Language = lang;

    if (p_Language == LNG_Russian) {
        englishButton->setChecked (false);
        russianButton->setChecked (true);

        setApplicationLanguage (lang);
    } else {
        p_Language = LNG_English;

        englishButton->setChecked (true);
        russianButton->setChecked (false);

        setApplicationLanguage (lang);
    }
}

void SettingsDialog::setPromptOnExit (bool prompt)
{
    p_PromptOnExit = prompt;

    if (p_PromptOnExit) {
        p_PromptBox->setCheckState (Qt::Checked);
    } else {
        p_PromptBox->setCheckState (Qt::Unchecked);
    }
}

void SettingsDialog::setBeepNeeded (bool beep)
{
    p_Beep = beep;

    if (p_Beep) {
        p_BeepBox->setCheckState (Qt::Checked);
    } else {
        p_BeepBox->setCheckState (Qt::Unchecked);
    }
}

void SettingsDialog::slotUpdateLanguage (void)
{
    setWindowTitle (tr ("EyeGuard advanced options"));

    englishButton->setText (tr ("&English"));
    russianButton->setText (tr ("&Russian"));

    p_PromptBox->setText (tr ("&Prompt on quit"));
    p_BeepBox->setText (tr ("&Beep when displaying warning messages"));

    p_TabBox->setTabText (p_TabBox->indexOf (p_LanguageBox), tr ("&Language"));
    p_TabBox->setTabText (p_TabBox->indexOf (p_MiscBox), tr ("&Miscellaneous"));

    okButton->setText (tr ("&OK"));
    cancelButton->setText (tr ("&Cancel"));
}

void SettingsDialog::setApplicationLanguage (int lang)
{
    static QTranslator * ru_translator;

    if (lang == LNG_English) {
        qApp->removeTranslator (ru_translator);
    } else if (lang == LNG_Russian) {
        if (!ru_translator) {
            ru_translator = new QTranslator ();

            ru_translator->load (":/resources/russian.qm");
        }
        qApp->installTranslator (ru_translator);
    }
}

void SettingsDialog::accept (void)
{
    if (russianButton->isChecked ()) {
        p_Language = LNG_Russian;
    } else {
        p_Language = LNG_English;
    }

    setApplicationLanguage (p_Language);

    if (p_PromptBox->checkState () == Qt::Checked) {
        p_PromptOnExit = true;
    } else {
        p_PromptOnExit = false;
    }

    if (p_BeepBox->checkState () == Qt::Checked) {
        p_Beep = true;
    } else {
        p_Beep = false;
    }

    QDialog::accept ();
}

bool SettingsDialog::event (QEvent * evt)
{
    if (evt->type () == QEvent::LanguageChange) {
        slotUpdateLanguage ();

        return true;
    } else {
        return (QWidget::event (evt));
    }
}
