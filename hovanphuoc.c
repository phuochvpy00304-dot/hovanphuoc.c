#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

#define MAX_SL 100
#define FILE_AUTO "hethong.dat"
#define FILE_AUTO_CSV "hethong.csv"

// ===================== 1. CAU TRUC DU LIEU =====================
typedef struct {
    int ngay;
    int thang;
    int nam;
} Date;

typedef enum {
    NV_BINH_THUONG = 1,
    TRUONG_PHONG = 2,
    NV_KINH_DOANH = 3
} LoaiNV;

typedef enum {
    GT_NAM = 1,
    GT_NU = 2
} GioiTinh;

typedef struct {
    int msnv;
    char ho[30];
    char ten[30];
    GioiTinh gioiTinh;
    Date namsinh;
    char noisinh[100];
    char diachi[200];
    Date ngayvao;
    char phongBan[50];
    LoaiNV loai;
    float luongCoBan;
    union {
        struct {
            float phuCapCV;
        } tp;
        struct {
            float doanhSo;
            float tiLeHoaHong;
            float tienHoaHong;
        } kd;
    } rieng;
    float tongLuong;
} NhanVien;

typedef struct {
    NhanVien a[MAX_SL];
    int n;
} DanhSach;

// ===================== 2. NGUYEN MAU HAM =====================
void xoaXuongDong(char* str);
int nhapSoNguyen(const char* msg);
float nhapSoThuc(const char* msg);
float nhapSoThucDuong(const char* msg);
void nhapChuoi(const char* msg, char* str, int size);

// Date functions
int laNamNhuan(int nam);
int kiemTraNgayHopLe(int d, int m, int y);
void nhapNgay(Date* d, const char* msg);
Date layNgayHienTai(void);
int tinhTuoiTaiNgay(Date ngaySinh, Date ngayTinh);
int tinhThamNien(Date ngayVao);
float tinhPhuCapThamNien(int thamNien);
int layNamHienTai(void);

// Logic
int timMaxID(DanhSach ds);
void tinhTongLuong(NhanVien* nv);
int nhap1NV(NhanVien* nv, int idMoi);
void themMoiNV(DanhSach* ds);
void xuat1NV(NhanVien nv);
void xuatDanhSach(DanhSach ds);
int timMSNVTuanTu(DanhSach ds, int msnv);
void timHoTenTuanTu(DanhSach ds);
int timMSNVNhiPhan(DanhSach ds, int msnv);
void sapXepChonTheoMSNV(DanhSach* ds);
void sapXepChonTheoTen(DanhSach* ds);
void sapXepNoiBotTheoLuong(DanhSach* ds);
void formatTien(long long value, char* out, int size);
int demKyTuHienThi(const char* s);
void inCot(const char* str, int width);

// Features
void timKiemNangCao(DanhSach ds);
void timKiemTheoKhoang(DanhSach ds);
void baoCaoThamNien(DanhSach ds);
void thongKeLuongCaoNhat(DanhSach ds);
void thongKeTongHop(DanhSach ds);
void xuatThongKeCSV(DanhSach ds, const char* filename);

// File
void luuFile(DanhSach ds, const char* filename, char* mode);
void docFile(DanhSach* ds, const char* filename);
void luuFileCSV(DanhSach ds, const char* filename);
void docFileCSV(DanhSach* ds, const char* filename);
int coDuoiCSV(const char* s);
int laChuoiNgay(const char* s);
const char* chuGioiTinh(GioiTinh gt);
GioiTinh gioiTinhTuChuoi(const char* s);
void saoLuuKhoiPhuc(DanhSach* ds);
void menuTimKiem(DanhSach ds);
void menuSapXep(DanhSach* ds);
void menu();

// ===================== 3. MAIN =====================
int main() {
    setlocale(LC_ALL, "");
    menu();
    return 0;
}

// ===================== 4. TRIEN KHAI =====================
void xoaXuongDong(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

void nhapChuoi(const char* msg, char* str, int size) {
    printf("%s", msg);
    fgets(str, size, stdin);
    xoaXuongDong(str);
}

int nhapSoNguyen(const char* msg) {
    char buffer[100];
    char* endPtr;
    while (1) {
        printf("%s", msg);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            xoaXuongDong(buffer);
            if (strlen(buffer) == 0) continue;
            int val = (int)strtol(buffer, &endPtr, 10);
            if (*endPtr == '\0') return val;
        }
    }
}

float nhapSoThuc(const char* msg) {
    char buffer[100];
    char* endPtr;
    while (1) {
        printf("%s", msg);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            xoaXuongDong(buffer);
            if (strlen(buffer) == 0) continue;
            float val = strtof(buffer, &endPtr);
            if (*endPtr == '\0') return val;
        }
    }
}

float nhapSoThucDuong(const char* msg) {
    float val;
    do {
        val = nhapSoThuc(msg);
        if (val <= 0) printf("   >> Loi: Gia tri phai > 0!\n");
    } while (val <= 0);
    return val;
}

// --- DATE VALIDATION ---
int laNamNhuan(int nam) {
    return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}

int kiemTraNgayHopLe(int d, int m, int y) {
    if (y < 1900 || y > 2100) return 0;
    if (m < 1 || m > 12) return 0;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (laNamNhuan(y)) daysInMonth[2] = 29;
    if (d < 1 || d > daysInMonth[m]) return 0;
    return 1;
}

void nhapNgay(Date* d, const char* msg) {
    printf("%s (dd mm yyyy): ", msg);
    char buffer[100];
    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d %d %d", &d->ngay, &d->thang, &d->nam) == 3) {
            if (kiemTraNgayHopLe(d->ngay, d->thang, d->nam)) return;
        }
        printf("   >> Ngay khong ton tai! Nhap lai (vi du: 29 02 2024): ");
    }
}

Date layNgayHienTai(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Date d;
    d.ngay = tm.tm_mday;
    d.thang = tm.tm_mon + 1;
    d.nam = tm.tm_year + 1900;
    return d;
}

int tinhTuoiTaiNgay(Date ngaySinh, Date ngayTinh) {
    int age = ngayTinh.nam - ngaySinh.nam;
    if (ngayTinh.thang < ngaySinh.thang ||
        (ngayTinh.thang == ngaySinh.thang && ngayTinh.ngay < ngaySinh.ngay)) {
        age--;
    }
    return age;
}

int tinhThamNien(Date ngayVao) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int namHienTai = tm.tm_year + 1900;
    return namHienTai - ngayVao.nam;
}

