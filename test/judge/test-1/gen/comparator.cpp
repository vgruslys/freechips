#include <cstdio>
#include <cstdint>
#include <cstring>
#include <map>

const char* cards[52] = {
    "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks", "As",
    "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ac",
    "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd", "Ad",
    "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh", "Ah" };

uint32_t* readInput32(const char* path_in, uint32_t* size)
{
    FILE* file_in = fopen(path_in, "rb");
    fread(size, sizeof(uint32_t), 1, file_in);
    printf("size=%d\n",*size);
    uint32_t* data = new uint32_t[*size];
    fread(data, sizeof(uint32_t), *size, file_in);
    fclose(file_in);
    return data;
}

uint64_t* readInput64(const char* path_in, uint64_t* size)
{
    FILE* file_in = fopen(path_in, "rb");
    fread(size, sizeof(uint64_t), 1, file_in);
    printf("size=%lld\n",*size);
    uint64_t* data = new uint64_t[*size];
    fread(data, sizeof(uint64_t), *size, file_in);
    fclose(file_in);
    return data;
}

void printHand(uint64_t hand) {
    printf("%s %s %s %s %s %s %s\n",
            cards[ hand & 0x000000000000ff],
            cards[(hand & 0x0000000000ff00) >> 8],
            cards[(hand & 0x00000000ff0000) >> 16],
            cards[(hand & 0x000000ff000000) >> 24],
            cards[(hand & 0x0000ff00000000) >> 32],
            cards[(hand & 0x00ff0000000000) >> 40],
            cards[(hand & 0xff000000000000) >> 48]);
}

int main(int argc, char** argv)
{
    if (argc != 6) {
        printf("usage: %s file_our_rank file_his_rank file_our_out file_his_out file_in\n", argv[0]);
        return 0;
    }
    
    uint32_t size_our;
    uint32_t* data_our = readInput32(argv[1], &size_our);

    uint32_t size_his;
    uint32_t* data_his = readInput32(argv[2], &size_his);

    uint32_t size_our_out;
    uint32_t* data_our_out = readInput32(argv[3], &size_our_out);

    uint32_t size_his_out;
    uint32_t* data_his_out = readInput32(argv[4], &size_his_out);

    uint64_t size_hands;
    uint64_t* data_hands = readInput64(argv[5], &size_hands);

    uint32_t max_diff = 0;
    uint32_t which_set = 0;
    uint32_t hi_rank_our = 0;
    uint32_t lo_rank_our = 1000000000;
    uint32_t hi_rank_his = 0;
    uint32_t lo_rank_his = 1000000000;

    for (uint32_t i = 0; i < size_hands; ++i) {
        if (data_our[i] > data_his[i] && max_diff < data_our[i] - data_his[i]) {
            max_diff = data_our[i] - data_his[i];
            which_set = i;
        }
        else if (data_our[i] < data_his[i] && max_diff < data_his[i] - data_our[i]) {
            max_diff = data_his[i] - data_our[i];
            which_set = i;
        }

        if (data_our[i] > hi_rank_our) hi_rank_our = data_our[i];
        if (data_our[i] < lo_rank_our) lo_rank_our = data_our[i];
        if (data_his[i] > hi_rank_his) hi_rank_his = data_his[i];
        if (data_his[i] < lo_rank_his) lo_rank_his = data_his[i];
    }

    for (uint32_t i = 0; i < 10; ++i) {
        printf("#%d. Our: %d, his: %d. (%x, %x) ", i+1, data_our[i], data_his[i], data_our_out[i], data_his_out[i]);
        printHand(data_hands[i]);
    }

    printf("%d %d %llu\n", size_our, size_his, size_hands); 
    printf("Largest error: %d\n", max_diff);
    printf("Our rank: %d--%d\n", lo_rank_our, hi_rank_our);
    printf("His rank: %d--%d\n", lo_rank_his, hi_rank_his);

    uint64_t culprit = data_hands[which_set];
    printf("\nThe culprit:\n");
    printHand(culprit);

    delete[] data_our;
    delete[] data_his;
    delete[] data_our_out;
    delete[] data_his_out;
    delete[] data_hands;

    return 0;
}
