#define _CRT_SECURE_NO_WARNINGS
#define zero 0 //���� ��������

#include <ctype.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 100 //�޸��� ũ��
#define S_REG_SIZE 8 //s�������� ����
#define T_REG_SIZE 10 //t���������� ����
#define TOKEN_SIZE 5 // token�� �ִ����

typedef struct {
    int data;
} Register; //�������� ����ü

typedef struct { // opcode, operand1~operand3�� ����ü 
    char opcode[5]; //�� �ܾ��� ũ��� 5�� �Ѿ�� �ʱ� ������ 5���� ����
    char operand1[5];
    char operand2[5];
    char operand3[5];
}inst;

char memory[MEMORY_SIZE][MEMORY_SIZE]; //�������� 2���� �迭
char IR[MEMORY_SIZE]; //fetch�� IR�� ��instruction����
int pc;

Register t[10]; //t��������
Register s[8]; //s��������
Register v[1]; //v��������

int decode(char* instruction, inst* decoded_instruction);
void execute(inst decoded_instruction);
void execute_LW(inst decoded_instruction);
void execute_ADD(inst decoded_instruction);
void execute_SUB(inst decoded_instruction);
void execute_MUL(inst decoded_instruction);
void execute_DIV(inst decoded_instruction);
void execute_NOP();
void execute_JMP(inst decoded_instruction);
void execute_BEQ(inst decoded_instruction);
void execute_BNE(inst decoded_instruction);
void execute_SLT(inst decoded_instruction);


int main(int argc, char* argv[]) {
    // ���ϸ��� ����� ���ڷ� �޾ƿ�
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    char* filename = argv[1];

    char buffer[MEMORY_SIZE]; // �� ���� ���� ����

    //���� ����
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("���� ���� ����");
        return 1;
    }

    int row = 0; // �� �ε���
    int col = 0; // �� �ε���

    //���Ͽ��� �� �پ� �о 2���� �迭�� ����
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        col = 0; // �� �ε��� �ʱ�ȭ
        //���� ���ڿ��� 2���� �迭�� ����
        for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; ++i) {
            memory[row][col++] = buffer[i];
        }
        memory[row][col] = '\0'; // ���ڿ� ���� �� ���� �߰�
        row++; // ���� ������ �̵�
    }

    //���� �ݱ�
    fclose(file);

    inst decoded_instruction;

    // load �ܰ�
    pc = 0;
    while (pc < row) {
        strcpy(IR, memory[pc]);  // fetch �ܰ�
        decode(IR, &decoded_instruction); // decode �ܰ� 
        printf("(line %d) %s\n", pc + 1, memory[pc]);
        printf("32217072> ");
        pc++;
        execute(decoded_instruction);
    }

    return 0;
}

int decode(char* instruction, inst* decoded_instruction) {
    // ������ �������� ��ūȭ�Ͽ� �м�
    char* token = strtok(instruction, " ");
    if (token == NULL) {
        printf("Error: The instruction is empty.\n");
        return 1; // ��ū�� ������ ����
    }

    // opcode ����
    if (strlen(token) >= TOKEN_SIZE) {
        printf("Error: Opcode length exceeds the allowed limit.\n"); // opcode ���� ����
        return 1;
    }
    strcpy(decoded_instruction->opcode, token);

    // operand1 ����
    token = strtok(NULL, " ");
    if (token != NULL) {
        if (strlen(token) >= TOKEN_SIZE) {
            printf("Error: Operand1 length exceeds the allowed limit.\n"); // operand1 ���� ����
            return 1;
        }
        strcpy(decoded_instruction->operand1, token);
    }
    else {
        strcpy(decoded_instruction->operand1, "");
    }

    // operand2 ����
    token = strtok(NULL, " ");
    if (token != NULL) {
        if (strlen(token) >= TOKEN_SIZE) {
            printf("Error: Operand2 length exceeds the allowed limit.\n"); //operand2 ���� ����
            return 1;
        }
        strcpy(decoded_instruction->operand2, token);
    }
    else {
        strcpy(decoded_instruction->operand2, "");
    }

    // operand3 ����
    token = strtok(NULL, " ");
    if (token != NULL) {
        if (strlen(token) >= TOKEN_SIZE) {
            printf("Error: Operand3 length exceeds the allowed limit.\n"); // operand3 ���� ����
            return 1;
        }
        strcpy(decoded_instruction->operand3, token);
    }
    else {
        strcpy(decoded_instruction->operand3, "");
    }

    // �߰� ��ū�� �ִ��� Ȯ��
    token = strtok(NULL, " ");
    if (token != NULL) {
        printf("Error: There are more than four tokens.\n"); // �ܾ 5�� �̻��̸� ����
        return 1;
    }


    return 0; // ���������� �м� �Ϸ�
}

