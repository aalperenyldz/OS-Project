#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  int arrival_time;
  int priority;
  int burst_time;
  int completion_time;
  int turnaround_time;
  int waiting_time;
} Process;

//First Come First Served(FIFO)
void fcfs(Process processes[], int n) {
  int current_time = 0;
  float total_turnaround_time = 0, total_waiting_time = 0;

  for (int i = 0; i < n; i++) {
    if (processes[i].arrival_time > current_time){
      current_time = processes[i].arrival_time; //geçerli işlemde varış zamanı büyük mü ona bak => büyükse current = işlemin varışına
    }
    processes[i].completion_time = current_time + processes[i].burst_time; //tamamlanma zamanı hesaplıyoruz
    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time; //dönüş süresi
    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time; //bekleme süresi
    current_time = processes[i].completion_time;
    total_turnaround_time += processes[i].turnaround_time;
    total_waiting_time += processes[i].waiting_time;
  }

  printf("Ortalama Dönüş Süresi: %.2f\n", total_turnaround_time / n);
  printf("Ortalama Bekleme Süresi: %.2f\n", total_waiting_time / n);
}

//Shortest Job First
void sjf(Process processes[] , int n){
    int current_time = 0;
    int completed = 0;
    float total_turnaround_time = 0, total_waiting_time = 0;

    while (completed < n){
        int shortest_job = -1; //en kısa işlem süresine sahip olanın indeksini tutucak
        int shortest_burst = 9999; //işlem süresi tutar

        // en kısa işlem süresine sahip ve henüz tamamlanmamış işlemi bul
        for(int i = 0 ; i < n ; i++){
            if (processes[i].arrival_time <= current_time && processes[i].burst_time < shortest_burst 
            && processes[i].completion_time == 0) //işlemin kısa veyahut en kısa işlem olup olmadığı kontrolü
            {
                shortest_job = i; 
                shortest_burst = processes[i].burst_time;
        }
    }
        
        // eğer uygun bir işlem bulunamadıysa, şimdiki zamanı güncelle
        if(shortest_job == -1){
            current_time++;
            continue;
    }
    
    // işlemin tamamlanma zamanını, dönüş süresini ve bekleme süresini hesapla
    processes[shortest_job].completion_time = current_time + processes[shortest_job].burst_time;
    processes[shortest_job].turnaround_time = processes[shortest_job].completion_time - processes[shortest_job].arrival_time;
    processes[shortest_job].waiting_time = processes[shortest_job].turnaround_time - processes[shortest_job].burst_time;
    
    // şimdiki zamanı ve tamamlanan işlem sayısını güncelle   
    current_time = processes[shortest_job].completion_time;
    completed++;
    
    // toplam dönüş süresini ve bekleme süresini güncelle    
    total_turnaround_time += processes[shortest_job].turnaround_time;
    total_waiting_time += processes[shortest_job].waiting_time;
    }

    printf("Ortalama Dönüş Süresi: %.2f\n", total_turnaround_time / n);
    printf("Ortalama Bekleme Süresi: %.2f\n", total_waiting_time / n);
}



