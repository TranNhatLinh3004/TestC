#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOTH 30
#define MAX_ENTERPRISE 30

typedef enum {
    ADD,
    DELETE,
    UPDATE,
    SEARCH,
    REGISTER,
    ALTER,
    SHOW,
    QUIT,
    INVALID
} CommandType;

typedef struct {
    int booth_index;
    char name[100];
    char abbre[10];
    int itemValue;
    int itemWeight;
} Enterprise;

// Khai báo hàm handleShowCommand trước khi sử dụng
void handleShowCommand(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE], char *command);

//Yêu cầu 1: Hàm lấy tên viết tắt
void getAbbreviation(char *name, char *abbre) {
    int i = 0, j = 0;
    int len = strlen(name);
    while (i < len) {
        if (i == 0 || (i > 0 && name[i - 1] == ' ')) {
            abbre[j++] = name[i];
        }
        i++;
    }
    abbre[j] = '\0';
}

//Yêu cầu 2: Hàm  hàm để kiểm tra từ đầu tiên của chuỗi lệnh và trả về kiểu tương ứng
CommandType getCommandType(const char *command) {
    if (command == NULL || strlen(command) == 0) {
        return INVALID;
    }

    // Sao chép từ đầu tiên trong lệnh
    char firstWord[100];
    int i = 0;
    while (command[i] != '\0' && !isspace(command[i])) {
        firstWord[i] = command[i];
        i++;
    }
    firstWord[i] = '\0'; // Kết thúc chuỗi

    // So sánh từ đầu tiên với các lệnh hợp lệ
    if (strcmp(firstWord, "ADD") == 0) {
        return ADD;
    } else if (strcmp(firstWord, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(firstWord, "UPDATE") == 0) {
        return UPDATE;
    } else if (strcmp(firstWord, "SEARCH") == 0) {
        return SEARCH;
    } else {
        return INVALID;
    }
}




//Yêu cầu 3: Hàm đăng ký doanh nghiệp
int registerEnterprise(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE], 
                       char *name, int booth, int itemValue, int itemWeight, 
                       int *out_booth, char *out_abbre) {
    getAbbreviation(name, out_abbre);
    int index = (booth == -1) ? (strlen(out_abbre) * 30) % 26 : booth;
    
    // Tìm vị trí hợp lệ cho booth
    while (map[index / 15][index % 15] != 0) {
        index = (index + 1) % MAX_BOOTH;
        if (index == booth) return -1; // Không tìm được gian hàng trống
    }

    // Cập nhật thông tin
    map[index / 15][index % 15] = 1;
    *out_booth = index;

    Enterprise e;
    e.booth_index = index;
    strcpy(e.name, name);
    strcpy(e.abbre, out_abbre);
    e.itemValue = itemValue;
    e.itemWeight = itemWeight;

    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        if (enterpriseArray[i].booth_index == -1) {
            enterpriseArray[i] = e;
            break;
        }
    }
    return index + 200;
}

//Yêu cầu 4: In thông tin chi tiết của doanh nghiệp
void printEnterpriseDetails(Enterprise e) {
    printf("||%d||%s||%s||%d||%d||\n", e.booth_index, e.name, e.abbre, e.itemValue, e.itemWeight);
}

//Yêu cầu 5: In thông tin chi tiết của doanh nghiệp
void createEnterprise(Enterprise* e, int booth_index, int itemValue, int itemWeight, char* name, char* abbre) {
    if (e == NULL) {
        e = (Enterprise*)malloc(sizeof(Enterprise));
    }

    e->booth_index = booth_index;
    e->itemValue = itemValue;
    e->itemWeight = itemWeight;
    
    strncpy(e->name, name, sizeof(e->name) - 1);
    e->name[sizeof(e->name) - 1] = '\0'; 

    strncpy(e->abbre, abbre, sizeof(e->abbre) - 1);
    e->abbre[sizeof(e->abbre) - 1] = '\0';  
}


//Yêu cầu 7: Hàm hiển thị sơ đồ gian hàng
void showMap(int map[2][15]) {
    printf("|0 |1 |2 |3 |4 |5 |6 |7 |8 |9 |10|11|12|13|14|\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 15; j++) {
            printf("|%2d", map[i][j]);
        }
        printf("|\n");
    }
}

//Yêu cầu 8 : Hàm hiển thị chỉ số gian hàng theo trạng thái
void showIndexOfStatus(int map[2][15], int status) {
    printf("Index[");
    int first = 1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 15; j++) {
            if (map[i][j] == status) {
                if (!first) printf(", ");
                printf("%d", i * 15 + j);
                first = 0;
            }
        }
    }
    printf("]\n");
}

//Yêu cầu 9 : Hàm này sẽ đếm số lượng gian hàng
void showTotalOfStatus(Enterprise enterpriseArray[30], int status) {
    int count = 0;
    for (int i = 0; i < 30; i++) {
        if (enterpriseArray[i].booth_index == status) {
            count++;
        }
    }

    printf("Show #%d\n", count);
}