void execute(inst decoded_instruction) {
    // opcode�� ���� ����
    if (strcmp(decoded_instruction.opcode, "LW") == 0) {
        //LW ���� ����
        execute_LW(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "ADD") == 0) {
        // ADD ���� ����
        execute_ADD(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "SUB") == 0) {
        // SUB ���� ����
        execute_SUB(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "MUL") == 0) {
        // MUL ���� ����
        execute_MUL(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "DIV") == 0) {
        //DIV ���� ����
        execute_DIV(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "NOP") == 0) {
        // NOP ���� ����
        execute_NOP();
    }
    else if (strcmp(decoded_instruction.opcode, "JMP") == 0) {
        // JMP ���� ����
        execute_JMP(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "BEQ") == 0) {
        // BEQ ���� ����
        execute_BEQ(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "BNE") == 0) {
        // BNE ���� ����
        execute_BNE(decoded_instruction);
    }
    else if (strcmp(decoded_instruction.opcode, "SLT") == 0) {
        // SLT ���� ����
        execute_SLT(decoded_instruction);
    }
    else {
        printf("Error: Unknown instruction\n"); // ����ġ ���� opcode ����
    }
}

void execute_LW(inst decoded_instruction) {
    // Operand1�� ������ ���������� �ε���
    char reg_type = decoded_instruction.operand1[0]; // �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // �������� �ε���

    // Operand2�� �ִ� 16���� �� ��������
    int data;
    data = strtol(decoded_instruction.operand2, NULL, 0);

    // Operand1�� ������ ���������� ���� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    // �������Ϳ� ������ �ε�
    switch (reg_type) {
    case 's':
        s[reg_index].data = data;
        printf("Loaded %d to s%d\n", data, reg_index);
        break;
    case 'v':
        v[reg_index].data = data;
        printf("Loaded %d to v%d\n", data, reg_index);
        break;
    case 't':
        t[reg_index].data = data;
        printf("Loaded %d to t%d\n", data, reg_index);
        break;
    default:
        printf("Unknown register type\n");
    }
}