int layNamHienTai(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

float tinhPhuCapThamNien(int thamNien) {
    if (thamNien >= 10) return 2000000;
    if (thamNien >= 5) return 1000000;
    return 0;
}

// --- LOGIC ---
int timMaxID(DanhSach ds) {
    int max = 0;
    for(int i=0; i<ds.n; i++) {
        if(ds.a[i].msnv > max) max = ds.a[i].msnv;
    }
    return max;
}

void tinhTongLuong(NhanVien* nv) {
    float phuCapCV = 0;
    float tienHoaHong = 0;
    int thamNien = tinhThamNien(nv->ngayvao);
    float phuCapThamNien = tinhPhuCapThamNien(thamNien);
    
    if (nv->loai == TRUONG_PHONG) {
        phuCapCV = nv->rieng.tp.phuCapCV;
    } else if (nv->loai == NV_KINH_DOANH) {
        nv->rieng.kd.tienHoaHong = nv->rieng.kd.doanhSo * (nv->rieng.kd.tiLeHoaHong / 100.0);
        tienHoaHong = nv->rieng.kd.tienHoaHong;
    }
    
    nv->tongLuong = nv->luongCoBan + phuCapCV + phuCapThamNien + tienHoaHong;
}

int nhap1NV(NhanVien* nv, int idMoi) {
    nv->msnv = idMoi;
    printf("   >> Ma NV (Tu dong): %d\n", nv->msnv);
    
    nhapChuoi("- Ho dem: ", nv->ho, sizeof(nv->ho));
    nhapChuoi("- Ten: ", nv->ten, sizeof(nv->ten));
    
    printf("- Gioi tinh (1: Nam | 2: Nu): ");
    int gt;
    do {
        gt = nhapSoNguyen("");
    } while (gt < 1 || gt > 2);
    nv->gioiTinh = (GioiTinh)gt;
    
    Date homNay = layNgayHienTai();
    while (1) {
        nhapNgay(&nv->namsinh, "- Ngay sinh");
        int tuoi = tinhTuoiTaiNgay(nv->namsinh, homNay);
        if (tuoi >= 18) break;
        printf(">> Nhan vien chua du 18 tuoi, khong the nhan viec.\n");
        printf("1. Nhap lai ngay sinh\n2. Huy them nhan vien\nChon: ");
        int chon = nhapSoNguyen("");
        if (chon == 2) {
            memset(nv, 0, sizeof(NhanVien));
            return 0;
        }
    }
    
    nhapChuoi("- Noi sinh: ", nv->noisinh, sizeof(nv->noisinh));
    nhapChuoi("- Dia chi: ", nv->diachi, sizeof(nv->diachi));
    nhapChuoi("- Phong ban: ", nv->phongBan, sizeof(nv->phongBan));
    
    while (1) {
        nhapNgay(&nv->ngayvao, "- Ngay vao lam");
        int tuoiVaoLam = tinhTuoiTaiNgay(nv->namsinh, nv->ngayvao);
        if (tuoiVaoLam >= 18) break;
        printf(">> Thoi diem vao lam nhan vien chua du 18 tuoi.\n");
    }
    
    printf("- Chuc vu (1: Thuong | 2: Truong phong | 3: Kinh doanh): ");
    int loai;
    do {
        loai = nhapSoNguyen("");
    } while (loai < 1 || loai > 3);
    nv->loai = (LoaiNV)loai;
    
    nv->luongCoBan = nhapSoThucDuong("- Luong co ban (>0): ");
    
    if (nv->loai == TRUONG_PHONG) {
        nv->rieng.tp.phuCapCV = nhapSoThucDuong("- Phu cap chuc vu: ");
    } else if (nv->loai == NV_KINH_DOANH) {
        nv->rieng.kd.doanhSo = nhapSoThuc("- Doanh so: ");
        nv->rieng.kd.tiLeHoaHong = nhapSoThuc("- Ty le hoa hong (%): ");
    }
    
    tinhTongLuong(nv);
    return 1;
}

void themMoiNV(DanhSach* ds) {
    if (ds->n >= MAX_SL) { printf(">> Danh sach day!\n"); return; }
    printf("\n--- THEM NHAN VIEN ---\n");
    int newID = timMaxID(*ds) + 1;
    if (!nhap1NV(&ds->a[ds->n], newID)) {
        printf(">> Da huy them nhan vien.\n");
        return;
    }
    ds->n++;
    printf(">> Them thanh cong!\n");
}

void formatTien(long long value, char* out, int size) {
    if (size <= 0) return;
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%lld", value);
    if (len <= 0) { out[0] = '\0'; return; }
    
    int outIdx = 0;
    for (int i = 0; i < len; i++) {
        if (outIdx >= size - 1) break;
        out[outIdx++] = buf[i];
        int remaining = len - i - 1;
        if (remaining > 0 && (remaining % 3 == 0)) {
            if (outIdx < size - 1) out[outIdx++] = '.';
        }
    }
    out[outIdx] = '\0';
}

int demKyTuHienThi(const char* s) {
    int len = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (((unsigned char)s[i] & 0xC0) != 0x80) len++;
        i++;
    }
    return len;
}

void inCot(const char* str, int width) {
    int lenBytes = (int)strlen(str);
    int lenVisual = demKyTuHienThi(str);
    int diff = lenBytes - lenVisual;
    printf("%-*s", width + diff, str);
}

void xuat1NV(NhanVien nv) {
    char chucVu[20];
    if (nv.loai == NV_BINH_THUONG) strcpy(chucVu, "Nhan vien");
    else if (nv.loai == TRUONG_PHONG) strcpy(chucVu, "Truong phong");
    else strcpy(chucVu, "Kinh doanh");
    
    char hoTen[70];
    snprintf(hoTen, sizeof(hoTen), "%s %s", nv.ho, nv.ten);
    
    int thamNien = tinhThamNien(nv.ngayvao);
    float pcThamNien = tinhPhuCapThamNien(thamNien);
    
    char nsinh[16];
    snprintf(nsinh, sizeof(nsinh), "%02d/%02d/%04d", nv.namsinh.ngay, nv.namsinh.thang, nv.namsinh.nam);
    
    char luongCB[20], phuCapCV[20], phuCapTN[20], doanhSo[20], tienHH[20], tongLuong[20];
    formatTien((long long)nv.luongCoBan, luongCB, sizeof(luongCB));
    formatTien((long long)nv.tongLuong, tongLuong, sizeof(tongLuong));
    formatTien((long long)pcThamNien, phuCapTN, sizeof(phuCapTN));
    
    char tiLeHH[16] = "";
    phuCapCV[0] = '\0';
    doanhSo[0] = '\0';
    tienHH[0] = '\0';
    
    if (nv.loai == TRUONG_PHONG) {
        formatTien((long long)nv.rieng.tp.phuCapCV, phuCapCV, sizeof(phuCapCV));
    } else if (nv.loai == NV_KINH_DOANH) {
        formatTien((long long)nv.rieng.kd.doanhSo, doanhSo, sizeof(doanhSo));
        snprintf(tiLeHH, sizeof(tiLeHH), "%.0f%%", nv.rieng.kd.tiLeHoaHong);
        formatTien((long long)nv.rieng.kd.tienHoaHong, tienHH, sizeof(tienHH));
    }
    
    printf("%-6d ", nv.msnv);
    inCot(hoTen, 24); printf(" ");
    printf("%-12.12s ", nsinh);
    inCot(nv.phongBan, 12); printf(" ");
    inCot(chucVu, 14); printf(" ");
    printf("%8d %12.12s %12.12s %12.12s %12.12s %8.8s %12.12s %12.12s\n",
           thamNien, luongCB, phuCapCV, phuCapTN, doanhSo, tiLeHH, tienHH, tongLuong);
}

