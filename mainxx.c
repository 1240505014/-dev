#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "/var/log/syslog" // Syslog dosya yolu
#define MAX_LINE_LENGTH 1024 // Maksimum satýr uzunluðu

// Baðlý liste düðüm yapýsý
typedef struct LogNode {
    char logEntry[MAX_LINE_LENGTH];
    struct LogNode* next;
} LogNode;

// Baðlý listeye yeni düðüm ekleme fonksiyonu
void append(LogNode** head, const char* logEntry) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (!newNode) {
        perror("Bellek tahsisi baþarýsýz");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->logEntry, logEntry, MAX_LINE_LENGTH - 1);
    newNode->logEntry[MAX_LINE_LENGTH - 1] = '\0';
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    LogNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Baðlý listedeki loglarý yazdýrma fonksiyonu
void printLogs(LogNode* head) {
    while (head) {
        printf("%s", head->logEntry);
        head = head->next;
    }
}

// Baðlý listeyi temizleme fonksiyonu
void freeList(LogNode* head) {
    LogNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE* file = fopen(LOG_FILE, "r");
    if (!file) {
        perror("Syslog dosyasý açýlamadý");
        return EXIT_FAILURE;
    }

    LogNode* logList = NULL;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        append(&logList, buffer);
    }

    fclose(file);

    printf("Sistem Günlük Kayýtlarý:\n");
    printLogs(logList);

    freeList(logList);
    return EXIT_SUCCESS;
}

