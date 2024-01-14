import tkinter as tk

class TestAppController:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.test_devices = {"ATmega328P Xplained Mini" : True,
                    "Modul LCD displeje" : True,
                    "Rele modul" : True,
                    "Rotacni enkoder" : True,
                    "RTC a EEPROM modul" : True,
                    "Modul s termistorem" : True,
                    "Reproduktor" : True,
                    "Maticova klavesnice" : True}
        
    def start_clicked(self):
        print("Start")