void xuatDanhSach(DanhSach ds) {
    if (ds.n == 0) { printf("\n>> Danh sach trong!\n"); return; }
    
    printf("\n%-6s ", "MSNV");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-12s ", "Ngay sinh");
    inCot("Phong ban", 12); printf(" ");
    inCot("Chuc vu", 14); printf(" ");
    printf("%8s %12s %20s %12s %12s %8s %15s %16s\n",
           "Tham nien", "Luong CB", "PC chuc vu", "PC tham nien", "Doanh so", "TL HH", "Tien HH", "Tong luong");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < ds.n; i++) xuat1NV(ds.a[i]);
    
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void timHoTenTuanTu(DanhSach ds) {
    char hoTenCanTim[100];
    nhapChuoi("Nhap ho ten can tim: ", hoTenCanTim, sizeof(hoTenCanTim));
    
    printf("\nKET QUA:\n");
    int count = 0;
    for (int i = 0; i < ds.n; i++) {
        char hoTen[70];
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[i].ho, ds.a[i].ten);
        if (strstr(hoTen, hoTenCanTim)) {
            xuat1NV(ds.a[i]);
            count++;
        }
    }
    if (count == 0) printf("Khong tim thay!\n");
}

int timMSNVTuanTu(DanhSach ds, int msnv) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.a[i].msnv == msnv) return i;
    }
    return -1;
}

void sapXepChonTheoMSNV(DanhSach* ds) {
    for (int i = 0; i < ds->n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < ds->n; j++) {
            if (ds->a[j].msnv < ds->a[minIdx].msnv) minIdx = j;
        }
        if (minIdx != i) {
            NhanVien temp = ds->a[i];
            ds->a[i] = ds->a[minIdx];
            ds->a[minIdx] = temp;
        }
    }
}

int soSanhTenHo(const NhanVien* a, const NhanVien* b) {
    int cmp = strcmp(a->ten, b->ten);
    if (cmp != 0) return cmp;
    cmp = strcmp(a->ho, b->ho);
    if (cmp != 0) return cmp;
    if (a->msnv < b->msnv) return -1;
    if (a->msnv > b->msnv) return 1;
    return 0;
}

void sapXepChonTheoTen(DanhSach* ds) {
    for (int i = 0; i < ds->n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < ds->n; j++) {
            if (soSanhTenHo(&ds->a[j], &ds->a[minIdx]) < 0) minIdx = j;
        }
        if (minIdx != i) {
            NhanVien temp = ds->a[i];
            ds->a[i] = ds->a[minIdx];
            ds->a[minIdx] = temp;
        }
    }
}

int timMSNVNhiPhan(DanhSach ds, int msnv) {
    sapXepChonTheoMSNV(&ds);
    int left = 0;
    int right = ds.n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (ds.a[mid].msnv == msnv) return mid;
        if (ds.a[mid].msnv < msnv) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void sapXepNoiBotTheoLuong(DanhSach* ds) {
    for (int i = 0; i < ds->n - 1; i++) {
        for (int j = 0; j < ds->n - 1 - i; j++) {
            if (ds->a[j].tongLuong < ds->a[j + 1].tongLuong) {
                NhanVien temp = ds->a[j];
                ds->a[j] = ds->a[j + 1];
                ds->a[j + 1] = temp;
            }
        }
    }
}

void timKiemNangCao(DanhSach ds) {
    printf("\n--- TIM KIEM ---\n");
    printf("1. Ho\n2. Noi sinh\n3. Dia chi\n4. Nam sinh\n5. Nam vao lam\n");
    int chon = nhapSoNguyen("Chon (1-5): ");
    
    char chuoiCanTim[100] = "";
    int soCanTim = 0;
    
    if (chon >= 1 && chon <= 3) {
        nhapChuoi("Nhap noi dung can tim: ", chuoiCanTim, sizeof(chuoiCanTim));
    } else if (chon >= 4 && chon <= 5) {
        soCanTim = nhapSoNguyen("Nhap nam can tim: ");
    } else {
        printf(">> Lua chon khong hop le!\n");
        return;
    }
    
    printf("\nKET QUA:\n");
    int count = 0;
    for (int i = 0; i < ds.n; i++) {
        int match = 0;
        switch (chon) {
            case 1: if (strstr(ds.a[i].ho, chuoiCanTim)) match = 1; break;
            case 2: if (strstr(ds.a[i].noisinh, chuoiCanTim)) match = 1; break;
            case 3: if (strstr(ds.a[i].diachi, chuoiCanTim)) match = 1; break;
            case 4: if (ds.a[i].namsinh.nam == soCanTim) match = 1; break;
            case 5: if (ds.a[i].ngayvao.nam == soCanTim) match = 1; break;
        }
        if (match) { xuat1NV(ds.a[i]); count++; }
    }
    if (count == 0) printf("Khong tim thay!\n");
}

void timKiemTheoKhoang(DanhSach ds) {
    printf("\n--- TIM KIEM THEO KHOANG ---\n");
    printf("1. Tong luong\n2. Tuoi\n3. Tham nien\n");
    int chon = nhapSoNguyen("Chon: ");
    
    float minF = 0, maxF = 0;
    int minI = 0, maxI = 0;
    
    if (chon == 1) {
        minF = nhapSoThuc("Tu muc: "); maxF = nhapSoThuc("Den muc: ");
    } else {
        minI = nhapSoNguyen("Tu: "); maxI = nhapSoNguyen("Den: ");
    }
    
    printf("\nKET QUA:\n");
    int count = 0;
    int curYear = layNamHienTai();
    for (int i = 0; i < ds.n; i++) {
        int match = 0;
        if (chon == 1) {
            if (ds.a[i].tongLuong >= minF && ds.a[i].tongLuong <= maxF) match = 1;
        } else if (chon == 2) {
            int tuoi = curYear - ds.a[i].namsinh.nam;
            if (tuoi >= minI && tuoi <= maxI) match = 1;
        } else if (chon == 3) {
            int tn = tinhThamNien(ds.a[i].ngayvao);
            if (tn >= minI && tn <= maxI) match = 1;
        }
        if (match) { xuat1NV(ds.a[i]); count++; }
    }
    if (count == 0) printf("Khong tim thay!\n");
}

void thongKeLuongCaoNhat(DanhSach ds) {
    if (ds.n == 0) return;
    float max = -1;
    for (int i = 0; i < ds.n; i++) if (ds.a[i].tongLuong > max) max = ds.a[i].tongLuong;
    printf("\n--- LUONG CAO NHAT: %.0f ---\n", max);
    for (int i = 0; i < ds.n; i++) if (ds.a[i].tongLuong == max) xuat1NV(ds.a[i]);
}

void baoCaoThamNien(DanhSach ds) {
    printf("\n");
    inCot("Ho va ten", 24); printf(" ");
    inCot("Chuc vu", 16); printf(" ");
    inCot("Tham nien", 10); printf(" ");
    printf("%12s\n", "PC tham nien");
    printf("-----------------------------------------------------------------------\n");
    
    for (int i = 0; i < ds.n; i++) {
        int tn = tinhThamNien(ds.a[i].ngayvao);
        float pc = tinhPhuCapThamNien(tn);
        
        char hoTen[70];
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[i].ho, ds.a[i].ten);
        
        char chucVu[20];
        if (ds.a[i].loai == 1) strcpy(chucVu, "Nhan vien");
        else if (ds.a[i].loai == 2) strcpy(chucVu, "Truong phong");
        else strcpy(chucVu, "Kinh doanh");
        
        char tnStr[16];
        char pcStr[20];
        snprintf(tnStr, sizeof(tnStr), "%d nam", tn);
        formatTien((long long)pc, pcStr, sizeof(pcStr));
        
        inCot(hoTen, 24); printf(" ");
        inCot(chucVu, 16); printf(" ");
        inCot(tnStr, 10); printf(" ");
        printf("%12s\n", pcStr);
    }
}

