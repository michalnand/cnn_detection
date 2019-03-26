from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel,  QInputDialog, QLineEdit, QFileDialog
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QPen

import image_label_lib



app = QApplication([])
window = QWidget()
layout = QVBoxLayout()

layout.addWidget(QPushButton('Top'))
layout.addWidget(QPushButton('Bottom'))



image_label = image_label_lib.ImageLabel()
image_label.load_image("image.jpg")
image_label.show()

window.setLayout(layout)
window.show()


filename = QFileDialog.getOpenFileName(window, "Open File", filter='All Files(*.*);; BMP (*.bmp);; JPG (*.jpg);; JPEG (*.jpeg);; PNG (*.png)')
print("file name", filename)

app.exec_()
