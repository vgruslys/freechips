#include <cstdio>
#include <cstdint>
#include <cstring>
#include <map>

uint32_t* computeIncreasing(uint32_t* data, uint32_t size)
{
    std::map<uint32_t,uint32_t> container;


    for (uint32_t i = 0; i < size; ++i) {
        container.insert(std::pair<uint32_t,uint32_t>(data[i], 0));
    }

    std::map<uint32_t,uint32_t>::iterator it = container.begin();
    for (uint32_t i = 0; it != container.end(); ++it, ++i) {
        it->second = i;
    }

    uint32_t* order = new uint32_t[size];
    for (int i = 0; i < size; ++i)
        order[i] = container.find(data[i])->second;

    return order;
}

uint32_t* computeDecreasing(uint32_t* data, uint32_t size)
{
    std::map<uint32_t,uint32_t> container;

    for (uint32_t i = 0; i < size; ++i)
        container.insert(std::pair<uint32_t,uint32_t>(data[i], 0));

    std::map<uint32_t,uint32_t>::iterator it = container.begin();
    uint32_t distinct = container.size();
    for (uint32_t i = 0; it != container.end(); ++it, ++i)
        it->second = distinct - i - 1;

    uint32_t* order = new uint32_t[size];
    for (int i = 0; i < size; ++i)
        order[i] = container.find(data[i])->second;

    return order;
}

uint32_t* readInput(const char* path_in, uint32_t* size)
{
    FILE* file_in = fopen(path_in, "rb");
    fread(size, sizeof(uint32_t), 1, file_in);
    uint32_t* data = new uint32_t[*size];
    fread(data, sizeof(uint32_t), *size, file_in);
    fclose(file_in);
    return data;
}

void writeOutput(const char* path_out, uint32_t* data, uint32_t size)
{
    FILE* file_out = fopen(path_out, "wb");
    fwrite(&size, sizeof(uint32_t), 1, file_out);
    fwrite(data, sizeof(uint32_t), size, file_out);
    fclose(file_out);
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("usage: %s file_in file_out order\n", argv[0]);
        return 0;
    }
    
    printf("Reading data... ");
    uint32_t size;
    uint32_t* data = readInput(argv[1], &size);
    printf("Done.\n");

    printf("Sorting... ");
    fflush(stdout);
    uint32_t* answer;
    if (!std::strcmp(argv[3], "inc"))
        answer = computeIncreasing(data, size);

    else if (!std::strcmp(argv[3], "dec"))
        answer = computeDecreasing(data, size);
    
    else {
        delete[] data;
        printf("order has to be 'inc' or 'dec'\n");
        return 0;
    }
    printf("Done.\n");

    printf("Writing output... ");
    writeOutput(argv[2], answer, size);
    printf("Done.\n");

    delete[] answer;
    return 0;
}
