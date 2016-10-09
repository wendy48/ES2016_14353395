/**************************************************************************
	dol_sched_if.h
 
	Scheduler interface for a DOL process    

	(c) 2006 by Alexander Maxiaguine <maxiagui@tik.ee.ethz.ch>

	Computer Engineering and Networks Laboratory, TIK
	Swiss Federal Institute of Technology, ETHZ Zurich 
	Switzerland

**************************************************************************/

/**************************************************************************
	Change Log:

	14.03.06 -- creation

**************************************************************************/

#ifndef DOL_SCHED_IF_H
#define DOL_SCHED_IF_H

class dol_sched_if 
{

public:
	virtual void initialize() = 0;
	virtual int fire() = 0;


protected:
	dol_sched_if() 	{}


private:

    // disabled
    dol_sched_if( const dol_sched_if& );
    dol_sched_if& operator = ( const dol_sched_if& );
};

#endif
