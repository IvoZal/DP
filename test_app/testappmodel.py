import serial

class TestModel:
    def __init__(self, com_port, baud_rate):
        self.ser = serial.Serial(com_port, baud_rate)
        
    def start_test(self, message, timeout):
        self.ser.timeout = timeout
        self.ser.write(message)

    def eval_test(self):
        result = self.ser.readline()
        if(result.find("PASS") > 0):
            return True
        else:
            return False, result
        
    def self_test(self, message, timeout=3):
        self.start_test(message,timeout)
        return self.eval_test()
