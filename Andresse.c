#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour convertir une chaîne IP en un entier 32 bits
unsigned long ip_to_ulong(const char *ip) {
    unsigned long result = 0;
    int octet;
    char ip_copy[16];
    strncpy(ip_copy, ip, sizeof(ip_copy));
    char *token = strtok(ip_copy, ".");

    for (int i = 3; i >= 0 && token != NULL; i--) {
        octet = atoi(token);
        result |= (octet << (i * 8));
        token = strtok(NULL, ".");
    }
    return result;
}

// Fonction pour convertir un entier 32 bits en une chaîne IP
void ulong_to_ip(unsigned long ip, char *buffer) {
    sprintf(buffer, "%lu.%lu.%lu.%lu",
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            ip & 0xFF);
}

int main() {
    char ip_str[16];
    int num_subnets;

    printf("Entrez l'adresse IP de base (ex: 192.168.1.0): ");
    scanf("%15s", ip_str);
    printf("Entrez le nombre de sous-réseaux à découper: ");
    scanf("%d", &num_subnets);

    if (num_subnets < 1 || num_subnets > 256) { // Limité à 256 pour simplification
        printf("Le nombre de sous-réseaux doit être entre 1 et 256.\n");
        return 1;
    }

    unsigned long base_ip = ip_to_ulong(ip_str);

    // Calcul du nombre de bits supplémentaires pour les sous-réseaux
    int subnet_bits = 0;
    int subnet_size = 1;

    while (subnet_size < num_subnets) {
        subnet_bits++;
        subnet_size *= 2;
    }

    // Nombre d'adresses par sous-réseau
    int addresses_per_subnet = 1 << (32 - (24 + subnet_bits)); // 24 bits pour l'adresse de base + bits de sous-réseau

    printf("Nombre d'adresses par sous-réseau: %d\n", addresses_per_subnet);

    for (int i = 0; i < num_subnets; i++) {
        unsigned long subnet_base = base_ip + (i * addresses_per_subnet);
        unsigned long network_addr = subnet_base;
        unsigned long broadcast_addr = network_addr + addresses_per_subnet - 1;

        char network_addr_str[16];
        char broadcast_addr_str[16];

        ulong_to_ip(network_addr, network_addr_str);
        ulong_to_ip(broadcast_addr, broadcast_addr_str);

        // Nombre de machines utilisables (adresses de 1 à l'avant-dernière adresse)
        int usable_machines = (addresses_per_subnet - 2); // Exclut l'adresse réseau et l'adresse de diffusion

        printf("\nSous-réseau %d:\n", i + 1);
        printf("  Adresse réseau: %s\n", network_addr_str);
        printf("  Adresse de broadcast: %s\n", broadcast_addr_str);
        printf("  Nombre de machines utilisables: %d\n", usable_machines);

        // Affichage des adresses des machines
        printf("  Adresses des machines:\n");
        for (unsigned long addr = network_addr + 1; addr < broadcast_addr; ++addr) {
            char addr_str[16];
            ulong_to_ip(addr, addr_str);
            printf("    %s\n", addr_str);
        }
    }

    return 0;
}
