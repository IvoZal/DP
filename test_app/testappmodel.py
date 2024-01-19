import serial

class TestModel:
    def __init__(self, com_port, baud_rate):
        self.ser = serial.Serial(com_port, baud_rate)
        
    def start_test(self, message, timeout):
        self.ser.timeout = timeout
        message += '\n'
        self.ser.write(message.encode('utf-8'))

    def eval_test(self):
        result = self.ser.readline().decode('utf-8')
        if(result.find("PASS") >= 0):
            return True, result
        else:
            return False, result
        
    def self_test(self, message, timeout=5):
        self.start_test(message,timeout)
        return self.eval_test()
