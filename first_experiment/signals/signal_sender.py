import sys, os, signal

valid_signals = list(signal.valid_signals())

def send_signal(pid, signalnum):
    signal = valid_signals[signalnum]
    os.kill(pid, signal)
    print(pid, signal)

def verify_process(process_id):
    return True

def verify_signal(signal):
    return signal < len(valid_signals) and signal >= 0

def main(args):
    if len(args) == 1:
        print("First arg is PID to receive signal")
        print("Second arg is a number of an avaiable signal:")
        n = 1
        for signal in valid_signals:
            print(n, " : ", signal)
            n+=1
        exit()

    pid = int(args[1])
    signal = int(args[2]) -1
    if verify_process(pid) and verify_signal(signal):
        send_signal(pid, signal)

if __name__ == '__main__':
    main(sys.argv)