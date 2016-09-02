"""Script responsible for connecting to nvim and attaching it to a bridge."""
from nvim_bridge import UIBridge
from neovim import attach
from neovim.compat import IS_PYTHON3

def main(ctx, prog, notify, listen, connect, font, profile):
    """Entry point."""
    address = connect or listen

    if address:
        import re
        p = re.compile(r'^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}(?:\:\d{1,5})?$')

        if p.match(address):
            args = ('tcp',)
            kwargs = {'address': address}
        else:
            args = ('socket',)
            kwargs = {'path': address}

    def find_nvim_pipe_number():
        #Find nvim pipe number. Spawn nvim if it was not launched yet.
        import subprocess
        import os 
        spawned = False

        while True:
            pipes = subprocess.check_output("pipelist", universal_newlines=True)
            address_location = pipes.find("nvim")

            if address_location != -1:
                break

            if not spawned:
                #try to spawn nvim
                print("spawn nvim myself")
                dnull = open(os.devnull)
                subprocess.Popen("nvim", stdin=dnull, stdout=dnull, stderr=dnull)
                spawned = True

        import re
        #for now, we just know that nvim pipe number starts at address_location + 5
        #but we do not know the exact lenght: could be 3, could be 5
        #Assume 5 digits and actually get 3: "716-0" - need to use regex to seperate the
        #first actual pipe address
        return re.findall(r'\d+', pipes[address_location + 5:address_location + 10])[0]
                
        #return pipes[address_location + 5:address_location + 9]

    pipe = find_nvim_pipe_number()

    nvim = attach('socket', path='\\\\.\\pipe\\nvim-' + pipe + '-0')

    print(nvim.channel_id)
    
    from gui_interface import GtkUI
    ui = GtkUI(font)
    bridge = UIBridge()
    bridge.connect(nvim, ui)
    
from threading import Thread

t = Thread(target = main, daemon = True, args = (None, None, False, None, None, ('Monospace', 13), 'disable'))
t.start()
