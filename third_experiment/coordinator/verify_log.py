LOG_PATH = "log.txt"
LOG_SEPARATOR= "|"

log_list = []
releases_order = []
requests_order = []

#verify if the release message cames after a grant message of the same pid
def verify_release_after_grant():
    previous_grant = dict
    for log in log_list:
        if log.get("message_type") == "GRANT":
            previous_grant = log
        if log.get("message_type") == "RELEASE":    
            if previous_grant.get("pid") != log.get("pid"):
               print("FAIL: RELEASE after GRANT") 
               print("RELEASE must be after a GRANT of the same PID")

    print("PASS: RELEASE after GRANT") 

#verify if the releases and requests pid are in the same order
def verify_process_order():
    if len(releases_order) != len(requests_order):
        print("FAIL: REQUESTS and RELEASES order")
        print("The numbers of requests and releases aren't the same")
    for i in range(len(requests_order)):
        if requests_order[i] != releases_order[i]:
            print("FAIL: REQUESTS and RELEASES order")
            print("Requests and releases aren't in the same order")
    print("PASS: REQUESTS and RELEASES order")

#split the separator in log line and returns it in a dict
def split_message(line):
    dict_log_line = dict()
    splitted_line = line.split(LOG_SEPARATOR)
    dict_log_line["timestamp"] = splitted_line[0]
    dict_log_line["message_type"] = splitted_line[1]
    dict_log_line["pid"] = splitted_line[2]
    return dict_log_line

#read the log file, build the support lists and run the validations
if __name__=='__main__':
    log_file = open(LOG_PATH, "r")
    for line in log_file:
        dict_log_line = split_message(line)
        log_list.append(dict_log_line)
        if dict_log_line.get("message_type") == "REQUEST":
            requests_order.append(dict_log_line.get("pid"))
        if dict_log_line.get("message_type") == "RELEASE":
            releases_order.append(dict_log_line.get("pid"))
    verify_process_order()
    verify_release_after_grant()