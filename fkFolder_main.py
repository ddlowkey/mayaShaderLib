from PySide.QtGui import *
from PySide import QtGui
import pysideuic
import xml.etree.ElementTree as xml
from cStringIO import StringIO
import sys
import os

FK_PROJECT_PATH = r'G:\MyMaya\script\fkToolkit\testAera\x'
FK_RENDER_PATH = r'G:\MyMaya\script\fkToolkit\testAera\y'


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


ui_base, ui_form = loadUiType(r'G:\MyMaya\script\fkToolkit\fkFolder.ui')





class pySideTest_Class(ui_base, ui_form):
    def __init__(self):
        super(pySideTest_Class, self).__init__()
        self.setupUi(self)

        projectList = self.get_project_list()
        self.cb_project.addItems(projectList)
        self.cb_assets_type.addItems(['props', 'element2d', 'environment', 'character'])
        self.cb_assets_task_name.addItems(['mdl', 'shd', 'txt', 'rig', 'mgfx'])
        self.pb_add_assets.clicked.connect(lambda: self.add_assets_dir())

        self.cb_shot_task_name.addItems(['comp', 'comp-slap', 'cgfx', 'mgfx', 'mp'])



    def get_assets_type_abbr(self):
        assets_type_abbr = self.cb_assets_type.currentText()

        if assets_type_abbr == 'props':
            return 'prp_'
        elif assets_type_abbr == 'element2d':
            return 'e2d_'
        elif assets_type_abbr == 'character':
            return 'chr_'
        elif assets_type_abbr == 'creature':
            return 'crt_'
        elif assets_type_abbr == 'environment':
            return 'env_'


    def create_dir(self):
        file_path = self.get_x_assets_path() + self.get_y_assets_path()
        for f in file_path:
            if not os.path.exists(f):
                os.makedirs(f)
                self.lw_create_list.addItem(f)
                self.te_create_list.append(f)

    def add_assets_dir(self):
        file_path = self.get_x_assets_path() + self.get_y_assets_path()
        self.lw_create_list.addItem(self.get_x_assets_path()[0])
        for f in file_path:
            if not os.path.exists(f):
                self.te_create_list.append(f)

    def get_project_list(self):
        folderList = []
        itemList = os.listdir(FK_PROJECT_PATH)

        for i in itemList:
            if os.path.isdir(FK_PROJECT_PATH + "\\" + i):
                folderList = folderList+[i]

        return folderList

    def get_x_assets_path(self):
        assets_type = self.cb_assets_type.currentText()
        project_name = self.cb_project.currentText()
        assets_name = self.get_assets_type_abbr() + self.le_assets_name.text()
        task_name = self.cb_assets_task_name.currentText()

        x_assets_path = FK_PROJECT_PATH + "\\" + project_name + "\\" + "_library" + "\\" + "assets" + "\\" + assets_type + "\\" + assets_name + "\\" + task_name
        x_assets_path_publish = x_assets_path + "\\" + '_publish'
        x_assets_path_workaera = x_assets_path + "\\" + '_workaera'
        return [x_assets_path, x_assets_path_publish, x_assets_path_workaera]

    def get_y_assets_path(self):
        assets_type = self.cb_assets_type.currentText()
        project_name = self.cb_project.currentText()
        assets_name = self.get_assets_type_abbr() + self.le_assets_name.text()
        task_name = self.cb_assets_task_name.currentText()

        y_assets_path = FK_RENDER_PATH + "\\" + project_name + "\\" + "_library" + "\\" + "assets" + "\\" + assets_type + "\\" + assets_name + "\\" + 'renders' + "\\" + task_name
        return [y_assets_path]




if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    gui = pySideTest_Class()
    gui.show()
    app.exec_()