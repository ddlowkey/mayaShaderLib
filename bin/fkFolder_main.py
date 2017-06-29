from PySide import QtGui
from PySide import QtCore
import pysideuic
import xml.etree.ElementTree as xml
from cStringIO import StringIO
import sys
import os

FK_PLATE_PATH = r'W:'
FK_PROJECT_PATH = r'X:'
FK_RENDER_PATH = r'Y:'

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


ui_base, ui_form = loadUiType(r'V:\pipeline\fkToolkit\ui\fkFolder.ui')





class pySideTest_Class(ui_base, ui_form):
    def __init__(self):
        super(pySideTest_Class, self).__init__()
        self.setupUi(self)


        projectList = self.get_project_list()
        self.cb_project.addItems(projectList)
        self.cb_project.activated.connect(lambda: self.refresh_sequence_name())
        self.cb_project.activated.connect(lambda: self.refresh_shot_name())
        self.cb_sequence_name.activated.connect(lambda: self.refresh_shot_name())

        self.cb_assets_type.addItems(['props', 'element2d', 'environment', 'character'])
        self.cb_assets_task_name.addItems(['mdl', 'shd', 'txt', 'rig', 'mgfx'])
        self.pb_add_assets.clicked.connect(lambda: self.add_assets_dir())

        self.cb_shot_task_name.addItems(['comp', 'comp_roto', 'comp_slap', 'mp', 'anim', 'cgfx', 'layout', 'lgt', 'mm'])

        self.pb_add_shot.clicked.connect(lambda: self.add_shot_dir())
        self.pb_add_basic.clicked.connect(lambda: self.add_basic_dir())
        self.pb_create_folder.clicked.connect(lambda: self.create_dir())

    def refresh_sequence_name(self):
        self.cb_sequence_name.clear()
        self.cb_sequence_name.addItems(self.get_sequence_list())

    def refresh_shot_name(self):
        self.cb_shot_name.clear()
        self.cb_shot_name.addItems(self.get_shot_list())


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
        file_path = []
        for index in self.lw_create_path_list.findItems("",QtCore.Qt.MatchContains):
            file_path.append(index.text())
            print index.text()
        for f in file_path:
            if not os.path.exists(f):
                os.makedirs(f)

    def add_assets_dir(self):
        file_path = self.get_x_assets_path() + self.get_y_assets_path()
        self.lw_create_task_list.addItem(self.get_x_assets_path()[0])
        for f in file_path:
            if not os.path.exists(f):
                self.lw_create_path_list.addItem(f)

    def add_shot_dir(self):
        file_path = self.get_x_shot_path() + self.get_y_shot_path()
        self.lw_create_task_list.addItem(self.get_x_shot_path()[0])
        for f in file_path:
            if not os.path.exists(f):
                self.lw_create_path_list.addItem(f)

    def add_basic_dir(self):
        file_path = self.get_basic_path()
        self.lw_create_task_list.addItems(self.get_basic_path())
        for f in file_path:
            if not os.path.exists(f):
                self.lw_create_path_list.addItem(f)

    def get_project_list(self):
        folderList = []
        itemList = os.listdir(FK_PLATE_PATH)

        for i in itemList:
            if os.path.isdir(FK_PLATE_PATH + "\\" + i):
                if '-' in i:
                    if '_' in i:
                        folderList = folderList+[i]

        return folderList

    def get_sequence_list(self):
        sequenceList = []
        project_name = self.cb_project.currentText()
        itemList = os.listdir(FK_PLATE_PATH + "\\" + project_name)

        for i in itemList:
            if os.path.isdir(FK_PLATE_PATH + "\\" + project_name + "\\" + i):
                if not '_' in i:
                    sequenceList = sequenceList+[i]

        return sequenceList

    def get_shot_list(self):
        shotList = []
        project_name = self.cb_project.currentText()
        sequence_name = self.cb_sequence_name.currentText()
        itemList = os.listdir(FK_PLATE_PATH + "\\" + project_name + "\\" + sequence_name)

        for i in itemList:
            if os.path.isdir(FK_PLATE_PATH + "\\" + project_name + "\\" + sequence_name + "\\" + i):
                if not '_' in i:
                    shotList = shotList+[i]

        return shotList

    def get_x_assets_path(self):
        assets_type = self.cb_assets_type.currentText()
        project_name = self.cb_project.currentText()
        assets_name = self.get_assets_type_abbr() + self.le_assets_name.text()
        task_name = self.cb_assets_task_name.currentText()

        x_assets_path = FK_PROJECT_PATH + "\\" + project_name + "\\" + "_library" + "\\" + "assets" + "\\" + assets_type + "\\" + assets_name + "\\" + task_name
        x_assets_path_publish = x_assets_path + "\\" + '_publish'
        x_assets_path_workaera = x_assets_path + "\\" + '_workaera'
        x_assets_path_export = [x_assets_path + "\\" + r'_export\abc', x_assets_path + "\\" + r'_export\fbx', x_assets_path + "\\" + r'_export\obj', x_assets_path + "\\" + r'_export\cam']
        return [x_assets_path, x_assets_path_publish, x_assets_path_workaera] + x_assets_path_export

    def get_y_assets_path(self):
        assets_type = self.cb_assets_type.currentText()
        project_name = self.cb_project.currentText()
        assets_name = self.get_assets_type_abbr() + self.le_assets_name.text()
        task_name = self.cb_assets_task_name.currentText()

        y_assets_path = FK_RENDER_PATH + "\\" + project_name + "\\" + "_library" + "\\" + "assets" + "\\" + assets_type + "\\" + assets_name + "\\" + 'renders' + "\\" + task_name
        return [y_assets_path]


    def get_x_shot_path(self):
        project_name = self.cb_project.currentText()
        sequence_name = self.cb_sequence_name.currentText()
        shot_name = self.cb_shot_name.currentText()
        task_type = 'noneType'
        task_name = self.cb_shot_task_name.currentText()

        if task_name in ['comp', 'comp_roto', 'comp_slap', 'mp']:
            task_type = '2d'
        elif task_name in ['anim', 'cgfx', 'layout', 'lgt', 'mm']:
            task_type = '3d'

        x_shot_path = FK_PROJECT_PATH + "\\" + project_name + "\\" + sequence_name + "\\" + shot_name + "\\" + task_type + "\\" + task_name
        return [x_shot_path]

    def get_y_shot_path(self):
        project_name = self.cb_project.currentText()
        sequence_name = self.cb_sequence_name.currentText()
        shot_name = self.cb_shot_name.currentText()
        task_type = 'noneType'
        task_name = self.cb_shot_task_name.currentText()

        if task_name in ['comp', 'comp_roto', 'comp_slap', 'mp']:
            task_type = '2d'
        elif task_name in ['anim', 'cgfx', 'layout', 'lgt', 'mm']:
            task_type = '3d'

        y_shot_path = FK_RENDER_PATH + "\\" + project_name + "\\" + sequence_name + "\\" + shot_name + "\\" + task_type + "\\" + task_name
        return [y_shot_path]

    def get_basic_path(self):

        project_name = self.cb_project.currentText()

        w_basic_edit = FK_PLATE_PATH + "\\" + project_name + "\\" + '_edit'
        w_basic_input = FK_PLATE_PATH + "\\" + project_name + "\\" + '_input'
        w_basic_output = FK_PLATE_PATH + "\\" + project_name + "\\" + '_output'
        w_basic_work = [FK_PLATE_PATH + "\\" + project_name + "\\" + '_work' + "\\" + 'doc', FK_PLATE_PATH + "\\" + project_name + "\\" + '_work' + "\\" + 'refrence']
        w_basic_temp = FK_PLATE_PATH + "\\" + project_name + "\\" + '_temp'
        w_basic_example_temp = FK_PLATE_PATH + "\\" + project_name + "\\" + 'example' + "\\" + '0000' + "\\" + 'elements' + "\\" + 'plates'
        w_basic_example = [w_basic_example_temp + "\\" + 'plate', w_basic_example_temp + "\\" + 'plate_cutref', w_basic_example_temp + "\\" + 'plate_ud']
        w_basic_path = [w_basic_edit, w_basic_input, w_basic_output, w_basic_temp] + w_basic_work + w_basic_example

        x_basic_temp = FK_PROJECT_PATH + "\\" + project_name + "\\" + '_temp'
        x_basic_concept = FK_PROJECT_PATH + "\\" + project_name + "\\" + '_library' + "\\" + 'concept'
        x_basic_hdr = FK_PROJECT_PATH + "\\" + project_name + "\\" + '_library' + "\\" + 'hdr'
        x_basic_lut = FK_PROJECT_PATH + "\\" + project_name + "\\" + '_library' + "\\" + 'lut'
        x_basic_previz = FK_PROJECT_PATH + "\\" + project_name + "\\" + '_library' + "\\" + 'previz'
        x_basic_path = [x_basic_temp, x_basic_concept, x_basic_hdr, x_basic_lut, x_basic_previz]

        y_basic_temp = FK_RENDER_PATH + "\\" + project_name + "\\" + '_temp'
        y_basic_daily = FK_RENDER_PATH + "\\" + project_name + "\\" + '_daily'
        y_basic_path = [y_basic_temp, y_basic_daily]

        return w_basic_path + x_basic_path + y_basic_path

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    gui = pySideTest_Class()
    gui.show()
    app.exec_()