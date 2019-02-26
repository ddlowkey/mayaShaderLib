#include "pluginShaderLibMain.h"
#include "main_form.h"

#include <maya/MFnPlugin.h>


QPointer<UiWidget> MainCmd::objectCreator;
const MString MainCmd::commandName("cmdShaderLib");

void MainCmd::cleanup()
{
	if (!objectCreator.isNull()) delete objectCreator;
}

MStatus MainCmd::doIt(const MArgList&)
{
	if (objectCreator.isNull())
	{
		objectCreator = new UiWidget();
	}
	else
	{
		objectCreator->showNormal();
		objectCreator->raise();
	}

	return MS::kSuccess;
}

MStatus initializePlugin(MObject plugin)
{
	MFnPlugin pluginFn(plugin, "y Frank Ding & Javier Yang", "1.0", "Any");
	CHECK_MSTATUS(pluginFn.registerCommand(MainCmd::commandName, MainCmd::creator));

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject plugin)
{
	MFnPlugin pluginFn(plugin, "by Frank Ding & Javier Yang", "1.0", "Any");
	MainCmd::cleanup();
	CHECK_MSTATUS(pluginFn.deregisterCommand(MainCmd::commandName));

	return MS::kSuccess;
}
