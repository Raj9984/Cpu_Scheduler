#include <iostream>
#include <queue>
#include <utility>
#include <string>
#include <vector>
#include <algorithm> // for all_of, count_if
#include <numeric>  // for accumulate
#include <cmath>
#include <limits>
#include <map>
#include <fstream>
using namespace std;

struct Process{
    string pid; // prcess id
    int bt; // burst time
    int at; // arrival time
    int priority; // priority of the process
    int wt; // wait time
    int tat; // turn around time
    Process(){
      pid = ""; bt = 0; at =0; priority =0; wt =0; tat =0;
    }
};

bool priorityVary(vector<Process> &processes){
   for(int i=0; i<processes.size(); i++){
    if(processes[i].priority!=processes[0].priority) return true;
   }
return false;
}

string predictAlgo(vector<Process> &processes,int variance_threshold_high, int variance_threshold_low,double &vbt, double &vat, double &abt){
   double tbt = accumulate(processes.begin(),processes.end(),0.0,[](double s, Process&p){return s + p.bt;});  // tbt => total burst time 
    abt = tbt / processes.size(); // average burst time 

   bool priority_vary = priorityVary(processes);
   
    vbt =0; // variance of all the burst time
   for(int i=0; i<processes.size(); i++){
    vbt += (processes[i].bt-abt)*(processes[i].bt-abt);
   }
   vbt = vbt/processes.size();
   
   double total_at = accumulate(processes.begin(),processes.end(),0.0,[](double s, const Process& p){return s+p.at;});
   double a_at = total_at/processes.size();

    vat =0;
   for(int i=0; i<processes.size(); i++){
    vat += (processes[i].at - a_at)*(processes[i].at - a_at);
   }
   vat = vat/processes.size();

  if(priority_vary) return "Priority_scheduling_preemptive";
  else if(vat>variance_threshold_high){
      if(vbt>variance_threshold_high)
        return "srtf";
      else return "fcfs";
  }
  else if(vbt>variance_threshold_high){
     return "sjf";
  }
  else if(vbt<variance_threshold_low&&abt>variance_threshold_high){
    return "ljfs";
  } 
  else return "round_robin";

}

// Scheduling algorithms
void fcfs(vector<Process> &processes,double vbt, double vat, double abt){
    // fcfs is a non-preemptive algorithm, all the processes are in the order of their arrival time, awt, art, atat|| *we need => awt, atat, art;
   int cpu_time = 0;
   double atat=0,awt=0,art=0;
   for(int i=0; i<processes.size(); i++){
      if(processes[i].at>cpu_time) cpu_time = processes[i].at;
      atat += double(cpu_time + processes[i].bt - processes[i].at);   // a-c;
      awt += double(cpu_time - processes[i].at);
      art += double(cpu_time - processes[i].at);
      cpu_time = cpu_time + processes[i].bt;
   }
   int n = processes.size();
   awt = awt/(double(n));
   atat = atat/(double(n));
   art = art/(double(n));
   fstream myfile;
   myfile.open("output.txt",ios::out);
   if(myfile.is_open()){
    myfile<<"FCFS is suitable for a fair approach when arrival times vary widely."<<endl;
    myfile<<"variance arrival time "<<to_string(vat)<<endl;
    myfile<<"variance burst time "<<to_string(vbt)<<endl;
    myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
    myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
    myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
    myfile<<endl;
    myfile <<"Order of process execution : "<<endl;
    for(int i=0; i<processes.size(); i++){
     myfile<<processes[i].pid<<" ";
    }
      myfile.close();
   }
}
/*sjf can be preemptive or non preemptive, in preemptive after every time quantum the algo check for the right candidate but in non-preemptive it check for the right candidate only after the completion of the running process so the running process does not get intrrupted in between*/
void sjf(vector<Process> &processes,double vbt, double vat, double abt){
  // sjf is a non-preemptive algo in general 
  vector<string> order;
  int cpu_time =processes[0].at,i=0,n = processes.size();
  double atat =0,awt=0,art=0;
  priority_queue<pair<int,Process*>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> ready_queue;
  Process* temp = &processes[i];
  ready_queue.push({processes[i].bt,temp});
  i++;
  while(!ready_queue.empty()){
    // take all the processes in the ready queue, which have arrival time less than or equal to the cpu time 
    while(i<n&&processes[i].at<=cpu_time){
       temp = &processes[i];
       ready_queue.push({processes[i].bt,temp});
       i++;
    }
    pair<int,Process*> p = ready_queue.top();  // this is the element to be processed next
    ready_queue.pop();
    order.push_back(p.second->pid);
    cpu_time += p.first;
    p.second->tat = cpu_time- p.second->at;
    p.second->wt = p.second->tat - p.second->bt;
    atat += p.second->tat;
    awt += p.second->wt;
    art += cpu_time - p.first -p.second->at;
    if(ready_queue.empty()&&i<n){
      temp = &processes[i];
      ready_queue.push({processes[i].bt,temp});
      i++;
    }
  }
  atat = atat/(double(n));
  awt = awt/(double(n));
  art = art/(double(n));
  fstream myfile;
  myfile.open("output.txt",ios::out);
  if(myfile.is_open()){
   myfile<<"SJF is optimal for minimizing average waiting time when burst times are highly variable."<<endl;
   myfile<<"variance arrival time "<<to_string(vat)<<endl;
   myfile<<"variance burst time "<<to_string(vbt)<<endl;
   myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
   myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
   myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
   myfile <<endl;
   myfile <<"Order of process execution :"<<endl;
   for(auto it :order) myfile <<it<<" ";
   myfile.close();
  }

}

