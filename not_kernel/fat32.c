#include <string.h>
#include "fat32.h"
#include "../drivers/disk.h"
#include "../kernel/memory.h"

static uint32_t fat32_get_fat_entry(fat32_fs_t* fs, uint32_t cluster){
    if(cluster >= (fs->boot_sector.total_sectors_32) / fs->boot_sector.sectors_per_cluster) return FAT32_CLUSTER_EOC_MAX;

    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fs->fat_start_sector + fat_offset / fs->boot_sector.bytes_per_sector;
    uint32_t sector_offset = fat_offset % fs->boot_sector.bytes_per_sector;

    if(!fs->fat_cache){
        fs->fat_cache = malloc(fs->boot_sector.bytes_per_sector);
        if (!fs->fat_cache) return FAT32_CLUSTER_EOC_MAX;
    }

    if(ATA_read_sectors(fs->partition.drive, fat_sector, 1, fs->fat_cache) != 1) return FAT32_CLUSTER_EOC_MAX;

    uint32_t entry = *(uint32_t*)((uint8_t*)fs->fat_cache + sector_offset);
    return entry & 0x0FFFFFFF;
}

static uint32_t fat32_cluster_to_sector(fat32_fs_t* fs, size_t cluster){
    if(cluster < 2) return INVALID_SECTOR;

    return fs->data_start_sector + (cluster - 2) * (fs->boot_sector.sectors_per_cluster);
}


int8_t fat32_mount(partition_t* partition, fat32_fs_t* fs){
    if(memset(fs, 0, sizeof(fat32_fs_t)) == nullptr) return -1;
    if(ATA_read_sectors(partition->drive, partition->start_sector, 1, &fs->boot_sector) != 0) return -2;
    if(fs->boot_sector.boot_signature != FAT32_SIGNATURE) return -3;

    fs->fat_start_sector = partition->start_sector + fs->boot_sector.reserved_sectors;
    fs->data_start_sector = partition->start_sector + (fs->boot_sector.sectors_per_fat_32 * fs->boot_sector.fat_count);
    fs->cluster_size_bytes = fs->boot_sector.sectors_per_cluster * fs->boot_sector.bytes_per_sector;

    return 0;
}

void fat32_unmount(fat32_fs_t* fs){
    free(fs);
    fs = nullptr;
}

static int8_t fat32_read_cluster(fat32_fs_t* fs, size_t cluster, void* buffer){
    size_t start_sector = fat32_cluster_to_sector(fs, cluster);
    if(start_sector == INVALID_SECTOR){
        buffer == nullptr;
    }

    buffer = malloc(fs->cluster_size_bytes);
    if(ATA_read_sectors(fs->partition.drive, start_sector, fs->boot_sector.sectors_per_cluster, buffer) != 0){
        free(buffer);
        buffer = nullptr;
        return -2;
    }

    return 0;
}

static size_t* fat32_get_cluster_list(size_t start_cluster, fat32_fs_t* fs){
    if(start_cluster < 2) return NULL;

    size_t current_cluster = start_cluster;
    size_t next_cluster;

    size_t* cluster_chain;

    for(size_t i = 0; ; i++){
        if(current_cluster == FAT32_CLUSTER_EOC_MIN || current_cluster == FAT32_CLUSTER_EOC_MAX) break;

        if(current_cluster == FAT32_CLUSTER_BAD || current_cluster == FAT32_CLUSTER_FREE){
            cluster_chain = realloc(cluster_chain, 1);
            cluster_chain[0] =  current_cluster;
            break;
        }

        if (i % 8 == 0){
            cluster_chain = realloc(cluster_chain, 8*(i+1));
        }

        cluster_chain[i] = current_cluster;
        current_cluster = fat32_get_fat_entry(fs, current_cluster);
    }

    return cluster_chain;
}

static int8_t fat32_construct_file(size_t start_cluster, fat32_fs_t* fs, void* buffer){
    size_t* cluster_chain = fat32_get_cluster_list(start_cluster, fs);

    if(cluster_chain[0] == FAT32_CLUSTER_BAD || cluster_chain[0] == FAT32_CLUSTER_FREE) return nullptr;

    buffer = nullptr;

    for(size_t cluster_number = 0; !(cluster_chain[cluster_number] == FAT32_CLUSTER_EOC_MAX || cluster_chain[cluster_number] == FAT32_CLUSTER_EOC_MIN); cluster_number ++){
        void* buffer = realloc(buffer, fs->cluster_size_bytes * cluster_number);

        size_t current_cluster = cluster_chain[cluster_number];

        if(fat32_read_cluster(fs, current_cluster, buffer + (fs->cluster_size_bytes * cluster_number)) != 0){
            free(buffer);
            return -1;
        }
    }

    return 0;
}

