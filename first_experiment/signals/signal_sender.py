import os
import signal
import sys

valid_signals = list(signal.valid_signals())


def send_signal(pid, signal_num):
    received_signal = valid_signals[signal_num]
    os.kill(pid, received_signal)
    print(pid, received_signal)


def verify_process(process_id):
    return True


def verify_signal(received_signal):
    return len(valid_signals) > received_signal >= 0


def main(args):
    if len(args) == 1:
        print("First arg is PID to receive signal")
        print("Second arg is a number of an available signal:")
        n = 1
        for valid_signal in valid_signals:
            print(n, " : ", valid_signal)
            n += 1
        exit()

    pid = int(args[1])
    received_signal = int(args[2]) - 1
    if verify_process(pid) and verify_signal(received_signal):
        send_signal(pid, received_signal)


if __name__ == '__main__':
    main(sys.argv)
