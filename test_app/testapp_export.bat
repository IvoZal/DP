pyinstaller --onefile main.py ^
    --icon=testbench.ico ^
    --add-binary=avrdude/avrdude.exe:. ^
    --add-data=avrdude/avrdude.pdb:. ^
    --add-data=avrdude/avrdude.conf:. ^
    --add-data=m328p_binary/ATmega328P_test_prog.hex:. ^
    --add-data=lcd_image.png:. ^
    --noconsole