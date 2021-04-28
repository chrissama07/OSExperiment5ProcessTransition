#include<iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct pcb{
    char id;
    int memory;
    int priority;
}pcb;

typedef vector<pcb> pcbList;

pcbList runningList, readyList, blockList, newList, exitList, showList;

int totalMemory = 100;

int printFlag = 0;

void printInformation();

int minIndex(vector<int> a) {
    if (a.empty()) {
        return 0;
    }
    int index = 0;
    for (int i = 1; i < a.size(); ++i) {
        if (a[i] < a[index]) {
            index = i;
        }
    }
    return index;
}

void Dispatch() {
    if (runningList.empty()) {
        if (!readyList.empty()) {
            vector<int> a;
            for (auto & i : readyList) {
                a.push_back(i.priority);
            }
            int index = minIndex(a);
            runningList.push_back(readyList[index]);
            readyList.erase(readyList.begin() + index);
        } else {
            printf("Error! The ReadyList is empty.");
        }
    } else {
        printf("Error! There is a process (%c) is running.", runningList[0].id);
    }
}

void DispatchRelease() {
    if (!readyList.empty()) {
        vector<int> a;
        for (auto & i : readyList) {
                a.push_back(i.priority);
        }
        int index = minIndex(a);
        runningList.push_back(readyList[index]);
        readyList.erase(readyList.begin() + index);
    }
}

void Timeout() {
    if (runningList.empty()) {
        printf("Error! There is no process running.");
    } else {
        pcb p = runningList[0];
        readyList.push_back(p);
        runningList.pop_back();
        Dispatch();
    }
}

void eventWait() {
    if (runningList.empty()) {
        printf("Error! There is no process running.");
    } else {
        pcb p = runningList[0];
        blockList.push_back(p);
        runningList.pop_back();
        if (!readyList.empty()) {
            Dispatch();
        }
    }
}

void eventOccur(char a) {
    int index = -1;
    for (int i = 0; i < blockList.size(); ++i) {
        if (a == blockList[i].id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Error! There is no %c process in blockList.", a);
    } else {
        pcb p = blockList[index];
        readyList.push_back(p);
        blockList.erase(blockList.begin() + index);
        if (readyList.empty() && runningList.empty()) {
            readyList.pop_back();
            runningList.push_back(p);
        }
    }
}

void admitProgress(pcb p) {
    readyList.push_back(p);
    totalMemory -= p.memory;
}

void admit() {
    if (newList.empty()) {
        printf("There is no process to admit.");
    } else {

        int index;
        vector<int> a;
        for (auto & i : newList) {
            a.push_back(i.priority);
        }
        index = minIndex(a);
        if (newList[index].memory <= totalMemory) {
            admitProgress(newList[index]);
            newList.erase(newList.begin() + index);
        } else {
            printf("Memory exceeded.");
        }
    }
}

void admitRelease() {
    int index;
    vector<int> a;
    for (auto & i : newList) {
        a.push_back(i.priority);
    }
    index = minIndex(a);
    if (newList[index].memory <= totalMemory) {
        admitProgress(newList[index]);
        newList.erase(newList.begin() + index);
    }
}

void create() {
    printf("Please input new process.\n");
    printf("id: ");
    char id;
    int memory, priority;
    cin>>id;
    printf("memory: ");
    cin>>memory;
    printf("priority: ");
    cin>>priority;
    pcb newProcess;
    newProcess.id = id;
    newProcess.memory = memory;
    newProcess.priority = priority;

    showList.push_back(newProcess);

    if (newProcess.memory <= totalMemory) {
        admitProgress(newProcess);
    } else {
        newList.push_back(newProcess);
        printf("Memory exceeded.");
    }
}

void release() {
    if (!runningList.empty()) {
        pcb p = runningList[0];
        totalMemory += p.memory;
        exitList.push_back(p);
        runningList.pop_back();
        admitRelease();
        DispatchRelease();
    }
}

void operate(int s) {
    if (s == 1) {
        Dispatch();
        printInformation();
    }
    else if (s == 2) {
        Timeout();
        printInformation();
    }
    else if (s == 3) {
        eventWait();
        printInformation();
    }
    else if (s == 4) {
        printf("Please enter the process to occur: ");
        char a;
        cin>>a;
        eventOccur(a);
        printInformation();
    }
    else if (s == 5) {
        create();
        printInformation();
    }
    else if (s == 6) {
        admit();
        printInformation();
    }
    else if (s == 7) {
        release();
        printInformation();
    }
    else {
        exit(0);
    }
}

void printInformation() {
    printf("\n");
    printf("*************** Processing Information ***************\n\n");

    if (!showList.empty()) {
        printf("All processes: \tID: \tMemory: \tPriority: \n");
        for (auto & i : showList) {
            printf("               \t %c \t %d    \t\t %d\n", i.id, i.memory, i.priority);
        }
    } else {
        printf("There is no process. Please create a new process");
    }
    printf("\n");
    printf("Total Memory remaining: %d\n\n", totalMemory);
    printf("New: ");
    for (auto & i : newList) {
        printf("%c ", i.id);
    }
    printf("\n");
    printf("Running: ");
    for (auto & i : runningList) {
        printf("%c ", i.id);
    }
    printf("\n");
    printf("Ready: ");
    for (auto & i : readyList) {
        printf("%c ", i.id);
    }
    printf("\n");
    printf("Block: ");
    for (auto & i : blockList) {
        printf("%c ", i.id);
    }
    printf("\n");
    printf("Release: ");
    for (auto & i : exitList) {
        printf("%c ", i.id);
    }
    printf("\n");

    printf("Please input your operation:\n");
    printf("1. Dispatch\t2. Timeout\t3. Wait Event\t4.Event Occur\n"
           "5. Create\t6. Admit\t7. Release\t8. Exit.\n");
//    cin>>operation;
//    if (operation < 1 || operation > 8) {
//        printf("Input Error. Please re-input.");
//        printFlag = 1;
//    } else {
//        operate(operation);
//    }
    while (true) {
        int operation;
        cin>>operation;
        if (operation < 1 || operation > 8) {
            printf("Input Error. Please re-input.\n");
            cin.ignore();
            continue;
        } else {
            operate(operation);
            break;
        }
    }
}

int main() {
    printInformation();
    if (printFlag == 1) {
        printFlag = 0;
        printInformation();
    }
    return 0;
}