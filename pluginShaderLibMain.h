#ifndef PLUGINSHADERLIBMAIN_H
#define PLUGINSHADERLIBMAIN_H

#include "main_form.h"

#include <maya/MPxCommand.h>


class MainCmd : public MPxCommand
{
public:
	static void		            cleanup();
	static void*	            creator() { return new MainCmd(); }

	MStatus			            doIt(const MArgList& args);

	static QPointer<UiWidget>	objectCreator;
	static const MString		commandName;
};

#endif
