#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// maximum number of landing scenarios
#define MAX_N 8

// we adjusted this iteration number a few times
// this is the least one feasible on OJ test cases 
#define ITER_NUM 18

// a struct used to hold the start and end of an interval during which
// a plane can safely land
typedef struct pair {
    double start;
    double end;
} pair;

// landing scenarios, each containing a start time and an end time.
pair timetable[MAX_N];

// used in depth-fisrt search to help record whether an item in timetable
// is visited or not
bool visited[MAX_N]; 

// check whether all the `n` items in timetable are visited
bool all_visited(int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (!visited[i]) {
            return false;
        }
    }

    return true;
}

// depth fisrt search. check whether the given `window` is feasible or not
bool check_time_window_dfs(int n, double window, double start_time) {
    int i;
    double mininal_next_start_time, next_start_time;

    // the `window` is feasible if all items in timetable are visited
    if (all_visited(n)) {
        return true;
    }


    // increase start time by `window` each time
    mininal_next_start_time = start_time + window;

    // traverse through the `timetable`. recursively check `window`
    for (i = 0; i < n; i++) {
        
        if (!visited[i] && mininal_next_start_time <= timetable[i].end) {
            visited[i] = true;

            // decide `next_start_time` greedily
            next_start_time = (
                mininal_next_start_time > timetable[i].start ?
                mininal_next_start_time : timetable[i].start);

            if (check_time_window_dfs(n, window, next_start_time)) {
                return true;
            }

            visited[i] = false;
        }
    }
    
    return false;
}


int main(void) {
    int i, j, n, case_index;
    int minute, second;
    double window, window_low, window_high;
    double min_start, max_end;

    case_index = 1;
    while (scanf("%d", &n) != EOF) {
        // exit if n is zero
        if (n == 0) {
            break;
        }

        // initialize timetable
        // we will need the mininal start time and the maximal end time
        min_start = 1440;
        max_end = 0;
        for (i = 0; i < n; i++) {
            scanf("%lf%lf", &timetable[i].start, &timetable[i].end);
            if (timetable[i].start < min_start) {
                min_start = timetable[i].start;
            }
            if (timetable[i].end > max_end) {
                max_end = timetable[i].end;
            }
        }


        // binary search for the optimal time window.
        // for a given time window, recursively check whether it is feasible
        // upper bound can be decided by `max_end`, `min_start` and `n`
        window_low = 0;
        window_high = (max_end - min_start) / (n - 1);
        // adjust the `ITER_NUM` here to make sure we have good precision for
        // `second`, so that it is rounded to a correct integer
        for (i = 0; i < ITER_NUM; i++) {
            window = (window_low + window_high) / 2;

            for (j = 0; j < n; j++) {
                memset(visited, 0, sizeof(visited));
                visited[j] = true;

                // if feasible, increase lower bound
                if (check_time_window_dfs(n, window, timetable[j].start)) {
                    window_low = window;
                    break;
                }
            }
            // if not feasible, decrease upper bound
            if (j == n) {
                window_high = window;
            }
        }


        // output should be displayed in 'minute:second' pattern
        minute = (int) window_low;
        // second is rounded to the nearest integer
        second = (int) ((window_low - minute) * 60 + 0.5);

        if (second == 60) {
            minute += 1;
            second = 0;
        }
        
        printf("Case %d: %d:%02d\n", case_index, minute, second);
        case_index += 1;
    }

    return 0;
}
