import tkinter as tk
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

    def missing_input(self, message):
        self.err_label.config(text=self.err_label["text"] + message)

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
        canvas_widget.grid(column=0, row=3)

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
        match device.result:
            case "FAIL":
                color = 'red'

            case "SKIPPED":
                color = 'yellow'

            case others:
                color = 'green'
        
        device.status_label.config(text=device.result, bg=color)

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
                self.controller.serial_read_flag = False
                device.data = []

            case others:
                pass