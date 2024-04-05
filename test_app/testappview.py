import tkinter as tk
import csv
from PIL import ImageTk, Image
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation

class UserInteractView(tk.Toplevel):
    def __init__(self, parent, device):
        super().__init__(parent)
        self.device = device
        self.err_label = tk.Label(self, text="", font=("Arial",14))
        self.err_label.grid(column=0, row=1, sticky=tk.W, padx=5, pady=5)

        skip_btn = tk.Button(self, text="Ukončit test", font=("Arial",14), background='green', command=lambda: self.save_result(False))
        skip_btn.grid(column=0,row=2, sticky=tk.W, padx=5, pady=5)

    def encoder_view(self):
        self.title("Test rotacniho enkoderu")

        label = tk.Label(self, text="Otočte enkodérem oběma směry a stiskněte tlačítko v ose enkodéru", font=("Arial",14))
        label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def keyboard_view(self):
        self.title("Test maticové klávesnice")

        label = tk.Label(self, text="Postupně stiskněte všechna tlačítka maticové klávesnice", font=("Arial",14))
        label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def display_btn_view(self):
        self.title("Test tlačítek na modulu LCD displeje")

        label = tk.Label(self, text="Postupně stiskněte všechna tlačítka na modulu LCD displeje", font=("Arial",14))
        label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def missing_input(self, message):
        # self.err_label.config(text=self.err_label["text"] + message)
        self.err_label.config(text=message)

    def save_result(self, clicked_result):
        if(clicked_result):
            self.device.result = "PASS"
        else:
            self.device.result = "FAIL"
        self.destroy()