void thongKeTongHop(DanhSach ds) {
    if (ds.n == 0) {
        printf("\n>> Danh sach trong!\n");
        return;
    }
    
    printf("\n--- THONG KE TONG HOP ---\n");
    printf("1. Xem tren man hinh\n2. Xuat CSV\nChon: ");
    int chon = nhapSoNguyen("");
    
    if (chon == 2) {
        char fname[50];
        nhapChuoi("Nhap ten file CSV: ", fname, sizeof(fname));
        if (!coDuoiCSV(fname)) {
            char full[50];
            snprintf(full, sizeof(full), "%s.csv", fname);
            strncpy(fname, full, sizeof(fname) - 1);
            fname[sizeof(fname) - 1] = '\0';
        }
        xuatThongKeCSV(ds, fname);
        return;
    }
    
    double sumLuong = 0;
    float maxLuong = ds.a[0].tongLuong;
    float minLuong = ds.a[0].tongLuong;
    int idxMaxLuong = 0;
    int idxMinLuong = 0;
    
    double sumPC = 0;
    float maxPC = -1;
    float minPC = -1;
    int idxMaxPC = -1;
    int idxMinPC = -1;
    
    double sumDoanhSo = 0;
    float maxDoanhSo = -1;
    float minDoanhSo = -1;
    int idxMaxDoanhSo = -1;
    int idxMinDoanhSo = -1;
    int countKD = 0;
    
    int curYear = layNamHienTai();
    int sumTuoi = 0;
    int maxTuoi = curYear - ds.a[0].namsinh.nam;
    int minTuoi = curYear - ds.a[0].namsinh.nam;
    int idxMaxTuoi = 0;
    int idxMinTuoi = 0;
    
    int countNam = 0;
    int countNu = 0;
    
    for (int i = 0; i < ds.n; i++) {
        NhanVien nv = ds.a[i];
        
        sumLuong += nv.tongLuong;
        if (nv.tongLuong > maxLuong) { maxLuong = nv.tongLuong; idxMaxLuong = i; }
        if (nv.tongLuong < minLuong) { minLuong = nv.tongLuong; idxMinLuong = i; }
        
        int tn = tinhThamNien(nv.ngayvao);
        float pc = tinhPhuCapThamNien(tn);
        if (nv.loai == TRUONG_PHONG) pc += nv.rieng.tp.phuCapCV;
        sumPC += pc;
        if (idxMaxPC == -1 || pc > maxPC) { maxPC = pc; idxMaxPC = i; }
        if (idxMinPC == -1 || pc < minPC) { minPC = pc; idxMinPC = i; }
        
        if (nv.loai == NV_KINH_DOANH) {
            float dsSo = nv.rieng.kd.doanhSo;
            sumDoanhSo += dsSo;
            if (idxMaxDoanhSo == -1 || dsSo > maxDoanhSo) { maxDoanhSo = dsSo; idxMaxDoanhSo = i; }
            if (idxMinDoanhSo == -1 || dsSo < minDoanhSo) { minDoanhSo = dsSo; idxMinDoanhSo = i; }
            countKD++;
        }
        
        int tuoi = curYear - nv.namsinh.nam;
        sumTuoi += tuoi;
        if (tuoi > maxTuoi) { maxTuoi = tuoi; idxMaxTuoi = i; }
        if (tuoi < minTuoi) { minTuoi = tuoi; idxMinTuoi = i; }
        
        if (nv.gioiTinh == GT_NU) countNu++;
        else countNam++;
    }
    
    char hoTen[70];
    char msnvStr[12];
    char val[32];
    
    printf("\nBANG THONG KE LUONG\n");
    inCot("Hang muc", 24); printf(" ");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-6s %15s\n", "MSNV", "Tong luong");
    printf("--------------------------------------------------------------------------\n");
    
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxLuong].ho, ds.a[idxMaxLuong].ten);
    snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMaxLuong].msnv);
    formatTien((long long)maxLuong, val, sizeof(val));
    inCot("Luong cao nhat", 24); printf(" ");
    inCot(hoTen, 24); printf(" ");
    printf("%-6s %15s\n", msnvStr, val);
    
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinLuong].ho, ds.a[idxMinLuong].ten);
    snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMinLuong].msnv);
    formatTien((long long)minLuong, val, sizeof(val));
    inCot("Luong thap nhat", 24); printf(" ");
    inCot(hoTen, 24); printf(" ");
    printf("%-6s %15s\n", msnvStr, val);
    
    formatTien((long long)(sumLuong / ds.n + 0.5), val, sizeof(val));
    inCot("Luong trung binh", 24); printf(" ");
    inCot("", 24); printf(" ");
    printf("%-6s %15s\n", "", val);
    
    printf("\nBANG THONG KE PHU CAP\n");
    inCot("Hang muc", 24); printf(" ");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-6s %15s\n", "MSNV", "Phu cap");
    printf("--------------------------------------------------------------------------\n");
    
    if (idxMaxPC >= 0) {
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxPC].ho, ds.a[idxMaxPC].ten);
        snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMaxPC].msnv);
        formatTien((long long)maxPC, val, sizeof(val));
        inCot("Phu cap cao nhat", 24); printf(" ");
        inCot(hoTen, 24); printf(" ");
        printf("%-6s %15s\n", msnvStr, val);
        
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinPC].ho, ds.a[idxMinPC].ten);
        snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMinPC].msnv);
        formatTien((long long)minPC, val, sizeof(val));
        inCot("Phu cap thap nhat", 24); printf(" ");
        inCot(hoTen, 24); printf(" ");
        printf("%-6s %15s\n", msnvStr, val);
        
        formatTien((long long)(sumPC / ds.n + 0.5), val, sizeof(val));
        inCot("PC trung binh", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", val);
    }
    
    printf("\nBANG THONG KE DOANH SO\n");
    inCot("Hang muc", 24); printf(" ");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-6s %15s\n", "MSNV", "Doanh so");
    printf("--------------------------------------------------------------------------\n");
    
    if (countKD > 0) {
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxDoanhSo].ho, ds.a[idxMaxDoanhSo].ten);
        snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMaxDoanhSo].msnv);
        formatTien((long long)maxDoanhSo, val, sizeof(val));
        inCot("Doanh so cao nhat", 24); printf(" ");
        inCot(hoTen, 24); printf(" ");
        printf("%-6s %15s\n", msnvStr, val);
        
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinDoanhSo].ho, ds.a[idxMinDoanhSo].ten);
        snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMinDoanhSo].msnv);
        formatTien((long long)minDoanhSo, val, sizeof(val));
        inCot("Doanh so thap nhat", 24); printf(" ");
        inCot(hoTen, 24); printf(" ");
        printf("%-6s %15s\n", msnvStr, val);
        
        formatTien((long long)(sumDoanhSo / countKD + 0.5), val, sizeof(val));
        inCot("Doanh so trung binh", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", val);
        
        formatTien((long long)sumDoanhSo, val, sizeof(val));
        inCot("Tong doanh so cty", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", val);
    } else {
        inCot("Doanh so cao nhat", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", "0");
        inCot("Doanh so thap nhat", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", "0");
        inCot("Doanh so trung binh", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", "0");
        inCot("Tong doanh so cty", 24); printf(" ");
        inCot("", 24); printf(" ");
        printf("%-6s %15s\n", "", "0");
    }
    
    printf("\nBANG THONG KE TUOI\n");
    inCot("Hang muc", 24); printf(" ");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-6s %15s\n", "MSNV", "Tuoi");
    printf("--------------------------------------------------------------------------\n");
    
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxTuoi].ho, ds.a[idxMaxTuoi].ten);
    snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMaxTuoi].msnv);
    snprintf(val, sizeof(val), "%d", maxTuoi);
    inCot("Tuoi cao nhat", 24); printf(" ");
    inCot(hoTen, 24); printf(" ");
    printf("%-6s %15s\n", msnvStr, val);
    
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinTuoi].ho, ds.a[idxMinTuoi].ten);
    snprintf(msnvStr, sizeof(msnvStr), "%d", ds.a[idxMinTuoi].msnv);
    snprintf(val, sizeof(val), "%d", minTuoi);
    inCot("Tuoi thap nhat", 24); printf(" ");
    inCot(hoTen, 24); printf(" ");
    printf("%-6s %15s\n", msnvStr, val);
    
    snprintf(val, sizeof(val), "%d", (int)(sumTuoi / ds.n + 0.5));
    inCot("Tuoi trung binh", 24); printf(" ");
    inCot("", 24); printf(" ");
    printf("%-6s %15s\n", "", val);
    
    printf("\nBANG THONG KE GIOI TINH\n");
    inCot("Gioi tinh", 12); printf(" ");
    printf("%-10s\n", "So luong");
    printf("------------------------\n");
    inCot("Nam", 12); printf(" ");
    printf("%-10d\n", countNam);
    inCot("Nu", 12); printf(" ");
    printf("%-10d\n", countNu);
    
    printf("\nBANG BAO CAO THAM NIEN\n");
    baoCaoThamNien(ds);
}

