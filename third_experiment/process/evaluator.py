def evaluator(n, repeat):
    time_old = 0
    with open('result.txt') as f:
        lines = f.readlines()
    count_process = dict()
    count = 0
    for line in lines:
        print(f'line {count}: {line}')
        count += 1
        line_split = line.split("|")
        time_new = line_split[1].split(":")[1]
        if not verify_time(time_old, time_new):
            print("Error")
            return False
        time_old = time_new
        pid = line_split[0].split(":")[1]
        if pid not in count_process.keys():
            count_process[pid] = 1
        else:
            count_process[pid] = count_process[pid] + 1
    if not verify_lines(n, repeat, count):
        print("Error")
        return False
    if not verify_pid(count_process, repeat):
        print("Error")
        return False
    return True


def verify_time(time_old, time_new):
    if int(time_old) < int(time_new):
        return True
    return False


def verify_lines(n, repeat, count):
    if n*repeat == count:
        return True
    return False


def verify_pid(count_process, repeat):
    for k in count_process.keys():
        if not count_process[k] == repeat:
            return False
    return True


def clean_txt():
    open("result.txt", "w").close()


def verify_result(n, repeat):
    ok = evaluator(1, 7)
    if ok:
        print("Sucesso!")
    clean_txt()
