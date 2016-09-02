from __future__ import print_function, division
import math

import ui
import messages_from_ui 

class GtkUI(object):

    """Gtk+ UI class."""

    def __init__(self, font):
        """Initialize the UI instance."""
        pass

    def start(self, bridge):
        """Start the UI event loop."""
        bridge.attach(80, 24, True)
        self._bridge = bridge

        def register_poller(method, period):
            import time
            from threading import Thread
            def poller():
                while True:
                    time.sleep(period)
                    method()
            t = Thread(target = poller, daemon = True)
            t.start()

        def input_poll():
            inp = messages_from_ui.check_for_user_input()
            if inp != '':
                self._bridge.input(inp)

        def resize_poll():
            new_size = messages_from_ui.check_for_resize()
            '''expects (cols, rows) format'''
            if new_size[0] != -1:
                self._bridge.resize(new_size[0], new_size[1])

        def quit_poll():
            need_to_quit = messages_from_ui.check_for_quit()
            if need_to_quit == 1:
                print("calling self._bridge.exit()")
                self._bridge.exit()

        register_poller(input_poll, 0.001)
        register_poller(resize_poll, 0.1)
        register_poller(quit_poll, 0.1)

    def quit(self):
        ui.quit()

    def switch_to_navigator(self):
        ui.switch_to_navigator()

    def _nvim_resize(self, columns, rows):
        '''expects (rows, columns) format'''
        ui.resize(rows, columns)

    def _nvim_clear(self):
        ui.clear()

    def _nvim_lock_update_mutex(self):
        ui.lock_update_mutex()

    def _nvim_unlock_update_mutex(self):
        ui.unlock_update_mutex()

    def _nvim_redraw(self):
        ui.schedule_redraw()

    def _nvim_eol_clear(self):
        ui.eol_clear()

    def _nvim_cursor_goto(self, row, col):
        ui.cursor_goto(row, col)

    def _nvim_busy_start(self):
        pass

    def _nvim_busy_stop(self):
        pass

    def _nvim_mouse_on(self):
        pass

    def _nvim_mouse_off(self):
        pass

    def _nvim_mode_change(self, mode):
        if mode == 'normal':
            ui.change_mode(0)
        if mode == 'insert':
            ui.change_mode(1)

    def _nvim_set_scroll_region(self, top, bot, left, right):
        ui.set_scroll_region(top, bot, left, right)

    def _nvim_scroll(self, count):
        ui.scroll(count)

    def _nvim_highlight_set(self, attrs):
        if attrs == {}:
            '''reset highlight set'''
            ui.highlight_set(-1, 0)
            return
        
        attributeIDs = {'bold' : 0, 
                        'underline' : 1,
                        'undercurl' : 2,
                        'italic' : 3,
                        'reverse' : 4,
                        'foreground' : 5,
                        'background' : 6,
                        'special' : 7}
        
        for attr, value in attrs.items():
            if (type(value) == bool):
                if (value == True):
                    cvalue = 1
                else:
                    cvalue = 0
                ui.highlight_set(attributeIDs[attr], cvalue)
                #RPC(5, intToBytes(attributeIDs[attr]) + intToBytes(cvalue))
            else:
                ui.highlight_set(attributeIDs[attr], value)
                #RPC(5, intToBytes(attributeIDs[attr]) + intToBytes(value))

    def _nvim_put(self, text):
        ui.put(text)

    def _nvim_bell(self):
        pass

    def _nvim_visual_bell(self):
        pass

    def _nvim_update_fg(self, fg):
        '''
        fg could be negative, but update_fg receives an unsigned int.
        The resulting conversion is what is apparently expected
        by nvim, so simply passing should work.
        '''
        if fg < 0: fg = 0
        ui.update_fg(fg)

    def _nvim_update_bg(self, bg):
        #bg could be negative
        if bg < 0: bg = 0xFFFFFF
        ui.update_bg(bg)

    def _nvim_suspend(self):
        pass

    def _nvim_set_title(self, title):
        ui.set_title(title)

    def _nvim_set_icon(self, icon):
        pass
