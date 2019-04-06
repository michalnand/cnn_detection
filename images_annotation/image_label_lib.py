from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtGui import QIcon, QPixmap, QPainter, QPen, QImage, QTransform, QColor

from PyQt5.QtCore import Qt, QTimer

import random

class ImageLabel(QLabel):
    def __init__(self):
        QLabel.__init__(self)

        self.name_idx = 0

        self.output_size_x = 64
        self.output_size_y = 64
        self.augmentation_count = 10
        self.offset_noise = 5
        self.rotation_noise = 45

        self.color_noise_level = 0.1
        self.white_noise_level = 0.1

        self.color_noise_level = 0.02
        self.white_noise_level = 0.02

    def load_image(self, file_name, scale = 1200):

        scaled_width  = 1024
        scaled_height = 768

        scaled_width  = 512
        scaled_height = 512
        input_image = QImage(file_name);

        #self.input_image_scaled = input_image.scaledToWidth(scaled_width)
        #self.input_image_paint = input_image.scaledToWidth(scaled_width)

        self.input_image_scaled = input_image.scaled(scaled_width, scaled_height)
        self.input_image_paint = input_image.scaled(scaled_width, scaled_height)

        pixmap = QPixmap(self.input_image_paint)
        self.setPixmap(pixmap)

        self.width = self.input_image_scaled.size().width()
        self.height = self.input_image_scaled.size().height()

    def set_class_id(self, class_id):
        self.class_id = class_id

    def get_class_id(self):
        return self.class_id

    def mouseReleaseEvent(self, QMouseEvent):

        x0 = QMouseEvent.x()
        y0 = QMouseEvent.y()

        print("point = ", x0, y0, self.width, self.height)

        tmp_size_x = self.output_size_x*2
        tmp_size_y = self.output_size_y*2

        #y0+= tmp_size_y//2


        for i in range(0, self.augmentation_count):
            x = x0 - tmp_size_x//2 + self.rnd_int(-self.offset_noise, self.offset_noise)
            y = y0 - tmp_size_y//2 + self.rnd_int(-self.offset_noise, self.offset_noise)

            if x > 0 and y > 0 and x < self.width - tmp_size_x and y < self.height - tmp_size_y:

                augmented = self.augmentation(x, y, tmp_size_x, tmp_size_y)

                class_folder = "background"
                if self.get_class_id() == 0:
                    class_folder = "background"
                else:
                    class_folder = "foreground"

                result_path = "/home/michal/dataset/mars/"
                name = result_path + class_folder + "/" + str(self.name_idx) + "_" + str(i) + ".png"
                print("saving to ", name)
                augmented.save(name)

        for y in range(0, self.output_size_y//2):
            for x in range(0, self.output_size_x//2):
                r, g, b, a = QColor(self.input_image_scaled.pixel(x*2 ,y*2)).getRgb()

                if self.get_class_id() == 0:
                    r = 0
                    g = 0
                    b = 255
                else:
                    r = 255
                    g = 0
                    b = 0

                self.input_image_paint.setPixel(x*2 + x0 - self.output_size_x//2 , y*2 + y0 - self.output_size_y//4, QColor(r, g, b, a).rgb())

        pixmap = QPixmap(self.input_image_paint)
        self.setPixmap(pixmap)

        self.name_idx+= 1

    def augmentation(self, x, y, size_x, size_y):
        cropped_original = self.input_image_scaled.copy(x, y, size_x, size_y)

        angle = self.rnd_int(-self.rotation_noise, self.rotation_noise)
        t = QTransform()
        t.rotate(angle)
        rotated = cropped_original.transformed(t)

        rotated_size_x = rotated.size().width()
        rotated_size_y = rotated.size().height()

        cropped = rotated.copy(rotated_size_x//4, rotated_size_y//4, self.output_size_x, self.output_size_y)

        self.image_noise(cropped)

        return cropped

    def image_noise(self, image):
        color_range = 255
        r_offset = color_range*self.rnd(-self.color_noise_level, self.color_noise_level)
        g_offset = color_range*self.rnd(-self.color_noise_level, self.color_noise_level)
        b_offset = color_range*self.rnd(-self.color_noise_level, self.color_noise_level)

        if random.random() < 0.5:
            offset_noise = True
        else:
            offset_noise = False

        if random.random() < 0.5:
            white_noise = True
        else:
            white_noise = False

        for y in range(0, image.size().height()):
            for x in range(0, image.size().width()):
                r, g, b, a = QColor(image.pixel(x ,y)).getRgb()

                if offset_noise:
                    r+= r_offset
                    g+= g_offset
                    b+= b_offset

                if white_noise:
                    r+= color_range*self.rnd(-self.white_noise_level, self.white_noise_level)
                    g+= color_range*self.rnd(-self.white_noise_level, self.white_noise_level)
                    b+= color_range*self.rnd(-self.white_noise_level, self.white_noise_level)

                if r < 0:
                    r = 0
                if g < 0:
                    g = 0
                if b < 0:
                    b = 0

                if r > color_range:
                    r = color_range
                if g > color_range:
                    g = color_range
                if b > color_range:
                    b = color_range



                image.setPixel(x, y, QColor(r, g, b, a).rgb())



    def rnd_int(self, range_min, range_max):
        return random.randrange(range_min, range_max + 1)

    def rnd(self, range_min, range_max):
        return random.random()*(range_max - range_min) + range_min
