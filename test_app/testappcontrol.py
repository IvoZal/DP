import tkinter as tk

class Device:
    def __init__(self, name):
        self.name = name
        self.state = True
        self.tk_var = tk.BooleanVar(value=self.state)
        self.result = "FAIL"
        self.err_message = ""

class TestAppController:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.test_devices = [Device("ATmega328P Xplained Mini"),
                             Device("Rele modul"),
                             Device("RTC a EEPROM modul"),
                             Device("Rotacni enkoder"),
                             Device("Maticova klavesnice"),
                             Device("Modul LCD displeje"),
                             Device("Reproduktor"),
                             Device("Modul s termistorem"),]
        
    def start_clicked(self):
        print("Start")
        for device in self.test_devices:
            match device.name:
                case "ATmega328P Xplained Mini":
                    if(device.state):
                        test_result = self.model.self_test("TEST ATMEGA")
                        if(test_result[0]):
                            device.result = "PASS"
                        else:
                            device.result = "FAIL"
                            device.err_message = test_result[1]
                    else:
                        device.result = "SKIPPED"

                case "Rele modul":
                    if(device.state):
                        test_result = self.model.self_test("TEST RELAY")
                        if(test_result[0]):
                            device.result = "PASS"
                        else:
                            device.result = "FAIL"
                            device.err_message = test_result[1]
                    else:
                        device.result = "SKIPPED"

                case "RTC a EEPROM modul":
                    if(device.state):
                        test_result = self.model.self_test("TEST RTC")
                        if(test_result[0]):
                            device.result = "PASS"
                        else:
                            device.result = "FAIL"
                            device.err_message = test_result[1]
                    else:
                        device.result = "SKIPPED"

                case "Rotacni enkoder":
                    pass

                case "Maticova klavesnice":
                    pass

                case "Modul LCD displeje":
                    pass

                case "Reproduktor":
                    pass

                case "Modul s termistorem":
                    pass

                case others:
                    pass
