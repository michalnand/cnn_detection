from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QPen, QImage, QTransform

from PyQt5.QtCore import Qt, QTimer

import random

class ImageLabel(QLabel):
    def __init__(self):
        QLabel.__init__(self)

        self.name_idx = 0

        self.output_size_x = 64
        self.output_size_y = 64
        self.augmentation_count = 100
        self.offset_noise = 2
        self.rotation_noise = 45


    def load_image(self, file_name, scale = 1200):
        input_image = QImage(file_name);
        self.input_image_scaled = input_image.scaledToWidth(scale)

        pixmap = QPixmap(self.input_image_scaled)
        self.setPixmap(pixmap)

        self.width = self.input_image_scaled.size().width()
        self.height = self.input_image_scaled.size().height()


    def mouseReleaseEvent(self, QMouseEvent):

        x0 = QMouseEvent.x()
        y0 = QMouseEvent.y()

        print("point = ", x0, y0)

        for i in range(0, self.augmentation_count):
            x = x0 - self.output_size_x + self.rnd_int(-self.offset_noise, self.offset_noise)
            y = y0 - self.output_size_y + self.rnd_int(-self.offset_noise, self.offset_noise)
            #y+= self.output_size_y//2

            if x > 0 and y > 0 and x < self.width - 2*self.output_size_x and y < self.height - 2*self.output_size_y:

                augmented = self.augmentation(x, y)

                name = "result/" + str(self.name_idx) + "_" + str(i) + ".jpg"
                print("saving to ", name)
                augmented.save(name)


        self.name_idx+= 1

    def augmentation(self, x, y):
        cropped_original = self.input_image_scaled.copy(x, y, 2*self.output_size_x, 2*self.output_size_y)

        angle = self.rnd_int(-self.rotation_noise, self.rotation_noise)
        t = QTransform()
        t.rotate(angle)
        rotated = cropped_original.transformed(t)

        cropped = rotated.copy(self.output_size_x, self.output_size_y, self.output_size_x, self.output_size_y)
        #cropped = rotated.copy(self.output_size_x//2, self.output_size_y, self.output_size_x, self.output_size_y)


        return cropped

    def rnd_int(self, range_min, range_max):



        return random.randrange(range_min, range_max + 1)
