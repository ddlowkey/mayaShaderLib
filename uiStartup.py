from PySide import QtGui
import pysideuic
import xml.etree.ElementTree as xml
from cStringIO import StringIO
import sys


def loadUiType(uiFile):
    """
    Pyside "loadUiType" command like PyQt4 has one, so we have to convert the
    ui file to py code in-memory first and then execute it in a special frame
    to retrieve the form_class.
    """
    parsed = xml.parse(uiFile)
    widget_class = parsed.find('widget').get('class')
    form_class = parsed.find('class').text

    with open(uiFile, 'r') as f:
        o = StringIO()
        frame = {}

        pysideuic.compileUi(f, o, indent=0)
        pyc = compile(o.getvalue(), '<string>', 'exec')
        exec pyc in frame

        # Fetch the base_class and form class based on their type
        # in the xml from designer
        form_class = frame['Ui_%s' % form_class]
        base_class = eval('QtGui.%s' % widget_class)

    return form_class, base_class


ui_base, ui_form = loadUiType('G:\MyMaya\script\pySideTest\pySideTest.ui')


class pySideTest_Class(ui_base, ui_form):
    def __init__(self):
        super(pySideTest_Class, self).__init__()
        self.setupUi(self)


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    gui = pySideTest_Class()
    gui.show()
    app.exec_()