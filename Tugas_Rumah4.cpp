#include <iostream>
#include <string>
#include <vector>
#include <cmath>  // Untuk fmod pada double
#include <iomanip> // Untuk std::fixed dan std::setprecision

// Struktur dasar untuk Aset, yang akan di-extend oleh Saham dan Obligasi
struct Aset {
    std::string nama;   // Nama aset
    std::string jenis;  // Jenis aset (Saham atau Obligasi)
    int jumlah;         // Jumlah unit aset
    double harga;       // Harga per unit aset

    Aset(std::string nama, std::string jenis, int jumlah, double harga)
        : nama(nama), jenis(jenis), jumlah(jumlah), harga(harga) {}

    virtual void nextYear() = 0;
    virtual std::string toString() const = 0;
};

// Struktur untuk Saham, mewarisi dari Aset
struct Saham : public Aset {
    double dividen;
    double pertumbuhan;

    Saham(std::string nama, int jumlah, double harga, double pertumbuhan, double dividen)
        : Aset(nama, "SAHAM", jumlah, harga), pertumbuhan(pertumbuhan), dividen(dividen) {}

    void nextYear() override {
        grow();
        harga += harga * pertumbuhan;
    }

    void grow() {
        int a = 0x4b; 
        int c = 0x4a; 
        int m = 2;    

        pertumbuhan = fmod(a * pertumbuhan + c, m) - 1;
        pertumbuhan = pertumbuhan < 0 ? fmod(pertumbuhan, -m) : pertumbuhan;
    }

    std::string toString() const override {
        return "- " + nama + "\nTipe: Saham\nHarga: " + std::to_string(harga) + 
               "\nJumlah: " + std::to_string(jumlah) + 
               "\nDividen: " + std::to_string(dividen) + 
               "\nPertumbuhan: " + std::to_string(pertumbuhan);
    }
};

// Struktur untuk Obligasi, mewarisi dari Aset
struct Obligasi : public Aset {
    double bunga;
    const double fixBunga;
    int tahunJatuhTempo;
    double hargaMulai;

    Obligasi(std::string nama, int jumlah, double harga, double bunga, int maturitas)
        : Aset(nama, "OBLIGASI", jumlah, harga), hargaMulai(harga), fixBunga(harga * bunga), bunga(bunga), tahunJatuhTempo(maturitas + 1) {}

    void nextYear() override {
        if (tahunJatuhTempo == 0) return;
        tahunJatuhTempo--;
        harga += fixBunga;
    }

    bool isJatuhTempo() const {
        return tahunJatuhTempo <= 0;
    }

    std::string toString() const override {
        return "- " + nama + "\nTipe: Obligasi\nHarga: " + std::to_string(harga) + 
               "\nJumlah: " + std::to_string(jumlah) + 
               "\nBunga: " + std::to_string(bunga) + 
               "\nJatuh Tempo: " + (isJatuhTempo() ? "true" : "false");
    }
};

// Node untuk linked list yang menyimpan Aset
struct Node {
    Aset* data;
    Node* next;

    Node(Aset* data) : data(data), next(nullptr) {}
};

// Kelas utama untuk manajemen portofolio
class Pacilnomo {
private:
    Node* head;
    double earnings;

    void printSeparator() const {
        std::cout << std::string(64, '=') << std::endl;
    }

    void daftarAset() const {
        printSeparator();
        Node* current = head;
        int count = 0;
        while (current != nullptr) {
            std::cout << current->data->toString() << std::endl;
            current = current->next;
            count++;
        }
        std::cout << "Kamu memiliki " << count << " total aset:" << std::endl;
        printSeparator();
    }

