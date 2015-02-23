__author__ = 'Geir Istad'

import serial
import csv
import sys
import time


class TestInterface:
    psoc_serial = serial.Serial()
    input_csv_file_name = './../WaypointSystem/TestData/nmeacsvtestD.csv'
    output_csv_file_name = './../WaypointSystem/TestData/psocnmeacsvtestD.csv'
    line_buffer = ""
    serialPort = 7
    echo_to_console = False

    def __init__(self):
        """
        Add an init method at a later time
        :return:
        """
        """self.psoc_serial.setBaudrate(9600)
        self.psoc_serial.setByteSize(8)
        self.psoc_serial.setParity('N')
        """
        self.psoc_serial.port = 7
        print(self.psoc_serial)

    def set_file_name(self, filename):
        self.output_csv_file_name = filename

    def start_serial(self):
        if not self.psoc_serial.isOpen():
            self.psoc_serial.open()
        if self.psoc_serial.isOpen():
            print("Opened serial port " + self.psoc_serial.portstr)
        else:
            sys.exit("Unable to open serial port!")

    def start_logging_data(self):

        # Start the serial connection
        self.start_serial()

        time.sleep(2)

        # Load the input csv
        input_csv_file = open(self.input_csv_file_name, 'r', newline='\r\n')
        input_csv = csv.reader(input_csv_file)

        # Load the output csv file, note that since we are receiving normal strings the output will
        # be treated as a normal file and we just write the strings as-received to it.
        output_csv = open(self.output_csv_file_name, 'w', newline='\n')

        line_ending = '\r\n'

        # Cycle through the lines, send them over serial, wait for response and store response
        for row in input_csv:

            # Convert each row into a \r\n terminated byte string
            if self.echo_to_console:
                print("Read row: ", row)
            self.line_buffer = (','.join(row))
            self.line_buffer += line_ending
            self.line_buffer = self.line_buffer.encode()

            # Send each generated string
            if self.echo_to_console:
                print("Sending: ", self.line_buffer)
            self.psoc_serial.write(self.line_buffer)

            # Wait for echo of string
            self.line_buffer = self.psoc_serial.readline()
            if self.echo_to_console:
                print("Received (byte): ", self.line_buffer)

            # Convert string to utf-8
            self.line_buffer = self.line_buffer.decode("utf-8")
            if self.echo_to_console:
                print("Received (utf-8): ", self.line_buffer)

            # Write each converted string to output file in csv format
            if self.echo_to_console:
                print(self.line_buffer)
            output_csv.write(self.line_buffer)

        # Close the files used
        input_csv_file.close()
        output_csv.close()

myTest = TestInterface()

myTest.start_logging_data()