void xuatThongKeCSV(DanhSach ds, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) { printf(">> Loi mo file CSV!\n"); return; }
    fprintf(f, "\xEF\xBB\xBF");
    
    double sumLuong = 0;
    float maxLuong = ds.a[0].tongLuong;
    float minLuong = ds.a[0].tongLuong;
    int idxMaxLuong = 0;
    int idxMinLuong = 0;
    
    double sumPC = 0;
    float maxPC = -1;
    float minPC = -1;
    int idxMaxPC = -1;
    int idxMinPC = -1;
    
    double sumDoanhSo = 0;
    float maxDoanhSo = -1;
    float minDoanhSo = -1;
    int idxMaxDoanhSo = -1;
    int idxMinDoanhSo = -1;
    int countKD = 0;
    
    int curYear = layNamHienTai();
    int sumTuoi = 0;
    int maxTuoi = curYear - ds.a[0].namsinh.nam;
    int minTuoi = curYear - ds.a[0].namsinh.nam;
    int idxMaxTuoi = 0;
    int idxMinTuoi = 0;
    
    int countNam = 0;
    int countNu = 0;
    
    for (int i = 0; i < ds.n; i++) {
        NhanVien nv = ds.a[i];
        
        sumLuong += nv.tongLuong;
        if (nv.tongLuong > maxLuong) { maxLuong = nv.tongLuong; idxMaxLuong = i; }
        if (nv.tongLuong < minLuong) { minLuong = nv.tongLuong; idxMinLuong = i; }
        
        int tn = tinhThamNien(nv.ngayvao);
        float pc = tinhPhuCapThamNien(tn);
        if (nv.loai == TRUONG_PHONG) pc += nv.rieng.tp.phuCapCV;
        sumPC += pc;
        if (idxMaxPC == -1 || pc > maxPC) { maxPC = pc; idxMaxPC = i; }
        if (idxMinPC == -1 || pc < minPC) { minPC = pc; idxMinPC = i; }
        
        if (nv.loai == NV_KINH_DOANH) {
            float dsSo = nv.rieng.kd.doanhSo;
            sumDoanhSo += dsSo;
            if (idxMaxDoanhSo == -1 || dsSo > maxDoanhSo) { maxDoanhSo = dsSo; idxMaxDoanhSo = i; }
            if (idxMinDoanhSo == -1 || dsSo < minDoanhSo) { minDoanhSo = dsSo; idxMinDoanhSo = i; }
            countKD++;
        }
        
        int tuoi = curYear - nv.namsinh.nam;
        sumTuoi += tuoi;
        if (tuoi > maxTuoi) { maxTuoi = tuoi; idxMaxTuoi = i; }
        if (tuoi < minTuoi) { minTuoi = tuoi; idxMinTuoi = i; }
        
        if (nv.gioiTinh == GT_NU) countNu++;
        else countNam++;
    }
    
    char hoTen[70];
    
    fprintf(f, "BANG THONG KE LUONG\n");
    fprintf(f, "Hang muc,Ho va ten,MSNV,Tong luong\n");
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxLuong].ho, ds.a[idxMaxLuong].ten);
    fprintf(f, "Luong cao nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMaxLuong].msnv, maxLuong);
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinLuong].ho, ds.a[idxMinLuong].ten);
    fprintf(f, "Luong thap nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMinLuong].msnv, minLuong);
    fprintf(f, "Luong trung binh,,,%0.0f\n\n", sumLuong / ds.n);
    
    fprintf(f, "BANG THONG KE PHU CAP\n");
    fprintf(f, "Hang muc,Ho va ten,MSNV,Phu cap\n");
    if (idxMaxPC >= 0) {
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxPC].ho, ds.a[idxMaxPC].ten);
        fprintf(f, "Phu cap cao nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMaxPC].msnv, maxPC);
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinPC].ho, ds.a[idxMinPC].ten);
        fprintf(f, "Phu cap thap nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMinPC].msnv, minPC);
        fprintf(f, "PC trung binh,,,%0.0f\n\n", sumPC / ds.n);
    } else {
        fprintf(f, "Phu cap cao nhat,,,0\nPhu cap thap nhat,,,0\nPC trung binh,,,0\n\n");
    }
    
    fprintf(f, "BANG THONG KE DOANH SO\n");
    fprintf(f, "Hang muc,Ho va ten,MSNV,Doanh so\n");
    if (countKD > 0) {
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxDoanhSo].ho, ds.a[idxMaxDoanhSo].ten);
        fprintf(f, "Doanh so cao nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMaxDoanhSo].msnv, maxDoanhSo);
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinDoanhSo].ho, ds.a[idxMinDoanhSo].ten);
        fprintf(f, "Doanh so thap nhat,%s,%d,%.0f\n", hoTen, ds.a[idxMinDoanhSo].msnv, minDoanhSo);
        fprintf(f, "Doanh so trung binh,,,%0.0f\n", sumDoanhSo / countKD);
        fprintf(f, "Tong doanh so cty,,,%0.0f\n\n", sumDoanhSo);
    } else {
        fprintf(f, "Doanh so cao nhat,,,0\nDoanh so thap nhat,,,0\nDoanh so trung binh,,,0\nTong doanh so cty,,,0\n\n");
    }
    
    fprintf(f, "BANG THONG KE TUOI\n");
    fprintf(f, "Hang muc,Ho va ten,MSNV,Tuoi\n");
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMaxTuoi].ho, ds.a[idxMaxTuoi].ten);
    fprintf(f, "Tuoi cao nhat,%s,%d,%d\n", hoTen, ds.a[idxMaxTuoi].msnv, maxTuoi);
    snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[idxMinTuoi].ho, ds.a[idxMinTuoi].ten);
    fprintf(f, "Tuoi thap nhat,%s,%d,%d\n", hoTen, ds.a[idxMinTuoi].msnv, minTuoi);
    fprintf(f, "Tuoi trung binh,,,%d\n\n", (int)(sumTuoi / ds.n + 0.5));
    
    fprintf(f, "BANG THONG KE GIOI TINH\n");
    fprintf(f, "Gioi tinh,So luong\n");
    fprintf(f, "Nam,%d\n", countNam);
    fprintf(f, "Nu,%d\n", countNu);
    
    fprintf(f, "\nBANG BAO CAO THAM NIEN\n");
    fprintf(f, "Ho va ten,Chuc vu,Tham nien,PC tham nien\n");
    for (int i = 0; i < ds.n; i++) {
        int tn = tinhThamNien(ds.a[i].ngayvao);
        float pc = tinhPhuCapThamNien(tn);
        char hoTen[70];
        char chucVu[20];
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[i].ho, ds.a[i].ten);
        if (ds.a[i].loai == NV_BINH_THUONG) strcpy(chucVu, "Nhan vien");
        else if (ds.a[i].loai == TRUONG_PHONG) strcpy(chucVu, "Truong phong");
        else strcpy(chucVu, "Kinh doanh");
        fprintf(f, "%s,%s,%d,%.0f\n", hoTen, chucVu, tn, pc);
    }
    
    fclose(f);
    printf(">> Da luu CSV vao %s\n", filename);
}