class UserEvalView(tk.Toplevel):
    def __init__(self, parent, device):
        super().__init__(parent)
        self.device = device
        self.plot = None
        self.axis = None
        self.canvas = None
        self.tk_image = None

        yes_btn = tk.Button(self, text="Ano", font=("Arial",14), background='green', command=lambda: self.save_result(True))
        yes_btn.grid(column=0,row=2, sticky=tk.W, padx=5, pady=5)

        no_btn = tk.Button(self, text="Ne", font=("Arial",14), background='red', command=lambda: self.save_result(False))
        no_btn.grid(column=0,row=3, sticky=tk.W, padx=5, pady=5)

    def reproductor_view(self):
        self.title("Test reproduktoru")

        comm_label = tk.Label(self, text="Potvrťe, zda slyšíte tón z reproduktoru:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def display_view(self):
        self.title("Test LCD displeje")

        image_path = "lcd_image.png"
        image = Image.open(image_path)
        self.tk_image = ImageTk.PhotoImage(image)

        image_label = tk.Label(self, image=self.tk_image)
        image_label.grid(column=0, row=1, sticky=tk.W, padx=5, pady=5)

        comm_label = tk.Label(self, text="Potvrťe, zda na displeji svítí všechny pixely a správně se zobrazuje následující obrazec:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

    def update_graph(self, frame):
        self.axis.clear()
        self.axis.plot(self.device.data, marker='o')
        self.axis.set_title("Graf výstupního napětí modulu:")
        self.axis.set_xlabel("Číslo vzorku []")
        self.axis.set_ylabel("Napětí [V]")
        self.axis.grid(True)
        data_range = range(len(self.device.data))
        self.axis.set_xticks(data_range)
        self.axis.set_xticklabels([str(x) for x in data_range])
        # self.axis.set_ylim([0,5])
        self.canvas.draw()

    def thermistor_view(self):
        self.title("Test modulu s termisorem")

        comm_label = tk.Label(self, text="Vyhodnoťte, zda termisor funguje korektně:", font=("Arial",14))
        comm_label.grid(column=0, row=0, sticky=tk.W, padx=5, pady=5)

        # Init matplot figure
        figure = Figure(figsize=(6,6), dpi=100)
        self.axis = figure.add_subplot(1, 1, 1)
        
        self.canvas = FigureCanvasTkAgg(figure, master=self)
        canvas_widget = self.canvas.get_tk_widget()
        canvas_widget.grid(column=0, row=4)

        self.plot = animation.FuncAnimation(figure, self.update_graph, blit=False, interval=300, cache_frame_data=False)
        self.animation_running = True

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
        self.window = None
        
        ## Common test setting
        settings_frame = tk.Frame(self.root, bd=3, relief=tk.GROOVE)
        settings_frame.grid(column=0, row=0, padx=10, pady=5, sticky=tk.W)

        setup_label = tk.Label(settings_frame, text="Nastavení:", font=("Arial",14))
        setup_label.grid(column=0,row=0, sticky=tk.W, padx=5, pady=5)

        self.comm_label = tk.Label(settings_frame, text="", font=("Arial",14))
        self.comm_label.grid(column=0, row=2, sticky=tk.W, padx=5, pady=0)

        start_btn = tk.Button(settings_frame, text="Start", font=("Arial",14), background='green', command=self.controller.start_clicked)
        start_btn.grid(column=0,row=3, sticky=tk.W, padx=5, pady=2)

        exit_btn = tk.Button(settings_frame, text="Ukončit", font=("Arial",14), background='red', command=lambda: self.root.quit())
        exit_btn.grid(column=0, row=4, sticky=tk.W, padx=5, pady=2)

        # Menu for com port selection
        menu_frame = tk.Frame(settings_frame)
        menu_frame.grid(column=0, row=1, sticky=tk.W, padx=5, pady=2)

        menu_btn = tk.Menubutton(menu_frame, text="Vyberte COM port", relief="raised", indicatoron=True, font=("Arial",14))
        menu_btn.grid(column=0, row=1)

        port_menu = tk.Menu(menu_btn, tearoff=1)
        menu_btn.config(menu=port_menu)
        for port in self.controller.model.com_list:
            port_menu.add_command(label=port, command=lambda port=port: self.controller.selected_port.set(port))

        # Test report log settings
        log_label = tk.Label(settings_frame, text="Nastavení ukládání protokolů o zkoušce:", font=("Arial",14))
        log_label.grid(column=1, row=1, sticky=tk.W, padx=50, pady=2)

        self.log_switch = tk.Checkbutton(settings_frame, text="Logování vypnuto", variable=self.controller.log_flag, font=("Arial",14), command=self.toggle, onvalue=1, offvalue=0)
        self.log_switch.grid(column=1, row=2, sticky=tk.W, padx=50, pady=0)

        log_name_label = tk.Label(settings_frame, text="Zadejte název souboru:", font=("Arial",14))
        log_name_label.grid(column=1 ,row=3, sticky=tk.W, padx=50, pady=0)

        self.log_entry = tk.Entry(settings_frame, font=("Arial",14))
        self.log_entry.grid(column=1 ,row=4, sticky=tk.W, padx=50, pady=0)
        self.log_entry.bind("<Return>", self.submit)

        log_btn = tk.Button(settings_frame, text="Vytvořit logovací soubor", font=("Arial",14), background='green', command=self.submit)
        log_btn.grid(column=1, row=5, sticky=tk.W, padx=50, pady=2)

        self.err_label = tk.Label(settings_frame, text="", font=("Arial",14))
        self.err_label.grid(column=2 ,row=5, sticky=tk.W)

        # Menu for m328p programming
        prog_frame = tk.Frame(self.root, bd=3, relief=tk.GROOVE)
        prog_frame.grid(column=0, row=5, padx=10, pady=5, sticky=tk.W)

        prog_btn = tk.Button(prog_frame, text="Nahrát program do ATmega328P Xplained Mini", font=("Arial",14), background='grey',command=self.controller.program_m328p)
        prog_btn.grid(column=0, row=5)

        self.prog_label = tk.Label(prog_frame, text="", font=("Arial",14))
        self.prog_label.grid(column=0, row=6, sticky=tk.W, padx=5, pady=5)

        # Device specific labels
        device_frame = tk.Frame(self.root, bd=3, relief=tk.GROOVE)
        device_frame.grid(column=0, row=7, padx=10, pady=5, sticky=tk.W)

        test_label = tk.Label(device_frame, text="Vyberte, ktere moduly testovat:", font=("Arial",14))
        test_label.grid(column=0,row=7, sticky=tk.W, padx=5, pady=5)

        result_label = tk.Label(device_frame, text="Vysledky testu:", font=("Arial",14))
        result_label.grid(column=1, row=7, sticky=tk.W, padx=50, pady=5)  

        result_label = tk.Label(device_frame, text="Chybová hláška:", font=("Arial",14))
        result_label.grid(column=2, row=7, sticky=tk.W, padx=50, pady=5)        

        # Checkboxes for each test with status label
        i = 8
        for device in self.controller.test_devices:
            check_btn = tk.Checkbutton(device_frame, text=device.name, variable=device.tk_var, font=("Arial", 14),
                                        command=lambda d=device: self.update_state(d))
            check_btn.grid(row=i, column=0, padx=5, pady=2, sticky=tk.W)
            
            status_label = tk.Label(device_frame, text=device.result, font=("Arial", 14), bg='green')
            status_label.grid(row=i, column=1, padx=50, pady=2, sticky=tk.W)
            device.label["status"] = status_label

            error_label = tk.Label(device_frame, text="", font=("Arial", 14))
            error_label.grid(row=i, column=2, padx=5, pady=2, sticky=tk.W)
            device.label["error"] = error_label

            i += 1

        sel_all_btn = tk.Button(device_frame, text="Vybrat vše", font=("Arial",14), background='grey', command=lambda d=self.controller.test_devices: self.select_all(d, True))
        sel_all_btn.grid(row=i, column=0, sticky=tk.W, padx=5, pady=2)
        i += 1

        sel_all_none = tk.Button(device_frame, text="Zrušit výběr všech", font=("Arial",14), background='grey', command=lambda d=self.controller.test_devices: self.select_all(d, False))
        sel_all_none.grid(row=i, column=0, sticky=tk.W, padx=5, pady=2)
        i += 1

    def toggle(self):
        if(self.controller.log_flag.get() == 1):
            self.log_switch.config(text="Logování zapnuto")
        else:
            self.log_switch.config(text="Logování vypnuto")

    def submit(self, event=None):
        try:
            self.controller.log_filename = f"{self.log_entry.get()}.csv"
            with open(self.controller.log_filename, 'w', newline='') as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow(["Kit number", "ATmega328P Xplained Mini", "Rele modul", 
                                 "RTC a EEPROM modul", "Rotacni enkoder", "Maticova klavesnice", 
                                 "Tlacitka na LCD modulu", "Modul LCD displeje", "Reproduktor", "Modul s termistorem"])
        except Exception as e:
            self.err_label.config(text="Chyba při vytváření souboru!")
        else:
            self.err_label.config(text="Soubor vytvořen")

    def get_kit_name(self):
        return tk.simpledialog.askstring("Číslo kitu","Zadejte číslo testovaného kitu:")

    def select_all(self, devices, sel):
        for device in devices:
            device.run = sel
            device.tk_var.set(sel)

    def update_state(self, device):
        device.run = device.tk_var.get()

    def update_prog_status(self, message):
        self.prog_label.config(text=message)

    def update_status_label(self, device):
        match device.result:
            case "FAIL":
                color = 'red'

            case "SKIPPED":
                color = 'yellow'

            case others:
                color = 'green'
        
        device.label["status"].config(text=device.result, bg=color)
        device.label["error"].config(text=device.err_message)

    def open_window(self, device):
        match device.name:
            case "Rotacni enkoder":
                self.window = UserInteractView(self.root, device)
                self.window.encoder_view()
                self.root.wait_window(self.window)

            case "Maticova klavesnice":
                self.window = UserInteractView(self.root, device)
                self.window.keyboard_view()
                self.root.wait_window(self.window)

            case "Tlacitka na LCD modulu":
                self.window = UserInteractView(self.root, device)
                self.window.display_btn_view()
                self.root.wait_window(self.window)

            case "Reproduktor":
                self.window = UserEvalView(self.root, device)
                self.window.reproductor_view()
                self.root.wait_window(self.window)

            case "Modul LCD displeje":
                self.window = UserEvalView(self.root, device)
                self.window.display_view()
                self.root.wait_window(self.window)

            case "Modul s termistorem":
                self.window = UserEvalView(self.root, device)
                self.window.thermistor_view()
                self.root.wait_window(self.window)
                device.data = []

            case others:
                pass