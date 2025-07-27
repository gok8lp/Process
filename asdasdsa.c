#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
/*
int main(){
	int pid;
	pid = fork();

	if(pid > 0){
		//parent
		wait(NULL);
		printf("parent: %d benim pid: %d\n",pid,getpid());
	}
	if(pid == 0){
		//child
		printf("child: %d benim pid: %d\n",pid,getpid());
	}
	if(pid < 0){
		//error
	}

}
*/
/*
int main(){
	char giris[100];
	//First open write folde mode
	FILE *ptrm;
	ptrm = fopen("/home/gokalp/Documents/Eclipse/ttt/a.txt", "w");
	if(ptrm == NULL){
		printf("Folder didn't open\n");
		return 1;
	}

	//get user enter
	printf("Write anythink");
	fgets(giris, sizeof(giris), stdin);

	//write to folder
	fprintf(ptrm, "%s", giris);
	fclose(ptrm);

	//open folder again for read
	ptrm = fopen("/home/gokalp/Documents/Eclipse/ttt/a.txt", "r");
	if(ptrm == NULL){
		printf("Folder didn't open\n");
		return 1;
	}

	char write;
	while((write = fgetc(ptrm)) != EOF){
		putchar(write);
	}

	fclose(ptrm);
}
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  // wait için ekledim

// Yaprak süreç: rastgele sayı üretip kendi pid dosyasına yazıyor
void process_leaf(){
    srand(getpid()); // Farklı seed için süreç pid'sini kullan
    int r = rand() % 100; // 0-99 arası rastgele sayı
    char filename[20];
    sprintf(filename, "%d.txt", getpid()); // Dosya adı: <pid>.txt
    printf("r num gen :%d by %d\n", r, getpid());
    fflush(stdout);

    FILE *fp = fopen(filename, "w"); // Dosyayı yazma modunda aç
    fprintf(fp, "%d", r);            // Rastgele sayıyı dosyaya yaz
    fclose(fp);                     // Dosyayı kapat
}

// Verilen pid'ye ait dosyadan sayıyı okuyup döndürür
int read_file(int pid){
    wait(NULL); // Çocuk sürecin bitmesini bekle (dosya hazır olacak)
    char filename[20];
    int r;
    sprintf(filename, "%d.txt", pid); // Dosya adı oluştur
    FILE *fp = fopen(filename, "r");  // Dosyayı okuma modunda aç
    fscanf(fp, "%d", &r);             // Dosyadan sayıyı oku
    fclose(fp);                      // Dosyayı kapat
    return r;                       // Okunan değeri döndür
}

// Parent süreç çocukların dosyalarını okuyup toplam yapar, sonucu kendi dosyasına yazar
int parent_process(int pidleft, int pidright){
    int lvalue = read_file(pidleft);   // Sol çocuktan oku
    int rvalue = read_file(pidright);  // Sağ çocuktan oku
    char filename[20];
    sprintf(filename, "%d.txt", getpid()); // Kendi pid dosyasını oluştur
    FILE *fp = fopen(filename, "w");        // Yazma modunda aç
    fprintf(fp, "%d", (lvalue + rvalue));   // Toplamı dosyaya yaz
    printf("parent sum val: %d from %d\n", lvalue + rvalue, getpid());
    fclose(fp);                            // Dosyayı kapat
    return lvalue + rvalue;                // Toplamı döndür
}

// Süreç ağacı fonksiyonu (yükseklik kadar süreç oluşturur)
void tree(int height){
    if(height <= 0) {
        // Eğer yükseklik 0 veya altıysa, yaprak süreç oluştur
        process_leaf();
        return;
    }

    int pid = fork(); // Sol çocuk için yeni süreç oluştur
    if(pid > 0){ // Parent süreç
        int pid2 = fork(); // Sağ çocuk için yeni süreç oluştur
        if(pid2 > 0){ // Parent hâlâ buradayız
            printf(" parent: %d , lchild:%d, rchild:%d, height:%d\n", getpid(), pid, pid2, height);
            wait(NULL); // Çocuk süreçlerin bitmesini bekle
            // Çocukların dosyalarını okuyup toplama işlemi yap
            int result = parent_process(pid, pid2);
            if(height == 3)
                printf(" RESULT : %d\n", result); // En üstte sonucu yazdır
        }
        if(pid2 == 0){ // Sağ çocuk süreci burası
            tree(--height); // Yüksekliği azaltıp recursive çağrı
        }
    }
    if(pid == 0){ // Sol çocuk süreci burası
        tree(--height); // Yüksekliği azaltıp recursive çağrı
    }
}

int main(){
    tree(3); // 3 seviyeli süreç ağacı başlat
    return 0;
}
*/
void process_leaf(void){
	srand(getpid());
	int r = rand() % 100;
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	printf("r random num crate %d from %d", r, getpid());
	fflush(stdout);

	FILE *ptr = fopen(filename, "w");
	fprintf(ptr, "%d", r);
	fclose(ptr);
}

int read_file(int pid){
	srand(getpid());
	int r = rand() % 100;
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	FILE *ptr = fopen(filename, "r");
	fscanf(ptr, "%d", &r);
	fclose(ptr);
	return r;
}

int parent_process(int pidleft, int pidright){
	int lvalue = read_file(pidleft);
	int rvalue = read_file(pidright);
	char filename[20];
	sprintf(filename, "%d.txt", getpid());
	FILE *ptr = fopen(filename, "w");
	fprintf(ptr, "%d", (lvalue + rvalue));
	printf("parent sum val: %d from %d\n", lvalue + rvalue, getpid());
	fclose(ptr);
	return rvalue+lvalue;
}

void tree(int height){
	if(height < 0){
		process_leaf();
		return;
	}

	int pid = fork();

	if(pid > 0){
		int pid2 = fork();
		if(pid2 > 0){
			printf("parent: %d, lchild: %d, rchild: %d, height:%d\n", getpid(), pid, pid2, height);
			wait(NULL);
			int result = parent_process(pid, pid2);
			if(height == 3){
				printf("RESULT: %d\n", result);
			}
		}
		if(pid2 == 0){
			tree(--height);
		}
	}
	if(pid == 0){
		tree(--height);
	}
}

int main(){
	tree(3);
}