// --- FILE IO ---
void luuFile(DanhSach ds, const char* filename, char* mode) {
    FILE* f = fopen(filename, mode);
    if (!f) { printf(">> Loi mo file!\n"); return; }
    fwrite(&ds.n, sizeof(int), 1, f);
    if (ds.n > 0) fwrite(ds.a, sizeof(NhanVien), ds.n, f);
    fclose(f);
    printf(">> Da luu vao %s\n", filename);
}

void docFile(DanhSach* ds, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) { ds->n = 0; return; }
    fread(&ds->n, sizeof(int), 1, f);
    if (ds->n > MAX_SL) {
        printf("\n[CANH BAO] File chua %d ban ghi, vuot qua suc chua (%d)!\n", ds->n, MAX_SL);
        printf("Chi lay %d ban ghi dau tien.\n", MAX_SL);
        ds->n = MAX_SL;
    }
    if (ds->n > 0) fread(ds->a, sizeof(NhanVien), ds->n, f);
    fclose(f);
}

int coDuoiCSV(const char* s) {
    size_t len = strlen(s);
    if (len < 4) return 0;
    char c1 = (char)tolower((unsigned char)s[len - 4]);
    char c2 = (char)tolower((unsigned char)s[len - 3]);
    char c3 = (char)tolower((unsigned char)s[len - 2]);
    char c4 = (char)tolower((unsigned char)s[len - 1]);
    return c1 == '.' && c2 == 'c' && c3 == 's' && c4 == 'v';
}