void execute_ADD(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���
    char reg2_type = decoded_instruction.operand3[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg2_index = atoi(&decoded_instruction.operand3[1]); // operand3�� �������� �ε���

    // Operand1, Operand2, Operand3�� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg2_type != 's' && reg2_type != 't' && reg2_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand3�� ������ ���������� �ε��� ���� �˻�
    if (reg2_type == 's' && (reg2_index < 0 || reg2_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 't' && (reg2_index < 0 || reg2_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 'v' && (reg2_index < 0 || reg2_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    int result = 0; // ����� ������ ���� �ʱ�ȭ

    // �� �������� Ÿ�Կ� �°� ���� ����
    if (reg1_type == 's' && reg2_type == 's') {
        result = s[reg1_index].data + s[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 't') {
        result = s[reg1_index].data + t[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 'z') {
        result = s[reg1_index].data + zero;
    }
    else if (reg1_type == 't' && reg2_type == 's') {
        result = t[reg1_index].data + s[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 't') {
        result = t[reg1_index].data + t[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 'z') {
        result = t[reg1_index].data + zero;
    }
    else if (reg1_type == 'z' && reg2_type == 's') {
        result = zero + s[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 't') {
        result = zero + t[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 'z') {
        result = zero + zero;
    }

    // ����� Operand1�� ����� �������Ϳ� ����
    if (reg_type == 's') {
        s[reg_index].data = result;
    }
    else if (reg_type == 't') {
        t[reg_index].data = result;
    }
    else if (reg_type == 'v') {
        v[reg_index].data = result;
    }

    // ��� 
    printf("Added %c%d(%d) to %c%d(%d) and changed %c%d to %d\n",
        reg1_type, reg1_index, (reg1_type == 's') ? s[reg1_index].data : (reg1_type == 't') ? t[reg1_index].data : zero,
        reg2_type, reg2_index, (reg2_type == 's') ? s[reg2_index].data : (reg2_type == 't') ? t[reg2_index].data : zero,
        reg_type, reg_index, (reg_type == 's') ? s[reg_index].data : (reg_type == 't') ? t[reg_index].data : v[reg_index].data);
}



void execute_SUB(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���
    char reg2_type = decoded_instruction.operand3[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg2_index = atoi(&decoded_instruction.operand3[1]); // operand3�� �������� �ε���

    int result = 0; // ����� ������ ���� �ʱ�ȭ

    // Operand1, Operand2, Operand3�� ���� �������� Ÿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg2_type != 's' && reg2_type != 't' && reg2_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }

    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand3�� ������ ���������� �ε��� ���� �˻�
    if (reg2_type == 's' && (reg2_index < 0 || reg2_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 't' && (reg2_index < 0 || reg2_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 'v' && (reg2_index < 0 || reg2_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }


    // �� �������� Ÿ�Կ� �°� ���� ����
    if (reg1_type == 's' && reg2_type == 's') {
        result = s[reg1_index].data - s[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 't') {
        result = s[reg1_index].data - t[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 'z') {
        result = s[reg1_index].data - zero;
    }
    else if (reg1_type == 't' && reg2_type == 's') {
        result = t[reg1_index].data - s[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 't') {
        result = t[reg1_index].data - t[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 'z') {
        result = t[reg1_index].data - zero;
    }
    else if (reg1_type == 'z' && reg2_type == 's') {
        result = zero - s[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 't') {
        result = zero - t[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 'z') {
        result = zero - zero;
    }


    // ����� Operand1�� ����� �������Ϳ� ����
    if (reg_type == 's') {
        s[reg_index].data = result;
    }
    else if (reg_type == 't') {
        t[reg_index].data = result;
    }
    else if (reg_type == 'v') {
        v[reg_index].data = result;
    }

    // ��� (�׽�Ʈ��)
    printf("Subtracted %c%d(%d) to %c%d(%d) and changed %c%d to %d\n",
        reg1_type, reg1_index, (reg1_type == 's') ? s[reg1_index].data : (reg1_type == 't') ? t[reg1_index].data : zero,
        reg2_type, reg2_index, (reg2_type == 's') ? s[reg2_index].data : (reg2_type == 't') ? t[reg2_index].data : zero,
        reg_type, reg_index, (reg_type == 's') ? s[reg_index].data : (reg_type == 't') ? t[reg_index].data : v[reg_index].data);
}


void execute_MUL(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���
    char reg2_type = decoded_instruction.operand3[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg2_index = atoi(&decoded_instruction.operand3[1]); // operand3�� �������� �ε���


    // Operand1, Operand2, Operand3 �� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg2_type != 's' && reg2_type != 't' && reg2_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }

    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand3�� ������ ���������� �ε��� ���� �˻�
    if (reg2_type == 's' && (reg2_index < 0 || reg2_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 't' && (reg2_index < 0 || reg2_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 'v' && (reg2_index < 0 || reg2_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    int result = 0; // ����� ������ ���� �ʱ�ȭ

    // �� �������� Ÿ�Կ� �°� ���� ����
    if (reg1_type == 's' && reg2_type == 's') {
        result = s[reg1_index].data * s[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 't') {
        result = s[reg1_index].data * t[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 'z') {
        result = s[reg1_index].data * zero;
    }
    else if (reg1_type == 't' && reg2_type == 's') {
        result = t[reg1_index].data * s[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 't') {
        result = t[reg1_index].data * t[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 'z') {
        result = t[reg1_index].data * zero;
    }
    else if (reg1_type == 'z' && reg2_type == 's') {
        result = zero * s[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 't') {
        result = zero * t[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 'z') {
        result = zero * zero;
    }

    // ����� Operand1�� ����� �������Ϳ� ����
    if (reg_type == 's') {
        s[reg_index].data = result;
    }
    else if (reg_type == 't') {
        t[reg_index].data = result;
    }
    else if (reg_type == 'v') {
        v[reg_index].data = result;
    }

    // ��� 
    printf("Multiplied %c%d(%d) to %c%d(%d) and changed %c%d to %d\n",
        reg1_type, reg1_index, (reg1_type == 's') ? s[reg1_index].data : (reg1_type == 't') ? t[reg1_index].data : zero,
        reg2_type, reg2_index, (reg2_type == 's') ? s[reg2_index].data : (reg2_type == 't') ? t[reg2_index].data : zero,
        reg_type, reg_index, (reg_type == 's') ? s[reg_index].data : (reg_type == 't') ? t[reg_index].data : v[reg_index].data);
}

void execute_DIV(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���
    char reg2_type = decoded_instruction.operand3[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg2_index = atoi(&decoded_instruction.operand3[1]); // operand3�� �������� �ε���

    // ���� ó��: 0���� ������ ���
    if (reg2_type == 's' && s[reg2_index].data == 0) {
        printf("Error: Division by zero\n");
        return;
    }
    else if (reg2_type == 'v' && t[reg2_index].data == 0) {
        printf("Error: Division by zero\n");
        return;
    }
    else if (reg2_type == 't' && t[reg2_index].data == 0) {
        printf("Error: Division by zero\n");
        return;
    }

    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type.\n");
        return;
    }

    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand3�� ������ ���������� �ε��� ���� �˻�
    if (reg2_type == 's' && (reg2_index < 0 || reg2_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 't' && (reg2_index < 0 || reg2_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 'v' && (reg2_index < 0 || reg2_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    // Operand1, Operand2, Operand3 �� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg2_type != 's' && reg2_type != 't' && reg2_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }

    int result = 0;
    // �� �������� Ÿ�Կ� �°� ���� ����
    if (reg1_type == 's' && reg2_type == 's') {
        result = s[reg1_index].data / s[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 't') {
        result = s[reg1_index].data / t[reg2_index].data;
    }
    else if (reg1_type == 's' && reg2_type == 'z') {
        printf("Error: Division by zero\n");
        return;
    }
    else if (reg1_type == 't' && reg2_type == 's') {
        result = t[reg1_index].data / s[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 't') {
        result = t[reg1_index].data / t[reg2_index].data;
    }
    else if (reg1_type == 't' && reg2_type == 'z') {
        printf("Error: Division by zero\n");
        return;
    }
    else if (reg1_type == 'z' && reg2_type == 's') {
        result = zero / s[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 't') {
        result = zero / t[reg2_index].data;
    }
    else if (reg1_type == 'z' && reg2_type == 'z') {
        printf("Error: Division by zero\n");
        return;
    }


    // ����� Operand1�� ����� �������Ϳ� ����
    if (reg_type == 's') {
        s[reg_index].data = result;
    }
    else if (reg_type == 't') {
        t[reg_index].data = result;
    }
    else if (reg_type == 'v') {
        v[reg_index].data = result;
    }

    // ���
    printf("Diveded %c%d(%d) to %c%d(%d) and changed %c%d to %d\n",
        reg1_type, reg1_index, (reg1_type == 's') ? s[reg1_index].data : (reg1_type == 't') ? t[reg1_index].data : zero,
        reg2_type, reg2_index, (reg2_type == 's') ? s[reg2_index].data : (reg2_type == 't') ? t[reg2_index].data : zero,
        reg_type, reg_index, (reg_type == 's') ? s[reg_index].data : (reg_type == 't') ? t[reg_index].data : v[reg_index].data);
}

void execute_NOP() {
    printf("No Operation\n");
}

void execute_JMP(inst decoded_instruction) {
    int line; // line number
    if (isdigit(decoded_instruction.operand1[0])) {
        // ���� operand1�� ���ڷ� �����ϸ� ���� ���ڷ� �ؼ�
        line = strtol(decoded_instruction.operand1, NULL, 0); //line number
    }
    else if (decoded_instruction.operand1[0] == 's' ||
        decoded_instruction.operand1[0] == 't' ||
        decoded_instruction.operand1[0] == 'v') {
        // ���� operand1�� ���������� ���, ���� ó��
        printf("Error: The JMP instruction cannot directly use a register.\n");
        return;
    }
    else {
        // �׷��� ������ ���� ó��
        printf("Error: JMP invalid operand.\n");
        return;
    }


    printf("Jumped to line %d\n", line); //���
    pc = line - 1; // pc�� �޸��� ���� ���� ������ line���� 1�� ��

}



void execute_BEQ(inst decoded_instruction) {
    // Operand1�� ������ ���������� �ε���
    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���

    int line; // line number
    line = strtol(decoded_instruction.operand3, NULL, 0);
    //line number

    // Operand1, Operand2�� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    if (reg_type == 's' && reg1_type == 's') {

        if (s[reg_index].data == s[reg1_index].data) { //������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (s[reg_index].data != s[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 's' && reg1_type == 't') {

        if (s[reg_index].data == t[reg1_index].data) { //������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (s[reg_index].data != t[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }


    }
    else if (reg_type == 's' && reg1_type == 'z') {

        if (s[reg_index].data == zero) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, zero, line);
        }
        else if (s[reg_index].data != zero) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, zero, line);
            return;
        }

    }
    else if (reg_type == 't' && reg1_type == 's') {
        if (t[reg_index].data == s[reg1_index].data) { // ������ ���� 
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (t[reg_index].data != s[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 't' && reg1_type == 't') {

        if (t[reg_index].data == t[reg1_index].data) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (t[reg_index].data != t[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }
    }
    else if (reg_type == 't' && reg1_type == 'z') {
        if (t[reg_index].data == zero) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, zero, line);

        }
        else if (t[reg_index].data != zero) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, zero, line);
            return;
        }
    }
    else if (reg_type == 'z' && reg1_type == 's') {
        if (zero == s[reg1_index].data) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (zero != s[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 'z' && reg1_type == 't') {
        if (zero == t[reg1_index].data) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (zero != t[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 'z' && reg1_type == 'z') {
        if (zero == t[reg1_index].data) { // ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (zero != t[reg1_index].data) { // ���� ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }

    }

}

void execute_BNE(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���

    int line; //line number
    line = strtol(decoded_instruction.operand3, NULL, 0);
    //line number

        // Operand1, Operand2�� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    if (reg_type == 's' && reg1_type == 's') {
        if (s[reg_index].data != s[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (s[reg_index].data == s[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 's' && reg1_type == 't') {

        if (s[reg_index].data != t[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (s[reg_index].data == t[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }


    }
    else if (reg_type == 's' && reg1_type == 'z') {

        if (s[reg_index].data != zero) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, zero, line);
        }
        else if (s[reg_index].data == zero) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, s[reg_index].data,
                reg1_type, reg1_index, zero, line);
            return;
        }

    }
    else if (reg_type == 't' && reg1_type == 's') {

        if (t[reg_index].data != s[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (t[reg_index].data == s[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }


    }
    else if (reg_type == 't' && reg1_type == 't') {

        if (t[reg_index].data != t[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (t[reg_index].data == t[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not  equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 't' && reg1_type == 'z') {

        if (t[reg_index].data != zero) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, zero, line);

        }
        else if (t[reg_index].data == zero) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, t[reg_index].data,
                reg1_type, reg1_index, zero, line);
            return;
        }

    }
    else if (reg_type == 'z' && reg1_type == 's') {

        if (zero != s[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, s[reg1_index].data, line);
        }
        else if (zero == s[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, s[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 'z' && reg1_type == 't') {

        if (zero != t[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
        }
        else if (zero == t[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, t[reg1_index].data, line);
            return;
        }

    }
    else if (reg_type == 'z' && reg1_type == 'z') {

        if (zero != t[reg1_index].data) { // ���� ������ ����
            pc = line - 1;
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and jumped to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, zero, line);
        }
        else if (zero == t[reg1_index].data) { // ������ ���� ��ɾ�
            printf("checked if %c%d(%d) is not equal to %c%d(%d) and didn't jump to line %d\n", reg_type, reg_index, zero,
                reg1_type, reg1_index, zero, line);
            return;
        }

    }

}

void execute_SLT(inst decoded_instruction) {

    char reg_type = decoded_instruction.operand1[0]; // operand1�� �������� Ÿ�� (s, v, t)
    int reg_index = atoi(&decoded_instruction.operand1[1]); // operand1�� �������� �ε���
    char reg1_type = decoded_instruction.operand2[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg1_index = atoi(&decoded_instruction.operand2[1]); // operand2�� �������� �ε���
    char reg2_type = decoded_instruction.operand3[0]; // operand2�� �������� Ÿ�� (s, v, t)
    int reg2_index = atoi(&decoded_instruction.operand3[1]); // operand3�� �������� �ε���

    int result = 0; // ����� ������ ���� �ʱ�ȭ

    // Operand1, Operand2, Operand3�� ������ ���������� ��ȿ�� �˻�
    if (reg_type != 's' && reg_type != 't' && reg_type != 'v') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg1_type != 's' && reg1_type != 't' && reg1_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    if (reg2_type != 's' && reg2_type != 't' && reg2_type != 'z') {
        printf("Error: invalid register type\n");
        return;
    }
    // Operand1�� ������ ���������� �ε��� ���� �˻�
    if (reg_type == 's' && (reg_index < 0 || reg_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 't' && (reg_index < 0 || reg_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg_type == 'v' && (reg_index < 0 || reg_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand2�� ������ ���������� �ε��� ���� �˻�
    if (reg1_type == 's' && (reg1_index < 0 || reg1_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 't' && (reg1_index < 0 || reg1_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg1_type == 'v' && (reg1_index < 0 || reg1_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }
    // Operand3�� ������ ���������� �ε��� ���� �˻�
    if (reg2_type == 's' && (reg2_index < 0 || reg2_index > S_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 't' && (reg2_index < 0 || reg2_index > T_REG_SIZE - 1)) {
        printf("Error: invalid register index\n");
        return;
    }
    if (reg2_type == 'v' && (reg2_index < 0 || reg2_index > 0)) {
        printf("Error: invalid register index\n");
        return;
    }

    // �� �������� Ÿ�Կ� �°� ���� ����
    if (reg1_type == 's' && reg2_type == 's') {
        if (s[reg1_index].data < s[reg2_index].data) { // operand2�� operand3���� ������ result = 1
            result = 1;
        }
        else if (s[reg1_index].data >= s[reg2_index].data) { // operand2�� operand3 ���� ���� ������ result = 0
            result = 0;
        }
    }
    else if (reg1_type == 's' && reg2_type == 't') { // operand2�� operand3���� ������ result = 1
        if (s[reg1_index].data < t[reg2_index].data) {
            result = 1;
        }
        else if (s[reg1_index].data >= t[reg2_index].data) { // operand2�� operand3 ���� ���� ������ result = 0
            result = 0;
        }
    }
    else if (reg1_type == 't' && reg2_type == 's') { // operand2�� operand3���� ������ result = 1
        if (t[reg1_index].data < s[reg2_index].data) {
            result = 1;
        }
        else if (t[reg1_index].data >= s[reg2_index].data) { // operand2�� operand3 ���� ���� ������ result = 0
            result = 0;
        }
    }
    else if (reg1_type == 't' && reg2_type == 't') { // operand2�� operand3���� ������ result = 1
        if (t[reg1_index].data < t[reg2_index].data) {
            result = 1;
        }
        else if (t[reg1_index].data >= t[reg2_index].data) { // operand2�� operand3 ���� ���� ������ result = 0
            result = 0;
        }
    }

    // ����� Operand1�� ����� �������Ϳ� ����
    if (reg_type == 's') {
        s[reg_index].data = result;
    }
    else if (reg_type == 't') {
        t[reg_index].data = result;
    }
    else if (reg_type == 'v') {
        v[reg_index].data = result;
    }

    // ���
    printf("checked if %c%d(%d) is less than %c%d(%d) and set %d to %c%d\n",
        reg1_type, reg1_index, (reg1_type == 's') ? s[reg1_index].data : t[reg1_index].data,
        reg2_type, reg2_index, (reg2_type == 's') ? s[reg2_index].data : t[reg2_index].data,
        (reg_type == 's') ? s[reg_index].data : (reg_type == 't') ? t[reg_index].data : v[reg_index].data, reg_type, reg_index);
}