void ljfs(vector<Process> &processes,double vbt, double vat, double abt){
  vector<string> order;
  int cpu_time =processes[0].at,i=0,n = processes.size();
  double atat =0,awt=0,art=0;
  priority_queue<pair<int,Process*>> ready_queue;
  Process* temp = &processes[i];
  ready_queue.push({processes[i].bt,temp});
  i++;
  while(!ready_queue.empty()){
    // take all the processes in the ready queue, which have arrival time less than or equal to the cpu time 
    while(i<n&&processes[i].at<=cpu_time){
       temp = &processes[i];
       ready_queue.push({processes[i].bt,temp});
       i++;
    }
    pair<int,Process*> p = ready_queue.top();  // this is the element to be processed next
    ready_queue.pop();
    order.push_back(p.second->pid);
    cpu_time += p.first;
    p.second->tat = cpu_time- p.second->at;
    p.second->wt = p.second->tat - p.second->bt;
    atat += p.second->tat;
    awt += p.second->wt;
    art += cpu_time - p.first -p.second->at;
    if(ready_queue.empty()&&i<n){
      temp = &processes[i];
      ready_queue.push({processes[i].bt,temp});
      i++;
    }
  }
  atat = atat/(double(n));
  awt = awt/(double(n));
  art = art/(double(n));
  fstream myfile;
  myfile.open("output.txt",ios::out);
  if(myfile.is_open()){
   myfile<<"LJFS prioritizes longer jobs when burst time variance is low."<<endl;
   myfile<<"variance arrival time "<<to_string(vat)<<endl;
   myfile<<"variance burst time "<<to_string(vbt)<<endl;
   myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
   myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
   myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
   myfile <<endl;
   myfile <<"Order of process execution :"<<endl;
   for(auto it :order) myfile <<it<<" ";
   myfile.close();
  }

}

