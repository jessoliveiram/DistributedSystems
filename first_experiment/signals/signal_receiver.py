import sys, os, signal, time

def busy_wating():
    
    while(True):
        print("Waiting")
        time.sleep(3)

def block_wating():
    while(True):
        print("Waiting")
        signal.pause()

def receive_hup(signalNumber, frame):
    print ('sighup..hup..up..p')
    return

def terminate_process(signalNumber, frame):
    print ('Terminating the process')
    sys.exit()

def receive_signal(signalNumber, frame):
    print('Received:', signalNumber)
    return

def register_signals():
    signal.signal(signal.SIGHUP, receive_hup)
    signal.signal(signal.SIGINT, receive_signal)
    signal.signal(signal.SIGUSR1, receive_signal)
    signal.signal(signal.SIGSEGV, receive_signal)
    signal.signal(signal.SIGUSR2, receive_signal)
    signal.signal(signal.SIGPIPE, receive_signal)
    signal.signal(signal.SIGALRM, receive_signal)
    signal.signal(signal.SIGQUIT, receive_signal)
    signal.signal(signal.SIGILL, receive_signal)
    signal.signal(signal.SIGTRAP, receive_signal)
    signal.signal(signal.SIGABRT, receive_signal)
    signal.signal(signal.SIGBUS, receive_signal)
    signal.signal(signal.SIGFPE, receive_signal)
    signal.signal(signal.SIGTERM, terminate_process)

if __name__ == '__main__':
    print(os.getpid())
    register_signals()   
    args = sys.argv

    if args[1] == "busy":
        busy_wating()
    elif args[1] == "block":
        block_wating()
    else:
        print('"block" for block wating or "busy" for busy waiting')
