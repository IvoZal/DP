import tkinter as tk

class Device:
    def __init__(self, name):
        self.name = name
        self.state = True
        self.tk_var = tk.BooleanVar(value=self.state)
        self.result = "INIT"
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
                    if(device.state):
                        device.result = "FAIL"
                    else:
                        device.result = "SKIPPED"

                case "Maticova klavesnice":
                    if(device.state):
                        device.result = "FAIL"
                    else:
                        device.result = "SKIPPED"

                case "Modul LCD displeje":
                    if(device.state):
                        device.result = "FAIL"
                    else:
                        device.result = "SKIPPED"

                case "Reproduktor":
                    if(device.state):
                        device.result = "FAIL"
                    else:
                        device.result = "SKIPPED"

                case "Modul s termistorem":
                    if(device.state):
                        device.result = "FAIL"
                    else:
                        device.result = "SKIPPED"

                case others:
                    pass
