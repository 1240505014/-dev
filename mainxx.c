#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "/var/log/syslog" // Syslog dosya yolu
#define MAX_LINE_LENGTH 1024 // Maksimum sat�r uzunlu�u

// Ba�l� liste d���m yap�s�
typedef struct LogNode {
    char logEntry[MAX_LINE_LENGTH];
    struct LogNode* next;
} LogNode;

// Ba�l� listeye yeni d���m ekleme fonksiyonu
void append(LogNode** head, const char* logEntry) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (!newNode) {
        perror("Bellek tahsisi ba�ar�s�z");
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

// Ba�l� listedeki loglar� yazd�rma fonksiyonu
void printLogs(LogNode* head) {
    while (head) {
        printf("%s", head->logEntry);
        head = head->next;
    }
}

// Ba�l� listeyi temizleme fonksiyonu
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
        perror("Syslog dosyas� a��lamad�");
        return EXIT_FAILURE;
    }

    LogNode* logList = NULL;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        append(&logList, buffer);
    }

    fclose(file);

    printf("Sistem G�nl�k Kay�tlar�:\n");
    printLogs(logList);

    freeList(logList);
    return EXIT_SUCCESS;
}