//Yêu cầu 10 : Hàm này sẽ in ra thông tin của các gian hàng từ chỉ số start đến chỉ số end
void showIndexBracket(Enterprise enterpriseArray[30], int start, int end) {
    if (end == -1) {
        // Nếu end là -1, chỉ in thông tin của gian hàng tại start
        printf("Show [%d]\n", start);
        printf("Booth index: %d\n", enterpriseArray[start].booth_index);
        printf("Name: %s\n", enterpriseArray[start].name);
        printf("Abbreviation: %s\n", enterpriseArray[start].abbre);
        printf("Item Value: %d\n", enterpriseArray[start].itemValue);
        printf("Item Weight: %d\n", enterpriseArray[start].itemWeight);
    } else {
        // Nếu end khác -1, in thông tin các gian hàng từ start đến end
        printf("Show [%d:%d]\n", start, end);
        for (int i = start; i <= end; i++) {
            printf("Booth index: %d\n", enterpriseArray[i].booth_index);
            printf("Name: %s\n", enterpriseArray[i].name);
            printf("Abbreviation: %s\n", enterpriseArray[i].abbre);
            printf("Item Value: %d\n", enterpriseArray[i].itemValue);
            printf("Item Weight: %d\n", enterpriseArray[i].itemWeight);
        }
    }
}


//Yêu cầu 11 : Hàm xử lý câu lệnh Show
void handleShowCommand(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE], char *command) {
    if (strcmp(command, "Show map") == 0) {
        showMap(map);
    } else if (strcmp(command, "Show &0") == 0) {
        showIndexOfStatus(map, 0);
    } else if (strcmp(command, "Show &1") == 0) {
        showIndexOfStatus(map, 1);
    } else {
        printf("Lenh khong hop le.\n");
    }
}
//Yêu cầu 12: Hàm thay đổi gian hàng doanh nghiệp
int alterEnterprise(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE], 
                    char *abbre, int currentBooth, int newBooth) {
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        if (strcmp(enterpriseArray[i].abbre, abbre) == 0 && enterpriseArray[i].booth_index == currentBooth) {
            if (map[newBooth / 15][newBooth % 15] == 0) {
                map[currentBooth / 15][currentBooth % 15] = 0;
                map[newBooth / 15][newBooth % 15] = 1;
                enterpriseArray[i].booth_index = newBooth;
                return newBooth + 200;
            } else {
                return -1; // Booth mới đã có đăng ký
            }
        }
    }
    return -1; // Doanh nghiệp không tìm thấy
}

//Yêu cầu 13: Hàm xóa doanh nghiệp
int deleteEnterprise(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE], char *abbre, int booth) {
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        if (strcmp(enterpriseArray[i].abbre, abbre) == 0 && enterpriseArray[i].booth_index == booth) {
            map[booth / 15][booth % 15] = 0;
            enterpriseArray[i].booth_index = -1;
            enterpriseArray[i].name[0] = '\0';
            enterpriseArray[i].abbre[0] = '\0';
            return 200; 
        }
    }
    return -1; 
}

//Yêu cầu 14: Hàm xử lý câu lệnh chính
void handleCommand(char *command, int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE]) {
    char cmd[20];
    sscanf(command, "%s", cmd);

    if (strcmp(cmd, "Register") == 0) {
        char name[100];
        int booth, itemValue, itemWeight;
        if (sscanf(command, "Register [%[^]]] [%d] [%d] [%d]", name, &itemValue, &itemWeight, &booth) == 4) {
            char abbre[10];
            int out_booth;
            int result = registerEnterprise(map, enterpriseArray, name, booth, itemValue, itemWeight, &out_booth, abbre);
            if (result >= 200) {
                printf("Dang ky thanh cong. Booth: %d, Abbre: %s\n", out_booth, abbre);
            } else {
                printf("Dang ky that bai.\n");
            }
        } else {
            printf("Cau lenh khong dung dinh dang.\n");
        }
    } else if (strcmp(cmd, "Show") == 0) {
        handleShowCommand(map, enterpriseArray, command);
    } else {
        printf("Lenh khong hop le.\n");
    }
}

//Yêu cầu 15 : Hiện thưc hàm knapsack
int knapsack(Enterprise enterpriseArray[], int maxWeight, int numOfEnterprises, int index) {
    if (index >= numOfEnterprises || maxWeight <= 0) {
        return 0;
    }

    if (enterpriseArray[index].itemWeight > maxWeight) {
        return knapsack(enterpriseArray, maxWeight, numOfEnterprises, index + 1);
    }

    int valueWithoutCurrent = knapsack(enterpriseArray, maxWeight, numOfEnterprises, index + 1);

    int valueWithCurrent = enterpriseArray[index].itemValue + 
        knapsack(enterpriseArray, maxWeight - enterpriseArray[index].itemWeight, numOfEnterprises, index + 1);

    return (valueWithCurrent > valueWithoutCurrent) ? valueWithCurrent : valueWithoutCurrent;
}




// Hàm khởi tạo
void initMapAndEnterprises(int map[2][15], Enterprise enterpriseArray[MAX_ENTERPRISE]) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 15; j++) {
            map[i][j] = 0; // Tất cả gian hàng ban đầu là trống
        }
    }
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        enterpriseArray[i].booth_index = -1; // Tất cả doanh nghiệp ban đầu chưa đăng ký
    }
}

// Hàm main
int main() {
    int map[2][15];
    Enterprise enterpriseArray[MAX_ENTERPRISE];
    initMapAndEnterprises(map, enterpriseArray);

    char command[100];
    while (1) {
        printf("Nhap lenh: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Loại bỏ ký tự xuống dòng

        if (strcmp(command, "Quit") == 0) {
            printf("Thoat chuong trinh.\n");
            break;
        } else {
            handleCommand(command, map, enterpriseArray);
        }
    }

    return 0;
}