void srtf(vector<Process> &processes,double vbt, double vat, double abt){
  vector<string> order;
  int cpu_time =processes[0].at,i=0,n = processes.size();
  double atat =0,awt=0,art=0;
  priority_queue<pair<int,Process*>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> ready_queue; // here the first ele of pair holds the remaining burst time 
  Process* temp = &processes[i];
  ready_queue.push({processes[i].bt,temp});
  i++;
  while(!ready_queue.empty()){
    // take all the processes in the ready queue, which have arrival time less than or equal to the cpu time 
    while(i<n&&processes[i].at<=cpu_time){
       temp = &processes[i];
       ready_queue.push({processes[i].bt,temp});
       i++;
    }
    pair<int,Process*> p = ready_queue.top();  // this is the element to be processed next
    ready_queue.pop();
    if(order.size()==0||order.back()!=p.second->pid)
     order.push_back(p.second->pid);  

    if(p.first == p.second->bt) art += (cpu_time - p.second->at);
    cpu_time += 1;
    p.first -=1;
    if(p.first==0){
    p.second->tat = cpu_time- p.second->at;
    p.second->wt = p.second->tat - p.second->bt;
    atat += p.second->tat;
    awt += p.second->wt;
    }
    else ready_queue.push(p);
    if(ready_queue.empty()&&i<n){
      cpu_time = processes[i].at;
      temp = &processes[i];
      ready_queue.push({processes[i].bt,temp});
      i++;
    }
  }
  atat = atat/(double(n));
  awt = awt/(double(n));
  art = art/(double(n));
  fstream myfile;
  myfile.open("output.txt",ios::out);
  if(myfile.is_open()){
    myfile<<"Both arrival and burst times are highly variable, SRTF is chosen to handle dynamic changes effectively."<<endl;
   myfile<<"variance arrival time "<<to_string(vat)<<endl;
   myfile<<"variance burst time "<<to_string(vbt)<<endl;
   myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
   myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
   myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
   myfile <<endl;
   myfile <<"Order of process execution :"<<endl;
   for(auto it :order) myfile <<it<<" ";
   myfile.close();
  }
 
}

void round_robin(vector<Process> &processes,double vbt, double vat, double abt){
   vector<string> order;
   int cpu_time = processes[0].at,i=0,n = processes.size();
   double atat =0,awt=0,art=0;
   queue<pair<int,Process*>> q;
   Process* temp = &processes[0];
   q.push({temp->bt,temp});
   i++;
   while(!q.empty()){
     pair<int,Process*> p = q.front();
     q.pop();
     if(p.first==p.second->bt) art +=(cpu_time - p.second->at);
     if(order.empty()||order.back()!=p.second->pid) order.push_back(p.second->pid);
     cpu_time+=1;
     p.first -=1;
     while(i<n&&processes[i].at<=cpu_time){
      temp = &processes[i];
      q.push({temp->bt,temp});
      i++;
     }
     if(p.first==0){
       atat+= cpu_time - p.second->at;
       awt += (cpu_time-p.second->at - p.second->bt);
     }
     else q.push(p);
     if(q.empty()&&i<n){
      temp = &processes[i];
      q.push({temp->bt,temp});
      cpu_time = temp->at;
      i++;
     }
   }

   atat = atat/(double(n));
   awt = awt/(double(n));
   art = art/(double(n));
   fstream myfile;
   myfile.open("output.txt",ios::out);
   if(myfile.is_open()){
   myfile<<"As variance of arrival time and burst time is low Round Robin is used for equal treatment and responsiveness in time-sharing systems"<<endl;
   myfile<<"variance arrival time "<<to_string(vat)<<endl;
   myfile<<"variance burst time "<<to_string(vbt)<<endl;
   myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
   myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
   myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
   myfile <<endl;
   myfile <<"Order of process execution :"<<endl;
   for(auto it :order) myfile <<it<<" ";
   myfile.close();
   }
}

