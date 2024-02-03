import tkinter as tk
import threading

class Device:
    def __init__(self, name):
        self.name = name
        self.run = True
        self.tk_var = tk.BooleanVar(value=self.run)
        self.result = "INIT"
        self.err_message = ""
        self.status_label = None
        self.data = []

class TestAppController:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.selected_port = tk.StringVar(value="")
        self.serial_read_flag = False
        self.test_devices = [Device("ATmega328P Xplained Mini"),
                             Device("Rele modul"),
                             Device("RTC a EEPROM modul"),
                             Device("Rotacni enkoder"),
                             Device("Maticova klavesnice"),
                             Device("Modul LCD displeje"),
                             Device("Reproduktor"),
                             Device("Modul s termistorem"),]
        
    def start_adc_read(self, device):
        while self.serial_read_flag:
            try:
                data = self.model.read_data(None)
                raw_data = int(data.replace("ADC: ",""))
                device.data.append(5*raw_data/1023)
                if len(device.data) > 20:
                    device.data.pop(0)
            except ValueError:
                pass

    def interact_test(self, device):
        test_result = [False, ""]
        while self.serial_read_flag:
            try:
                test_result = self.model.eval_test(0)
            except:
                pass
            else:
                if(test_result[0]):
                    device.result = "PASS"
                    device.err_message = ""
                    self.view.window.save_result(True)
                    # break
                elif(test_result[1] != ""):
                    device.result = "FAIL"
                    device.err_message += test_result[1]
                    self.view.window.missing_input(device.err_message)
        
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
                            self.model.start_test("TEST ENCODER")

                            self.serial_read_flag = True
                            test_thread = threading.Thread(target=self.interact_test, args=(device,))
                            test_thread.start()

                            self.view.open_window(device)        
                            self.serial_read_flag = False                    
                        else:
                            device.result = "SKIPPED"

                    case "Maticova klavesnice":
                        if(device.run):
                            self.model.start_test("TEST KEYBRD")

                            self.serial_read_flag = True
                            test_thread = threading.Thread(target=self.interact_test, args=(device,))
                            test_thread.start()

                            self.view.open_window(device)        
                            self.serial_read_flag = False  
                        else:
                            device.result = "SKIPPED"

                    case "Modul LCD displeje":
                        if(device.run):
                            self.model.start_test("TEST LCD")
                            self.view.open_window(device)
                        else:
                            device.result = "SKIPPED"

                    case "Reproduktor":
                        if(device.run):
                            self.model.start_test("TEST REPRO")
                            self.view.open_window(device)
                        else:
                            device.result = "SKIPPED"

                    case "Modul s termistorem":
                        if(device.run):
                            self.model.start_test("TEST THERM")

                            self.serial_read_flag = True
                            serial_thread = threading.Thread(target=self.start_adc_read, args=(device,))
                            serial_thread.start()

                            self.view.open_window(device)                      
                        else:
                            device.result = "SKIPPED"

                    case others:
                        pass
                    
                self.view.update_status_label(device)
                self.model.ser.reset_input_buffer()
            self.model.close_serial()
