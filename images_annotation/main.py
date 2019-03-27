from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel,  QInputDialog, QLineEdit, QFileDialog
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QPen

from os import listdir
from os.path import isfile, join

import image_label_lib



class MainWindow():

    def __init__(self):
        self.app = QApplication([])
        self.window = QWidget()
        self.layout = QVBoxLayout()


        self.image_label = image_label_lib.ImageLabel()
        self.image_label.load_image("image.jpg")
        self.image_label.show()

        self.files_list = []
        self.file_idx = 0



        self.button_open_dir = QPushButton("open folder")
        self.button_open_dir.clicked.connect(self.event_open_folder_event)
        self.layout.addWidget(self.button_open_dir)

        self.button_next_file = QPushButton("next")
        self.button_next_file.clicked.connect(self.event_next_file)
        self.layout.addWidget(self.button_next_file)

        self.button_prev_file = QPushButton("prev")
        self.button_prev_file.clicked.connect(self.event_prev_file)
        self.layout.addWidget(self.button_prev_file)

        self.button_background = QPushButton("background")
        self.button_background.clicked.connect(self.event_background)
        self.layout.addWidget(self.button_background)


        self.button_foreground = QPushButton("foreground")
        self.button_foreground.clicked.connect(self.event_foreground)
        self.layout.addWidget(self.button_foreground)


        self.window.setLayout(self.layout)
        self.window.show()
        self.app.exec_()




    def event_open_folder_event(self):
        directory = QFileDialog.getExistingDirectory(self.window, "Select Directory") + "/"

        print("directory name", directory)
        files_list_tmp = [f for f in listdir(directory) if isfile(join(directory, f))]

        self.file_idx = 0
        self.files_list = []
        for i in range(0, len(files_list_tmp)):
            self.files_list.append(directory + files_list_tmp[i])

        print(self.files_list)

        self.select_file()

    def event_next_file(self):
        print(self.file_idx, len(self.files_list))
        if self.file_idx < len(self.files_list):
            self.file_idx = self.file_idx + 1
            self.select_file()

    def event_prev_file(self):
        if self.file_idx > 0:
            self.file_idx = self.file_idx - 1
            self.select_file()

    def event_background(self):
        self.image_label.set_class_id(0)

    def event_foreground(self):
        self.image_label.set_class_id(1)

    def select_file(self):
        self.image_label.load_image(self.files_list[self.file_idx])



main_window = MainWindow()