void priorityScheduling(vector<Process> &processes,double vbt, double vat, double abt){
    vector<string> order;
    int cpu_time =0, i=0, n = processes.size();
    double atat =0,art=0,awt=0;
    priority_queue<pair<int,pair<int,Process*>>> pq;
    cpu_time = processes[i].at;
    Process* temp;
    while(i<n&&processes[i].at<=cpu_time){
     temp = &processes[i];
     pq.push({processes[i].priority,{temp->bt,temp}});
     i++; 
    }

    while(!pq.empty()){
      // i have all the processes with arrival time <=cputime
      pair<int,pair<int ,Process*>> p = pq.top();
      pq.pop();
      if(p.second.first==p.second.second->bt){
        art += (cpu_time - p.second.second->at);
      }
      if(order.empty()||order.back()!=p.second.second->pid) order.push_back(p.second.second->pid);
      cpu_time +=1; // executing that process for 1 unit time;
      p.second.first -=1;
      if(p.second.first==0){ // process is finished 
          atat += cpu_time - p.second.second->at; // finishedtime-arrivatime
          awt += (cpu_time -p.second.second->at - p.second.second->bt);
      }
      else{
        pq.push(p);
      }
      // take all the processes that has come in that unit time 
      while(i<n&&processes[i].at<=cpu_time){
        temp = &processes[i];
        pq.push({temp->priority,{temp->bt,temp}});
        i++;
      }
      // taking the case when no process has come and pq is empty 
      if(i<n&&pq.empty()){
        cpu_time = processes[i].at;
        while(i<n&&processes[i].at<=cpu_time){
         temp = &processes[i];
         pq.push({temp->priority,{temp->bt,temp}});
         i++; 
        }
      }
    }
  atat = atat/(double(n));
  awt = awt/(double(n));
  art = art/(double(n));
  fstream myfile;
  myfile.open("output.txt",ios::out);
  if(myfile.is_open()){
   myfile<< "The choosen Scheduling algorithm is : Priority Scheduling"<<endl;
   myfile <<"Average turn around time for the given dataset is : "<< to_string(atat) <<endl;
   myfile <<"Average wait time for the given dataset is : "<< to_string(awt) <<endl;
   myfile <<"Average response time for the given dataset is : "<< to_string(art) <<endl;
   myfile <<endl;
   myfile <<"Order of process execution :"<<endl;
   for(auto it :order) myfile <<it<<" ";
   myfile.close();
  }
}

bool validP(Process &p){
  if(p.bt==0||p.pid.compare("")==0) return false;
  return true;
}

void getData(vector<Process> &processes){
     fstream myfile;
     myfile.open("input.txt",ios::in);
     while(myfile.is_open()){
        string line;
		while(getline(myfile,line)){
            Process p;
			string data=""; int nd=0,i=0,n = line.length();
        while(nd<4&&i<n){
          if(line[i]==' '){
				   i++;
				   continue;
			    }
          else{
				  nd++;
				  while(line[i]!=' '&&i<n){
					data +=line[i];
					i++;
				   } 
                // if nd =1 -> pid, nd=2 -> arrival time , nd =3 -> burst time, nd = 4 -> priority
                if(nd==1) p.pid = data;
                else if(nd ==2) p.at = stoi(data);
                else if(nd == 3) p.bt = stoi(data);
                else p.priority = stoi(data);
               data = "";
			   }
			}
      if(validP(p))
      processes.push_back(p);
		}
      myfile.close();
     }
   sort(processes.begin(),processes.end(),[](Process &a, Process &b){return a.at<=b.at;});
}

// driver code;
int main() 
{   int v_th_high = 10, v_th_low = 2;
    double vbt=0,vat=0,abt=0;
    vector<Process> processes;
    getData(processes);
    map<string,int> algo;
    algo["fcfs"] = 1;  // in salgo
    algo["sjf"] =2;   // in salgo
    algo["ljfs"] =3;  // in salgo  
    algo["srtf"] =4;  // in salgo
    algo["round_robin"] = 5; // in salgo
    algo["Priority_scheduling_preemptive"] = 6;
    string algo_to_apply = predictAlgo(processes,v_th_high,v_th_low,vbt,vat,abt);
    cout<<algo_to_apply<<endl;
   // algo_to_apply => fcfs,
    switch(algo[algo_to_apply]){
      case 1:
       fcfs(processes,vbt,vat,abt);
       break;
      case 2:
       sjf(processes,vbt,vat,abt);
       break;
      case 3:
       ljfs(processes,vbt,vat,abt);
       break;
      case 4:
       srtf(processes,vbt,vat,abt);
       break;
      case 5:
       round_robin(processes,vbt,vat,abt);
       break;
      case 6:
       priorityScheduling(processes,vbt,vat,abt);
      break;
    }
return 0;
}