int laChuoiNgay(const char* s) {
    int d = 0, m = 0, y = 0;
    char extra;
    return sscanf(s, "%d/%d/%d%c", &d, &m, &y, &extra) == 3;
}

const char* chuGioiTinh(GioiTinh gt) {
    return gt == GT_NU ? "Nu" : "Nam";
}

GioiTinh gioiTinhTuChuoi(const char* s) {
    if (!s || s[0] == '\0') return GT_NAM;
    if (isdigit((unsigned char)s[0])) return (atoi(s) == 2) ? GT_NU : GT_NAM;
    if (strcmp(s, "Nam") == 0) return GT_NAM;
    if (strcmp(s, "Nu") == 0) return GT_NU;
    return GT_NAM;
}

void luuFileCSV(DanhSach ds, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) { printf(">> Loi mo file CSV!\n"); return; }
    fprintf(f, "\xEF\xBB\xBF");
    
    fprintf(f, "MSNV,Ho,Ten,Gioi tinh,Ngay sinh,Noi sinh,Dia chi,Ngay vao,Phong ban,Chuc vu,Luong co ban,Phu cap chuc vu,Phu cap tham nien,Doanh so,Ty le hoa hong,Tien hoa hong,Tong luong\n");
    
    for (int i = 0; i < ds.n; i++) {
        NhanVien nv = ds.a[i];
        
        char nsinh[16];
        char ngayvao[16];
        snprintf(nsinh, sizeof(nsinh), "%02d/%02d/%04d", nv.namsinh.ngay, nv.namsinh.thang, nv.namsinh.nam);
        snprintf(ngayvao, sizeof(ngayvao), "%02d/%02d/%04d", nv.ngayvao.ngay, nv.ngayvao.thang, nv.ngayvao.nam);
        
        float phuCapCV = 0;
        float phuCapTN = tinhPhuCapThamNien(tinhThamNien(nv.ngayvao));
        float doanhSo = 0;
        float tiLeHH = 0;
        float tienHH = 0;
        
        char chucVu[20];
        if (nv.loai == NV_BINH_THUONG) strcpy(chucVu, "Nhan vien");
        else if (nv.loai == TRUONG_PHONG) strcpy(chucVu, "Truong phong");
        else strcpy(chucVu, "Sale");
        
        if (nv.loai == TRUONG_PHONG) {
            phuCapCV = nv.rieng.tp.phuCapCV;
        } else if (nv.loai == NV_KINH_DOANH) {
            doanhSo = nv.rieng.kd.doanhSo;
            tiLeHH = nv.rieng.kd.tiLeHoaHong;
            tienHH = nv.rieng.kd.tienHoaHong;
        }
        
        fprintf(f, "%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%.0f,%.0f,%.0f,%.0f,%.2f,%.0f,%.0f\n",
                nv.msnv, nv.ho, nv.ten, chuGioiTinh(nv.gioiTinh), nsinh, nv.noisinh, nv.diachi, ngayvao, nv.phongBan,
                chucVu, nv.luongCoBan, phuCapCV, phuCapTN, doanhSo, tiLeHH, tienHH, nv.tongLuong);
    }
    
    fclose(f);
    printf(">> Da luu CSV vao %s\n", filename);
}

void docFileCSV(DanhSach* ds, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) { ds->n = 0; return; }
    
    ds->n = 0;
    char line[1024];
    int lineNo = 0;
    
    while (fgets(line, sizeof(line), f)) {
        xoaXuongDong(line);
        
        if (lineNo == 0 && strstr(line, "MSNV") != NULL) {
            lineNo++;
            continue;
        }
        
        if (ds->n >= MAX_SL) break;
        
        NhanVien nv;
        memset(&nv, 0, sizeof(NhanVien));
        
        char* token = strtok(line, ",");
        if (!token) continue;
        nv.msnv = atoi(token);
        
        token = strtok(NULL, ","); if (token) strncpy(nv.ho, token, sizeof(nv.ho) - 1);
        token = strtok(NULL, ","); if (token) strncpy(nv.ten, token, sizeof(nv.ten) - 1);
        
        token = strtok(NULL, ",");
        if (token) {
            if (laChuoiNgay(token)) {
                sscanf(token, "%d/%d/%d", &nv.namsinh.ngay, &nv.namsinh.thang, &nv.namsinh.nam);
                nv.gioiTinh = GT_NAM;
            } else {
                nv.gioiTinh = gioiTinhTuChuoi(token);
                token = strtok(NULL, ",");
                if (token) sscanf(token, "%d/%d/%d", &nv.namsinh.ngay, &nv.namsinh.thang, &nv.namsinh.nam);
            }
        }
        
        token = strtok(NULL, ","); if (token) strncpy(nv.noisinh, token, sizeof(nv.noisinh) - 1);
        token = strtok(NULL, ","); if (token) strncpy(nv.diachi, token, sizeof(nv.diachi) - 1);
        token = strtok(NULL, ","); if (token) sscanf(token, "%d/%d/%d", &nv.ngayvao.ngay, &nv.ngayvao.thang, &nv.ngayvao.nam);
        token = strtok(NULL, ","); if (token) strncpy(nv.phongBan, token, sizeof(nv.phongBan) - 1);
        
        token = strtok(NULL, ",");
        if (token) {
            if (isdigit((unsigned char)token[0])) {
                nv.loai = (LoaiNV)atoi(token);
            } else if (strcmp(token, "Nhan vien") == 0) {
                nv.loai = NV_BINH_THUONG;
            } else if (strcmp(token, "Truong phong") == 0) {
                nv.loai = TRUONG_PHONG;
            } else if (strcmp(token, "Sale") == 0) {
                nv.loai = NV_KINH_DOANH;
            } else {
                nv.loai = NV_BINH_THUONG;
            }
        }
        
        token = strtok(NULL, ","); if (token) nv.luongCoBan = strtof(token, NULL);
        
        token = strtok(NULL, ",");
        if (token && nv.loai == TRUONG_PHONG) nv.rieng.tp.phuCapCV = strtof(token, NULL);
        
        token = strtok(NULL, ",");
        
        token = strtok(NULL, ",");
        if (token && nv.loai == NV_KINH_DOANH) nv.rieng.kd.doanhSo = strtof(token, NULL);
        
        token = strtok(NULL, ",");
        if (token && nv.loai == NV_KINH_DOANH) nv.rieng.kd.tiLeHoaHong = strtof(token, NULL);
        
        token = strtok(NULL, ",");
        if (token && nv.loai == NV_KINH_DOANH) nv.rieng.kd.tienHoaHong = strtof(token, NULL);
        
        token = strtok(NULL, ",");
        
        tinhTongLuong(&nv);
        ds->a[ds->n++] = nv;
    }
    
    fclose(f);
}

