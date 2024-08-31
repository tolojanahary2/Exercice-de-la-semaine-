#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int a, b, c, d, sr;
} Adresse;

void scanf_ip(char *ip, Adresse *p);

int main() { 
    Adresse p;
    char ip[20];
    scanf_ip(ip, &p);
    return 0;   
}

void scanf_ip(char *ip, Adresse *p) { 
   
  
    p->a = -1; p->b = -1; p->c = -1; p->d = -1; p->sr = -1;
    
    do {
        printf("Entrer l'adresse IP (ex: 192.168.1.1/24) :\n");
        scanf("%s", ip);
        sscanf(ip, "%d.%d.%d.%d/%d", &p->a, &p->b, &p->c, &p->d, &p->sr);
    } while (p->a < 0 || p->a > 255 || p->b < 0 || p->b > 255 || p->c < 0 || p->c > 255 || p->d < 0 || p->d > 255 || p->sr < 0 || p->sr > 32);

    printf("-> SR: %d\n", p->sr);

    if (p->a < 0 || p->a > 255 || p->b < 0 || p->b > 255 || p->c < 0 || p->c > 255 || p->d < 0 || p->d > 255) {
        printf("Erreur IP\n");
    } else {
        // Afficher les informations de l'adresse IP
        printf("Vérification de l'IP : %d.%d.%d.%d\n", p->a, p->b, p->c, p->d);

        // Calculer l'adresse du réseau et l'adresse de diffusion
        int subnet_mask = ~((1 << (32 - p->sr)) - 1); // Masque de sous-réseau
        int ip_num = (p->a << 24) | (p->b << 16) | (p->c << 8) | p->d;
        int network_addr = ip_num & subnet_mask;
        int broadcast_addr = network_addr | ~subnet_mask;
        
        int num_hosts = pow(2, 32 - p->sr) - 2; // Nombre de machines (hôtes) dans le sous-réseau

        // Afficher la classe d'adresse IP
        printf("Classe d'adresse IP : ");
        if (p->a >= 0 && p->a < 128) {
            printf("A\n");
            printf("Adresse du réseau : %d.0.0.0\n", p->a);
            printf("Adresse de diffusion : %d.255.255.255\n", p->a);
        } else if (p->a >= 128 && p->a < 192) {
            printf("B\n");
            printf("Adresse du réseau : %d.%d.0.0\n", p->a, p->b);
            printf("Adresse de diffusion : %d.%d.255.255\n", p->a, p->b);
        } else if (p->a >= 192 && p->a < 224) {
            printf("C\n");
            printf("Adresse du réseau : %d.%d.%d.0\n", p->a, p->b, p->c);
            printf("Adresse de diffusion : %d.%d.%d.255\n", p->a, p->b, p->c);
        } else {
            printf("Inconnu\n");
        }

        // Afficher l'adresse de réseau et l'adresse de diffusion
        printf("Adresse réseau : %d.%d.%d.%d\n", 
            (network_addr >> 24) & 0xFF,
            (network_addr >> 16) & 0xFF,
            (network_addr >> 8) & 0xFF,
            network_addr & 0xFF);

        printf("Adresse de diffusion : %d.%d.%d.%d\n", 
            (broadcast_addr >> 24) & 0xFF,
            (broadcast_addr >> 16) & 0xFF,
            (broadcast_addr >> 8) & 0xFF,
            broadcast_addr & 0xFF);

        printf("Nombre de machines : %d\n", num_hosts);
    }
}
