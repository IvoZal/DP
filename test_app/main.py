import tkinter as tk
from testappview import DefaultView
from testappcontrol import TestAppController

if __name__ == "__main__":
    root = tk.Tk()
    root.configure(pady=20,padx=20)
    root.title("Test application")

    control = TestAppController(None, None)

    view = DefaultView(root, control)
    control.view = view

    root.mainloop()

