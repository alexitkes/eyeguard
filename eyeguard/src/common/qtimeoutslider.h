#ifndef __QTIMEOUTSLIDER_H
#define __QTIMEOUTSLIDER_H	1

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

/**
 * @brief The slider used to select timeout duration of several options.
 *
 * A widget containing a text label and a slider.
 */
class QTimeoutSlider: public QWidget {
    Q_OBJECT

public:

    enum SpecialValue {
        Now = 0,
        Never = -1
    };

public:
    QTimeoutSlider (QWidget * parent = 0);

    ~QTimeoutSlider ();

public:
    bool valid (void) const;

    unsigned int value (void) const;
    unsigned int maximum (void) const;
    unsigned int minimum (void) const;

    bool fixedWidthNumbers (void) const;

    QString text (void) const;

    QString formatString (void) const;
    QString neverString (void) const;

    void setFormatString (const QString & str);
    void setNeverString (const QString & str);

public slots:
    void addValue (unsigned int nsec);
    void setValue (unsigned int nsec);
    void delValue (unsigned int nsec);

    void setFixedWidthNumbers (bool fixed);

protected slots:
    void slotValueChanged (int val);
    void slotUpdateLabel (void);

signals:
    void signalValueChanged (int value);

private:
    QSlider *		p_Slider;
    QLabel *		p_Label;

    QList<unsigned int>	p_Values;
    int			p_Current;

    QString			p_FormatString;
    QString			p_NeverString;

    bool			p_FixedWidthNumbers;
};

#endif
