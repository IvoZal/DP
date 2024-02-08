import serial
import serial.tools.list_ports as list_ports

class TestModel:
    def __init__(self):
        ports = list_ports.comports()
        self.com_list = []
        for port in ports:
            self.com_list.append(port.name)

    def init_serial(self, com_port, baud_rate):
        self.ser = serial.Serial(com_port, baud_rate)

    def close_serial(self):
        self.ser.close()
        
    def write(self, message):
        message += '\n'
        self.ser.write(message.encode('utf-8'))

    def eval_test(self, timeout):
        self.ser.timeout = timeout
        result = self.ser.readline().decode('utf-8')
        if(result.find("PASS") >= 0):
            return True, result
        else:
            return False, result
        
    def self_test(self, message, timeout=5):
        self.write(message)
        return self.eval_test(timeout)

    def read_data(self, timeout):
        self.ser.timeout = timeout
        return self.ser.readline().decode('utf-8')