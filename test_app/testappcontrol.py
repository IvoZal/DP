import tkinter as tk

class Device:
    def __init__(self, name):
        self.name = name
        self.run = True
        self.tk_var = tk.BooleanVar(value=self.run)
        self.result = "INIT"
        self.err_message = ""
        self.status_label = None

class TestAppController:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.selected_port = tk.StringVar(value="")
        self.test_devices = [Device("ATmega328P Xplained Mini"),
                             Device("Rele modul"),
                             Device("RTC a EEPROM modul"),
                             Device("Rotacni enkoder"),
                             Device("Maticova klavesnice"),
                             Device("Modul LCD displeje"),
                             Device("Reproduktor"),
                             Device("Modul s termistorem"),]
        
    def start_clicked(self):
        com_port = self.selected_port.get()
        try:
            self.model.init_serial(com_port, 38400)
            self.view.comm_label.config(text="COM port " + com_port + " selected.")
        except Exception as e:
            self.view.comm_label.config(text="Could not access selected port: " + com_port)
            pass
        else:
            for device in self.test_devices:
                match device.name:
                    case "ATmega328P Xplained Mini":
                        if(device.run):
                            test_result = self.model.self_test("TEST ATMEGA")
                            if(test_result[0]):
                                device.result = "PASS"
                            else:
                                device.result = "FAIL"
                                device.err_message = test_result[1]
                        else:
                            device.result = "SKIPPED"

                    case "Rele modul":
                        if(device.run):
                            test_result = self.model.self_test("TEST RELAY")
                            if(test_result[0]):
                                device.result = "PASS"
                            else:
                                device.result = "FAIL"
                                device.err_message = test_result[1]
                        else:
                            device.result = "SKIPPED"

                    case "RTC a EEPROM modul":
                        if(device.run):
                            test_result = self.model.self_test("TEST RTC")
                            if(test_result[0]):
                                device.result = "PASS"
                            else:
                                device.result = "FAIL"
                                device.err_message = test_result[1]
                        else:
                            device.result = "SKIPPED"

                    case "Rotacni enkoder":
                        if(device.run):
                            device.result = "FAIL"
                        else:
                            device.result = "SKIPPED"

                    case "Maticova klavesnice":
                        if(device.run):
                            device.result = "FAIL"
                        else:
                            device.result = "SKIPPED"

                    case "Modul LCD displeje":
                        if(device.run):
                            device.result = "FAIL"
                        else:
                            device.result = "SKIPPED"

                    case "Reproduktor":
                        if(device.run):
                            device.result = "INIT"
                            self.view.open_window(device)
                        else:
                            device.result = "SKIPPED"

                    case "Modul s termistorem":
                        if(device.run):
                            device.result = "FAIL"
                        else:
                            device.result = "SKIPPED"

                    case others:
                        pass
                    
                self.view.update_status_label(device)
            self.model.close_serial()
