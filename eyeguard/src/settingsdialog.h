#ifndef __SETTINGSDIALOG_H
#define __SETTINGSDIALOG_H	1

#include "defs.h"

/**
 * @brief Settings Dialog for Eye Guardian
 *
 * This is the class for the Eye Guardian Settings Dialog box. It is being used to
 * select program interface language and configure some additional options, including
 * prompt on exit.
 */
class SettingsDialog: public QDialog {
    Q_OBJECT

public:
    SettingsDialog (QWidget * parent = 0);
    ~SettingsDialog ();

public:
    int language (void) const;
    bool promptOnExit (void) const;
    bool beepNeeded (void) const;

public slots:
    void setLanguage (int lang);
    void setPromptOnExit (bool prompt);
    void setBeepNeeded (bool beep);

    void slotUpdateLanguage (void);

    virtual
    void accept (void);

public:
    static void setApplicationLanguage (int lang);

protected:
    virtual
    bool event (QEvent * evt);

private:
    int p_Language;
    bool p_PromptOnExit;
    bool p_Beep;

    QRadioButton * englishButton;
    QRadioButton * russianButton;

    QTabWidget * p_TabBox;
    QFrame * p_LanguageBox;
    QFrame * p_MiscBox;

    QCheckBox * p_PromptBox;
    QCheckBox * p_BeepBox;

    QPushButton * okButton;
    QPushButton * cancelButton;
};

#endif
