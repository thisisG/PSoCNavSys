__author__ = 'Geir'

import serial
import io


class TestInterface:
    def __init__(self):
        """
        Add an init method at a later time
        :return:
        """
        self.psoc_serial.baudrate = 9600
        self.psoc_serial.bytesize = 8
        self.psoc_serial.parity = 'None'

    psoc_serial = serial.Serial()
