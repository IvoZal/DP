import tkinter as tk

class DefaultView:
    def __init__(self, root, controller):
        self.controller = controller
        self.check_vars = {}
        
        start_btn = tk.Button(root, text="Start", font=("Arial",14), background='green')
        start_btn.grid(column=1,row=0, sticky=tk.E, padx=5, pady=5)
        i = 0
        for device, initial_state in self.controller.test_devices.items():
            check_var = tk.BooleanVar(value=initial_state)
            self.check_vars[device] = check_var

            check_btn = tk.Checkbutton(root, text=device, variable=check_var, font=("Arial", 14),
                                        command=lambda d=device: self.update_state(d))
            check_btn.grid(row=i, column=0, padx=5, pady=5, sticky=tk.W)
            i += 1

    def update_state(self, device):
        self.controller.test_devices[device] = self.check_vars[device].get()
        print(f"State of {device}: {self.controller.test_devices[device]}")