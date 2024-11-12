#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int arrival_time;
    int priority;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

// First Come First Served (FIFO)
void fcfs(Process processes[], int n) {
    int current_time = 0;
    float total_turnaround_time = 0, total_waiting_time = 0;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > current_time){
            current_time = processes[i].arrival_time; 
        }
        processes[i].completion_time = current_time + processes[i].burst_time; 
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time; 
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time; 
        current_time = processes[i].completion_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;

        printf("Process %d Completion Time: %d\n", processes[i].id, processes[i].completion_time); 
    }

    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

// Shortest Job First
void sjf(Process processes[] , int n){
    int current_time = 0;
    int completed = 0;
    float total_turnaround_time = 0, total_waiting_time = 0;
    int is_completed[10] = {0}; // İşlemlerin tamamlanıp tamamlanmadığını takip etmek için bir dizi

    while (completed < n){
        int shortest_job = -1; 
        int shortest_burst = 9999; 

        for(int i = 0 ; i < n ; i++){
            if (processes[i].arrival_time <= current_time && processes[i].burst_time < shortest_burst 
                && !is_completed[i])  // Tamamlanmamış işlemleri kontrol et
            {
                shortest_job = i; 
                shortest_burst = processes[i].burst_time;
            }
        }
        
        if(shortest_job == -1){
            current_time++;
            continue;
        }
    
        processes[shortest_job].completion_time = current_time + processes[shortest_job].burst_time;
        processes[shortest_job].turnaround_time = processes[shortest_job].completion_time - processes[shortest_job].arrival_time;
        processes[shortest_job].waiting_time = processes[shortest_job].turnaround_time - processes[shortest_job].burst_time;
        
        current_time = processes[shortest_job].completion_time;
        is_completed[shortest_job] = 1; // İşlemi tamamlandı olarak işaretle
        completed++;
        
        total_turnaround_time += processes[shortest_job].turnaround_time;
        total_waiting_time += processes[shortest_job].waiting_time;

        printf("Process %d Completion Time: %d\n", processes[shortest_job].id, processes[shortest_job].completion_time); 
    }

    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

// Priority Scheduling
void prio(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    float total_turnaround_time = 0, total_waiting_time = 0;
    int is_completed[10] = {0};

    // İşlemlerin kalan sürelerini başlangıçta burst time olarak ayarla
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        int highest_priority = -1;
        int highest_priority_value = 9999;

        // En yüksek önceliğe sahip ve henüz tamamlanmamış işlemi bul
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].priority < highest_priority_value && !is_completed[i]) {
                highest_priority = i;
                highest_priority_value = processes[i].priority;
            }
        }

        // Eğer uygun bir işlem bulunamadıysa, şimdiki zamanı güncelle
        if (highest_priority == -1) {
            current_time++;
            continue;
        }

        // En yüksek öncelikli işlemi 1 zaman birimi boyunca çalıştır
        processes[highest_priority].remaining_time--;

        // İşlem tamamlandı mı kontrol et
        if (processes[highest_priority].remaining_time == 0) {
            processes[highest_priority].completion_time = current_time + 1;
            processes[highest_priority].turnaround_time = processes[highest_priority].completion_time - processes[highest_priority].arrival_time;
            processes[highest_priority].waiting_time = processes[highest_priority].turnaround_time - processes[highest_priority].burst_time;

            is_completed[highest_priority] = 1;
            completed++;

            total_turnaround_time += processes[highest_priority].turnaround_time;
            total_waiting_time += processes[highest_priority].waiting_time;

            printf("Process %d Completion Time: %d\n", processes[highest_priority].id, processes[highest_priority].completion_time);
        }

        current_time++;
    }

    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <FCFS|SJF|PRIO> <process.txt>\n", argv[0]);
        return 1;
    }

    char *algorithm = argv[1];
    char *filename = argv[2];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    Process processes[10];
    int n = 0;
    while (fscanf(fp, "%d %d %d %d", &processes[n].id, &processes[n].arrival_time, &processes[n].priority, &processes[n].burst_time) == 4) {
        n++;
        if (n >= 10) {
            fprintf(stderr, "Error: Maximum 10 processes supported.\n");
            return 1;
        }
    }

    fclose(fp);

    if (strcmp(algorithm, "FCFS") == 0) {
        fcfs(processes, n);
    } else if (strcmp(algorithm, "SJF") == 0) {
        sjf(processes, n);
    } else if (strcmp(algorithm, "PRIO") == 0) {
        prio(processes, n);
    } else {
        fprintf(stderr, "Invalid scheduling algorithm: %s\n", algorithm);
        return 1;
    }

    return 0;
}