    void infoPortofolio() const {
        int jumlahSaham = 0, jumlahObligasi = 0;
        double netWorth = 0;
        Node* current = head;

        while (current != nullptr) {
            if (current->data->jenis == "SAHAM") jumlahSaham++;
            else if (current->data->jenis == "OBLIGASI") jumlahObligasi++;
            netWorth += current->data->harga * current->data->jumlah;
            current = current->next;
        }

        printSeparator();
        std::cout << "Info Portofolio\n"
                  << "Jumlah Jenis Saham: " << jumlahSaham << "\n"
                  << "Jumlah Jenis Obligasi: " << jumlahObligasi << "\n"
                  << "Total Nilai Portofolio: " << std::fixed << std::setprecision(2) << netWorth << std::endl;
        printSeparator();
    }

    void nextYear() {
        Node* current = head;
        while (current != nullptr) {
            current->data->nextYear();
            current = current->next;
        }
    }

public:
    Pacilnomo() : head(nullptr), earnings(0) {}

    void addAset(Aset* aset) {
        Node* newNode = new Node(aset);
        newNode->next = head;
        head = newNode;
    }

    void run() {
        int banyakAset;
        std::cout << "Silakan masukkan banyak aset yang tersedia: ";
        std::cin >> banyakAset;
        std::cin.ignore();

        for (int i = 0; i < banyakAset; i++) {
            std::cout << "Aset " << i + 1 << ": ";
            std::string namaAset, jenisAset;
            int jumlah;
            double harga;

            std::cin >> namaAset >> jenisAset >> jumlah >> harga;

            if (jenisAset == "SAHAM") {
                double dividen, pertumbuhan;
                std::cin >> pertumbuhan >> dividen;
                addAset(new Saham(namaAset, jumlah, harga, pertumbuhan, dividen));
            } else if (jenisAset == "OBLIGASI") {
                double bunga;
                int tahunJatuhTempo;
                std::cin >> bunga >> tahunJatuhTempo;
                addAset(new Obligasi(namaAset, jumlah, harga, bunga, tahunJatuhTempo));
            }
        }

        std::cout << "Selamat datang di...\n"
                  << " /$$$$$$$                     /$$ /$$\n"
                  << "| $$__  $$                   |__/| $$\n"
                  << "| $$  \\ $$ /$$$$$$   /$$$$$$$ /$$| $$ /$$$$$$$   /$$$$$$  /$$$$$$/$$$$   /$$$$$$\n"
                  << "| $$$$$$$/|____  $$ /$$_____/| $$| $$| $$__  $$ /$$__  $$| $$_  $$_  $$ /$$__  $$\n"
                  << "| $$____/  /$$$$$$$| $$      | $$| $$| $$  \\ $$| $$  \\ $$| $$ \\ $$ \\ $$| $$  \\ $$\n"
                  << "| $$      /$$__  $$| $$      | $$| $$| $$  | $$| $$  | $$| $$ | $$ | $$| $$  | $$\n"
                  << "| $$     |  $$$$$$$|  $$$$$$$| $$| $$| $$  | $$|  $$$$$$/| $$ | $$ | $$|  $$$$$$/\n"
                  << "|__/      \\_______/ \\_______/|__/|__/|__/  |__/ \\______/ |__/ |__/ |__/ \\______/\n\n";

        while (true) {
            std::cout << "Silakan pilih salah satu opsi berikut:\n"
                      << "[1] Daftar aset\n"
                      << "[2] Info portofolio\n"
                      << "[3] Lanjut ke tahun berikutnya\n"
                      << "[*] Keluar\n";
            printSeparator();
            std::string pilihan;
            std::cin >> pilihan;
            if (pilihan == "1") {
                daftarAset();
            } else if (pilihan == "2") {
                infoPortofolio();
            } else if (pilihan == "3") {
                nextYear();
                std::cout << "Setahun telah berlalu...\n";
                printSeparator();
            } else {
                std::cout << "Terima kasih telah menggunakan layanan Pacilnomo ~ !\n";
                break;
            }
        }
    }
};

int main() {
    Pacilnomo pacilnomo;
    pacilnomo.run();
    return 0;
}
