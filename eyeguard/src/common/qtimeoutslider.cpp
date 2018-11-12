#include "qtimeoutslider.h"

QTimeoutSlider::QTimeoutSlider ( QWidget * parent /* = 0 */ ):
    QWidget (parent)
{

	p_FormatString = QString ( "%1 hour(s) %2 minute(s) %3 second(s)" );
	p_NeverString = QString ( "Never" );
	p_FixedWidthNumbers = false;
	
	p_Slider = new QSlider ( Qt::Horizontal, this );
	p_Label = new QLabel ( p_NeverString, this );
	
	p_Values = QList<unsigned int> ();
	p_Current = -1;
	
	QVBoxLayout * layout = new QVBoxLayout ();
	
	layout->addWidget (p_Slider);
	layout->addWidget (p_Label);
	
	setLayout (layout);
	
	p_Slider->setMinimum (0);
	p_Slider->setMaximum (0);
	p_Slider->setValue (0);
	
	connect ( p_Slider, SIGNAL (valueChanged (int)), this, SLOT (slotValueChanged (int)) );

}

QTimeoutSlider::~QTimeoutSlider ()
{
}

bool
QTimeoutSlider::valid (void) const
{

	if ( p_Values.count () && p_Current >= 0 )
	    return true;
	else
	    return false;

}

unsigned int
QTimeoutSlider::value (void) const
{

	if ( p_Values.count () && p_Current >= 0 )
	    return ( p_Values.at (p_Current) );
	else
	    return (Never);

}

unsigned int
QTimeoutSlider::maximum (void) const
{

	if ( p_Values.empty () )
	    return (Never);
	else
	    return ( p_Values.last () );

}

unsigned int
QTimeoutSlider::minimum (void) const
{

	if ( p_Values.empty () )
	    return (Never);
	else
	    return ( p_Values.first () );

}

bool
QTimeoutSlider::fixedWidthNumbers (void) const
{

	return (p_FixedWidthNumbers);

}

QString
QTimeoutSlider::text (void) const
{

	return ( p_Label->text () );

}

QString
QTimeoutSlider::formatString (void) const
{

	return (p_FormatString);

}

QString
QTimeoutSlider::neverString (void) const
{

	return (p_NeverString);

}

void
QTimeoutSlider::setFormatString ( const QString & str )
{

	p_FormatString = str;
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::setNeverString ( const QString & str )
{

	p_NeverString = str;
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::addValue ( unsigned int nsec )
{

	int	i, n = p_Values.count ();
	
	for ( i = 0; i < n && p_Values.at (i) < nsec; i++ ) ;
	
	if ( i >= n )
	    p_Values.append (nsec);
	
	else if ( p_Values.at (i) > nsec )
	    p_Values.insert ( i, nsec );
	
	p_Slider->setMinimum (0);
	p_Slider->setMaximum (p_Values.count () - 1);
	
	if ( p_Current == -1 || i < p_Current )
	    p_Current++;
	
	p_Slider->setValue (p_Current);
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::setValue ( unsigned int nsec )
{

	int	i, n = p_Values.count ();
	
	for ( i = 0; i < n && p_Values.at (i) < nsec; i++ ) ;
	
	if ( i >= n )
	    p_Values.append (nsec);
	
	else if ( p_Values.at (i) > nsec )
	    p_Values.insert ( i, nsec );
	
	p_Slider->setMinimum (0);
	p_Slider->setMaximum (p_Values.count () - 1);
	
	p_Current = i;
	
	p_Slider->setValue (p_Current);
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::delValue ( unsigned int nsec )
{

	int	idx = p_Values.indexOf (nsec);
	
	if ( idx == -1 )
	    return;
	
	if ( idx == p_Current ) {
	
		if ( p_Current > 0 ) {
		
			p_Current--;
			p_Values.removeAt (idx);
			
			p_Slider->setMinimum (0);
			p_Slider->setMaximum (p_Values.count () - 1);
			
			p_Slider->setValue (p_Current);
		
		} else if ( p_Values.count () > 1 ) {
		
			p_Values.removeAt (idx);
			
			p_Slider->setMinimum (0);
			p_Slider->setMaximum (p_Values.count () - 1);
			
			p_Slider->setValue (p_Current);
		
		} else {
		
			p_Values.removeAt (idx);
			
			p_Slider->setMinimum (0);
			p_Slider->setMaximum (0);
			p_Slider->setValue (0);
			
			p_Current = -1;
		
		}
	
	} else {
	
		p_Values.removeAt (idx);
	
		p_Slider->setMinimum (0);
		p_Slider->setMaximum (p_Values.count () - 1);
	
	}
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::setFixedWidthNumbers ( bool fixed )
{

	p_FixedWidthNumbers = fixed;
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::slotValueChanged (int val)
{

	p_Current = val;
	
	emit signalValueChanged (p_Values.at (val));
	
	slotUpdateLabel ();

}

void
QTimeoutSlider::slotUpdateLabel (void)
{

	unsigned int nsec = Never;
	
	if ( p_Values.count () && p_Current >= 0 )
	    nsec = p_Values.at (p_Current);
	
	if ( nsec == (unsigned int) Never ) {
	
		p_Label->setText (p_NeverString);
	
	} else {
	
		int	hr = nsec / 3600,
			min = (nsec / 60) % 60,
			sec = nsec % 60;
		
		if ( p_FixedWidthNumbers )
		    p_Label->setText ( p_FormatString .arg (hr, 2, 10, QChar('0')) .arg (min, 2, 10, QChar('0')) .arg (sec, 2, 10, QChar('0')) );
		else
		    p_Label->setText ( p_FormatString .arg (hr) .arg (min) .arg (sec) );
	
	}

}

QString
QTimeoutSlider::versionString (void)
{

	return ( QString ( "%1.%2.%3.%4" ) .arg (majorVersion ()) .arg (minorVersion ()) .arg (microVersion ()) .arg (buildVersion ()) );

}
