import tkinter as tk

class TestAppController:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.test_devices = {"ATmega328P Xplained Mini" : True,
                             "Rele modul" : True,
                             "RTC a EEPROM modul" : True,
                             "Rotacni enkoder" : True,
                             "Maticova klavesnice" : True,
                             "Modul LCD displeje" : True,
                             "Reproduktor" : True,
                             "Modul s termistorem" : True,}
        
    def start_clicked(self):
        print("Start")