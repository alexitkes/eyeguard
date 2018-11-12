#ifndef __EYEGUARD_H
#define __EYEGUARD_H	1

#include "defs.h"

/**
 * @brief Main window of the EyeGuard tool.
 *
 * This is the class of the main window of the Eye Guardian tool.
 */
class EyeGuard: public QWidget
{
    Q_OBJECT

public:
    EyeGuard (QWidget * parent = 0);

    ~EyeGuard ();

public:
    void loadSettings (void);
    void saveSettings (void);

public slots:
    void slotStart (void);
    void slotSettings (void);
    void slotEnglishLanguage (void);
    void slotRussianLanguage (void);
    void slotQuit (void);
    void slotAbout (void);

    void slotChangeMajorTimeout (int value);
    void slotChangeMinorTimeout (int value);
    void slotUpdateTitleLabel (void);
    void slotDisplayFirstWarning (void);
    void slotDisplayNextWarning (void);
    void slotHandleTrayIconActivated (QSystemTrayIcon::ActivationReason reason);
    void slotUpdateLanguage (void);

protected:
    virtual
    void closeEvent ( QCloseEvent * event);
    virtual
    void timerEvent (QTimerEvent * event);
    virtual
    bool event (QEvent * evt);

private:
    int	p_MajorTimeout;
    int	p_MinorTimeout;
    int	p_TimerId;
    int	p_SecondsPassed;
    int	p_FailCount;
    int	p_Language;
    bool	p_PromptOnExit;
    bool	p_Beep;

    QLabel * p_TitleLabel;
    QLabel * p_MajorSliderLabel;
    QLabel * p_MinorSliderLabel;

    QPushButton * p_AboutButton;
    QPushButton * p_SettingsButton;
    QPushButton * p_HideButton;
    QPushButton * p_QuitButton;

    QGroupBox * p_SliderGroup;

    QTimeoutSlider * p_MajorSlider;
    QTimeoutSlider * p_MinorSlider;

    QMenu * p_MainMenu;

    QAction * p_AboutAction;
    QAction * p_SettingsAction;
    QAction * p_ShowAction;
    QAction * p_QuitAction;

    QSystemTrayIcon * p_TrayIcon;
};

#endif
