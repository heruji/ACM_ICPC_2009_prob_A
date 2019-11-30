# recursively check whether the given `min_gap` is feasible or not
def check_min_time_gap(n, min_gap, landing_time, timetable):
    if n == 0:
        return True

    min_next_landing_time = landing_time + min_gap

    for i in range(n):
        if min_next_landing_time <= timetable[i][1]:
            next_landing_time = max(min_next_landing_time, timetable[i][0])

            timetable[i], timetable[n - 1] = timetable[n - 1], timetable[i]
            feasible = check_min_time_gap(n - 1, min_gap, next_landing_time, timetable)
            timetable[i], timetable[n - 1] = timetable[n - 1], timetable[i]

            if feasible:
                return True
    else:
        return False


if __name__ == '__main__':
    max_n = 8
    max_end_time = 1440.
    iter_num = 18

    timetable = [None] * max_n
    case_index = 1

    while True:
        # some initializing work...
        n = int(input())
        if n == 0:
            break

        min_start = max_end_time
        max_end = 0.
        for i in range(n):
            input_ = input()
            start, end = input_.split(' ')
            timetable[i] = (float(start), float(end))

            if timetable[i][0] < min_start:
                min_start = timetable[i][0]

            if timetable[i][1] > max_end:
                max_end = timetable[i][1]


        # binary search for the optimal min time gap
        lower_bound, upper_bound = 0., (max_end - min_start) / (n - 1)
        
        for _ in range(iter_num):
            mid = (lower_bound + upper_bound) / 2

            for i in range(n):
                landing_time = timetable[i][0]

                timetable[i], timetable[n - 1] = timetable[n - 1], timetable[i]
                feasible = check_min_time_gap(n - 1, mid, landing_time, timetable)
                timetable[i], timetable[n - 1] = timetable[n - 1], timetable[i]

                if feasible:
                    lower_bound = mid
                    break
            else:
                upper_bound = mid


        minute = int(lower_bound)
        second = round((lower_bound - minute) * 60)
        
        if second == 60:
            minute += 1
            second = 0

        print('Case {0}: {1}:{2:02d}'.format(case_index, minute, second))
        case_index += 1
