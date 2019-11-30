# depth fisrt search. check whether the given `window` is feasible or not
def check_time_window_dfs(timetable, window, start_time, visited):
    if all(visited):
        return True

    next_start_time = start_time + window
    for i in range(len(timetable)):
        if not visited[i] and next_start_time <= timetable[i][1]:
            visited[i] = True
            next_start_time = max(next_start_time, timetable[i][0])
            if check_time_window_dfs(timetable, window, next_start_time, visited):
                return True
            visited[i] = False
    else:
        return False


if __name__ == '__main__':
    case_index = 1
    while True:
        # some initializing work...
        n = int(input())
        if n == 0:
            break

        timetable = []
        for _ in range(n):
            input_ = input()
            start, end, *_ = input_.split(' ')
            timetable.append((float(start), float(end)))


        # binary search for the optimal time window
        window_low, window_high = 0., 1440.
        
        for _ in range(70): # explain '70' in presentation
            window = (window_low + window_high) / 2

            for i in range(n):
                visited = [False] * n
                visited[i] = True
                if check_time_window_dfs(timetable, window, timetable[i][0], visited):
                    feasible = True
                    break
            else:
                feasible = False

            if feasible:
                window_low = window
            else:
                window_high = window


        minute = int(window_low)
        second = round((window_low - minute) * 60)
        
        if second == 60:
            minute += 1
            second = 0

        print('Case {0}: {1}:{2:02d}'.format(case_index, minute, second))
        case_index += 1
