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
// pointers to items in timetable.
pair *p_timetable[MAX_N];

// swap ith and jth items in `p_timetable`
void swap(int i, int j) {
    pair *t;

    if (i == j) {
        return;
    }

    t = p_timetable[i];
    p_timetable[i] = p_timetable[j];
    p_timetable[j] = t;
}


// check whether the given `window` is feasible or not
bool check_time_window(int n, double window, double start_time) {
    int i;
    bool feasible;
    double mininal_next_start_time, next_start_time;

    // the `window` is feasible if all items in timetable are visited
    if (n == 0) {
        return true;
    }

    // increase start time by `window` each time
    mininal_next_start_time = start_time + window;

    // traverse through the `timetable`. recursively check `window`
    for (i = 0; i < n; i++) {
        if (mininal_next_start_time <= p_timetable[i]->end) {
            // decide `next_start_time` greedily
            next_start_time = (
                mininal_next_start_time > p_timetable[i]->start ?
                mininal_next_start_time : p_timetable[i]->start);

            swap(i, n - 1);
            feasible = check_time_window(n - 1, window, next_start_time);
            swap(i, n - 1);

            if (feasible) {
                return true;
            }
        }
    }
    
    return false;
}


int main(void) {
    int i, j, n, case_index;
    int minute, second;
    bool feasible;
    double window, window_low, window_high, start_time;
    double min_start, max_end;

    // used in output to indicate the case number
    case_index = 1;

    while (scanf("%d", &n) != EOF) {
        // exit if n is zero
        if (n == 0) {
            break;
        }

        // initialize `timetable` and `p_timetable`
        // we will need the mininal start time and the maximal end time
        min_start = 1440;
        max_end = 0;

        for (i = 0; i < n; i++) {
            scanf("%lf%lf", &timetable[i].start, &timetable[i].end);
            p_timetable[i] = timetable + i;

            // update `min_start`
            if (timetable[i].start < min_start) {
                min_start = timetable[i].start;
            }

            // update `max_end`
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
                // note that `start_time` should be fetched out of two `swap`
                start_time = p_timetable[j]->start;

                swap(j, n - 1);
                feasible = check_time_window(n - 1, window, start_time);
                swap(j, n - 1);

                // if feasible, increase lower bound
                if (feasible) {
                    window_low = window;
                    break;
                }
            }

            // if not feasible, decrease upper bound
            if (!feasible) {
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