void saoLuuKhoiPhuc(DanhSach* ds) {
    printf("\n1. Sao luu CSV\n2. Phuc hoi tu CSV\nChon: ");
    int c = nhapSoNguyen("");
    
    char fname[50];
    nhapChuoi("Nhap ten file CSV: ", fname, sizeof(fname));
    
    if (!coDuoiCSV(fname)) {
        char full[50];
        snprintf(full, sizeof(full), "%s.csv", fname);
        strncpy(fname, full, sizeof(fname) - 1);
        fname[sizeof(fname) - 1] = '\0';
    }
    
    if (c == 1) luuFileCSV(*ds, fname);
    else { docFileCSV(ds, fname); printf(">> Hoan tat!\n"); }
}

void menuTimKiem(DanhSach ds) {
    int c;
    do {
        printf("\n--- CHUC NANG TIM KIEM ---");
        printf("\n1. Tim kiem nang cao");
        printf("\n2. Tim kiem theo khoang");
        printf("\n3. Tim ho ten (tuan tu)");
        printf("\n4. Tim MSNV (nhi phan)");
        printf("\n5. Quay lai");
        c = nhapSoNguyen("\nChon: ");
        
        switch (c) {
            case 1: timKiemNangCao(ds); break;
            case 2: timKiemTheoKhoang(ds); break;
            case 3: timHoTenTuanTu(ds); break;
            case 4: {
                int msnv = nhapSoNguyen("Nhap MSNV can tim: ");
                int pos = timMSNVNhiPhan(ds, msnv);
                if (pos == -1) printf("Khong tim thay!\n");
                else xuat1NV(ds.a[pos]);
                break;
            }
            case 5: break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (c != 5);
}

void menuSapXep(DanhSach* ds) {
    int c;
    do {
        printf("\n--- CHUC NANG SAP XEP ---");
        printf("\n1. Sap xep MSNV (chon)");
        printf("\n2. Sap xep ten (chon)");
        printf("\n3. Sap xep luong (noi bot)");
        printf("\n4. Quay lai");
        c = nhapSoNguyen("\nChon: ");
        
        switch (c) {
            case 1:
                sapXepChonTheoMSNV(ds);
                printf(">> Da sap xep theo MSNV (chon).\n");
                xuatDanhSach(*ds);
                break;
            case 2:
                sapXepChonTheoTen(ds);
                printf(">> Da sap xep theo ten (chon).\n");
                xuatDanhSach(*ds);
                break;
            case 3:
                sapXepNoiBotTheoLuong(ds);
                printf(">> Da sap xep theo luong (noi bot).\n");
                xuatDanhSach(*ds);
                break;
            case 4: break;
            default: printf("Lua chon khong hop le!\n");
        }
    } while (c != 4);
}

void menu() {
    DanhSach ds;
    ds.n = 0;
    docFile(&ds, FILE_AUTO);
    
    int c;
    do {
        printf("\n=== QUAN LY NHAN SU CONG TY ===");
        printf("\n1. Them nhan vien");
        printf("\n2. Xuat danh sach");
        printf("\n3. Chuc nang tim kiem");
        printf("\n4. Chuc nang sap xep");
        printf("\n5. Sao luu / Phuc hoi (CSV)");
        printf("\n6. Thong ke tong hop");
        printf("\n7. Luu file va thoat");
        printf("\n8. Xoa nhan vien theo ma");
        printf("\n9. Sua thong tin nhan vien");
        c = nhapSoNguyen("\nChon: ");
        
        switch (c) {
            case 1: themMoiNV(&ds); break;
            case 2: xuatDanhSach(ds); break;
            case 3: menuTimKiem(ds); break;
            case 4: menuSapXep(&ds); break;
            case 5: saoLuuKhoiPhuc(&ds); break;
            case 6: thongKeTongHop(ds); break;
            case 7:
                luuFile(ds, FILE_AUTO, "wb");
                luuFileCSV(ds, FILE_AUTO_CSV);
                printf(">> Da luu du lieu. Tam biet!\n");
                break;
            case 8: {
                if (ds.n == 0) {
                    printf(">> Danh sach trong!\n");
                    break;
                }
                int msnv = nhapSoNguyen("Nhap MSNV can xoa: ");
                int pos = timMSNVTuanTu(ds, msnv);
                if (pos == -1) {
                    printf(">> Khong tim thay nhan vien co MSNV %d!\n", msnv);
                } else {
                    printf("\n>> Thong tin nhan vien se xoa:\n");
                    xuat1NV(ds.a[pos]);
                    printf("\nBan co chac muon xoa? (1: Co | 0: Khong): ");
                    int xacNhan = nhapSoNguyen("");
                    if (xacNhan == 1) {
                        // Dich cac phan tu ve truoc
                        for (int i = pos; i < ds.n - 1; i++) {
                            ds.a[i] = ds.a[i + 1];
                        }
                        ds.n--;
                        printf(">> Da xoa nhan vien thanh cong!\n");
                    } else {
                        printf(">> Da huy thao tac xoa.\n");
                    }
                }
                break;
            }
            case 9: {
                if (ds.n == 0) {
                    printf(">> Danh sach trong!\n");
                    break;
                }
                int msnv = nhapSoNguyen("Nhap MSNV can sua: ");
                int pos = timMSNVTuanTu(ds, msnv);
                if (pos == -1) {
                    printf(">> Khong tim thay nhan vien co MSNV %d!\n", msnv);
                } else {
                    printf("\n>> Thong tin hien tai:\n");
                    xuat1NV(ds.a[pos]);
                    printf("\n>> Nhap thong tin moi:\n");
                    int idCu = ds.a[pos].msnv;
                    if (nhap1NV(&ds.a[pos], idCu)) {
                        printf(">> Da cap nhat thanh cong!\n");
                    } else {
                        printf(">> Da huy cap nhat.\n");
                    }
                }
                break;
            }
            default: if (c != 7) printf(">> Lua chon khong hop le!\n");
        }
    } while (c != 7);
}
