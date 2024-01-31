import tkinter as tk

class UserEvalView(tk.Toplevel):
    def __init__(self, parent, device):
        super().__init__(parent)
        self.device = device

        yes_btn = tk.Button(self, text="Ano", font=("Arial",14), background='green', command=lambda: self.save_result(True))
        yes_btn.grid(column=0,row=1, sticky=tk.W, padx=5, pady=5)

        no_btn = tk.Button(self, text="Ne", font=("Arial",14), background='red', command=lambda: self.save_result(False))
        no_btn.grid(column=0,row=2, sticky=tk.W, padx=5, pady=5)

    def reproductor_view(self):
        self.title("Test reproduktoru")

        comm_label = tk.Label(self, text="Potvrťe, zda slyšíte tón z reproduktoru:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def display_view(self):
        self.title("Test LCD displeje")

        comm_label = tk.Label(self, text="Potvrťe, zda je na displeji správně zobrazený testovací obrazec:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def thermistor_view(self):
        self.title("Test modulu s termisorem")

        comm_label = tk.Label(self, text="Vyhodnoťte, zda termisor funguje korektně:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def save_result(self, clicked_result):
        if(clicked_result):
            self.device.result = "PASS"
        else:
            self.device.result = "FAIL"
        self.destroy()

class DefaultView:
    def __init__(self, root, controller):
        self.root = root
        self.controller = controller
        
        # Menu for com port selection
        menu_frame = tk.Frame(self.root)
        menu_frame.grid(column=2, row=2)

        menu_btn = tk.Menubutton(menu_frame, text="Select COM port", relief="raised", indicatoron=True)
        menu_btn.grid(column=2, row=2)

        port_menu = tk.Menu(menu_btn, tearoff=1)
        menu_btn.config(menu=port_menu)
        for port in self.controller.model.com_list:
            port_menu.add_command(label=port, command=lambda port=port: self.controller.selected_port.set(port))

        # Common labels
        test_label = tk.Label(self.root, text="Vyberte, ktere moduly testovat:", font=("Arial",14))
        test_label.grid(column=0,row=0, sticky=tk.W, padx=5, pady=5)

        result_label = tk.Label(self.root, text="Vysledky testu:", font=("Arial",14))
        result_label.grid(column=1, row=0, sticky=tk.W, padx=50, pady=5)

        start_btn = tk.Button(self.root, text="Start", font=("Arial",14), background='green', command=self.controller.start_clicked)
        start_btn.grid(column=2,row=0, sticky=tk.E, padx=5, pady=5)

        exit_btn = tk.Button(self.root, text="Exit", font=("Arial",14), background='red', command=lambda: self.root.quit())
        exit_btn.grid(column=2, row=1, sticky=tk.E, padx=5, pady=5)

        self.comm_label = tk.Label(self.root, text="")
        self.comm_label.grid(column=2, row=3, sticky=tk.E, padx=5, pady=5)

        # Checkboxes for each test with status label
        i = 1
        for device in self.controller.test_devices:
            check_btn = tk.Checkbutton(self.root, text=device.name, variable=device.tk_var, font=("Arial", 14),
                                        command=lambda d=device: self.update_state(d))
            check_btn.grid(row=i, column=0, padx=5, pady=5, sticky=tk.W)
            
            status_label = tk.Label(self.root, text=device.result, font=("Arial", 14), bg='green')
            status_label.grid(row=i, column=1, padx=50, pady=5, sticky=tk.W)
            device.status_label = status_label

            i += 1

    def update_state(self, device):
        device.run = device.tk_var.get()

    def update_status_label(self, device):
        color = 'green'
        match device.result:
            case "FAIL":
                color = 'red'

            case "SKIPPED":
                color = 'yellow'

            case others:
                pass
        
        device.status_label.config(text=device.result, bg=color)

    def open_window(self, device):
        window = UserEvalView(self.root, device)
        match device.name:
            case "Reproduktor":
                window.reproductor_view()

            case "Modul LCD displeje":
                window.display_view()

            case "Modul s termistorem":
                window.thermistor_view()

            case others:
                pass
        self.root.wait_window(window)