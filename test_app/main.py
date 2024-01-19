import tkinter as tk
from testappview import DefaultView
from testappcontrol import TestAppController
from testappmodel import TestModel

if __name__ == "__main__":
    root = tk.Tk()
    root.configure(pady=20,padx=20)
    root.title("Test application")

    com_port = 'COM4'
    model = TestModel(com_port,38400)

    control = TestAppController(model, None)

    view = DefaultView(root, control)
    control.view = view

    root.mainloop()

