import gdb

class MyNotStoppingBreakpoint(gdb.Breakpoint):
    def stop(self):
        #print(gdb.parse_and_eval("count"), end="\r")
        return False


MyNotStoppingBreakpoint("function")
