__author__ = 'Geir'

import serial


class TestInterface:
    psoc_serial = serial.Serial()
    csv_file_name = "default.csv"
    line_buffer = ""

    def __init__(self):
        """
        Add an init method at a later time
        :return:
        """
        self.psoc_serial.baudrate = 9600
        self.psoc_serial.bytesize = 8
        self.psoc_serial.parity = 'N'

    def set_file_name(self, filename):
        self.csv_file_name = filename

    def start_logging_data(self):
        if not self.psoc_serial.isOpen():
            self.psoc_serial.open()
        with open(self.csv_file_name, 'w', newline='\n') as self.csv_file:
            print("Opening serial port and reading data from PSoC")
            while True:
                self.line_buffer = self.psoc_serial.readline()
                print(self.line_buffer)
                self.csv_file.write(self.line_buffer)


myTest = TestInterface()

myTest.start_logging_data()