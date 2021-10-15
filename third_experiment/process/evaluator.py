def evaluator(n, repeat):
    time_old = 0
    with open('result.txt') as f:
        lines = f.readlines()
    # count_process = {} guardar pid e incrementar contagem
    count = 0
    for line in lines:
        print(f'line {count}: {line}')
        count += 1
        line_split = line.split("|")
        time_new = line_split[1].split(":")[1]
        if not verify_time(time_old, time_new):
            return False
        time_old = time_new
        pid = line_split[0].split(":")[1]
        # count pid
        print(pid + " " + time_new)
    if not verify_lines(n, repeat, count):
        return False
    # if not verify_pid(count_process):
    #     return False
    return True


def verify_time(time_old, time_new):
    if int(time_old) < int(time_new):
        return True
    return False


def verify_lines(n, repeat, count):
    if n*repeat == count:
        return True
    return False

#
# def verify_pid(count_process):
#


def clean_txt():
    return True


if __name__ == '__main__':
    # ler argumentos do console
    ok = evaluator(1, 7)
    if ok:
        print("Sucesso!")
    clean_txt()
