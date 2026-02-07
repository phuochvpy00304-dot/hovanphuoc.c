// =============================================================================
// GIẢI THÍCH CODE QUẢN LÝ NHÂN SỰ - hovanphuoc.c
// =============================================================================

// 1. THƯ VIỆN VÀ HẰNG SỐ

#include <stdio.h>      // Thư viện nhập xuất chuẩn (printf, scanf, fopen...)
#include <string.h>     // Thư viện xử lý chuỗi (strlen, strcpy, strcmp...)
#include <stdlib.h>     // Thư viện chuẩn (malloc, atoi, strtol...)
#include <time.h>       // Thư viện xử lý thời gian (time, localtime...)
#include <ctype.h>      // Thư viện kiểm tra ký tự (isdigit, tolower...)
#include <locale.h>     // Thư viện hỗ trợ ngôn ngữ địa phương

#define MAX_SL 100                  // Số lượng nhân viên tối đa
#define FILE_AUTO "hethong.dat"     // Tên file lưu dữ liệu nhị phân
#define FILE_AUTO_CSV "hethong.csv" // Tên file lưu dữ liệu CSV

// -----------------------------------------------------------------------------
// 2. CẤU TRÚC DỮ LIỆU
// -----------------------------------------------------------------------------

// 2.1 Cấu trúc Date (Ngày tháng)
typedef struct {
    int ngay;   // Ngày (1-31)
    int thang;  // Tháng (1-12)
    int nam;    // Năm (1900-2100)
} Date;
// Giải thích: Lưu trữ thông tin ngày tháng năm

// 2.2 Enum LoaiNV (Loại nhân viên)
typedef enum {
    NV_BINH_THUONG = 1,  // Nhân viên thường
    TRUONG_PHONG = 2,    // Trưởng phòng
    NV_KINH_DOANH = 3    // Nhân viên kinh doanh
} LoaiNV;
// Giải thích: Định nghĩa 3 loại chức vụ nhân viên

// 2.3 Enum GioiTinh
typedef enum {
    GT_NAM = 1,  // Nam
    GT_NU = 2    // Nữ
} GioiTinh;
// Giải thích: Định nghĩa giới tính

// 2.4 Cấu trúc NhanVien (Nhân viên)
typedef struct {
    int msnv;                // Mã số nhân viên (tự động tăng)
    char ho[30];             // Họ đệm
    char ten[30];            // Tên
    GioiTinh gioiTinh;       // Giới tính (1=Nam, 2=Nữ)
    Date namsinh;            // Ngày sinh
    char noisinh[100];       // Nơi sinh
    char diachi[200];        // Địa chỉ
    Date ngayvao;            // Ngày vào làm
    char phongBan[50];       // Phòng ban
    LoaiNV loai;             // Loại nhân viên (1=Thường, 2=TP, 3=KD)
    float luongCoBan;        // Lương cơ bản
    
    union {                  // Union: chỉ dùng 1 trong 2 cấu trúc
        struct {
            float phuCapCV;  // Phụ cấp chức vụ (cho Trưởng phòng)
        } tp;
        struct {
            float doanhSo;        // Doanh số (cho NV Kinh doanh)
            float tiLeHoaHong;    // Tỷ lệ hoa hồng (%)
            float tienHoaHong;    // Tiền hoa hồng (tính toán)
        } kd;
    } rieng;
    
    float tongLuong;         // Tổng lương (tính toán)
} NhanVien;
// Giải thích: 
// - Union `rieng` tiết kiệm bộ nhớ vì Trưởng phòng và NV Kinh doanh có thông tin khác nhau
// - Trưởng phòng có phụ cấp chức vụ
// - NV Kinh doanh có doanh số và hoa hồng

// 2.5 Cấu trúc DanhSach
typedef struct {
    NhanVien a[MAX_SL];  // Mảng chứa tối đa 100 nhân viên
    int n;               // Số lượng nhân viên hiện tại
} DanhSach;

// -----------------------------------------------------------------------------
// 3. HÀM MAIN
// -----------------------------------------------------------------------------

int main() {
    setlocale(LC_ALL, "");  // Hỗ trợ hiển thị tiếng Việt
    menu();                 // Gọi hàm menu chính
    return 0;
}

// -----------------------------------------------------------------------------
// 4. CÁC HÀM XỬ LÝ CHUỖI VÀ NHẬP LIỆU
// -----------------------------------------------------------------------------

// 4.1 Hàm xoaXuongDong
void xoaXuongDong(char* str) {
    size_t len = strlen(str);                      // Lấy độ dài chuỗi
    if (len > 0 && str[len - 1] == '\n')          // Nếu ký tự cuối là xuống dòng
        str[len - 1] = '\0';                       // Thay bằng ký tự kết thúc chuỗi
}
// Mục đích: Xóa ký tự xuống dòng '\n' ở cuối chuỗi khi dùng fgets()

// 4.2 Hàm nhapChuoi
void nhapChuoi(const char* msg, char* str, int size) {
    printf("%s", msg);              // In thông báo
    fgets(str, size, stdin);        // Đọc chuỗi từ bàn phím
    xoaXuongDong(str);              // Xóa ký tự xuống dòng
}

// 4.3 Hàm nhapSoNguyen
int nhapSoNguyen(const char* msg) {
    char buffer[100];
    char* endPtr;
    while (1) {                                    // Lặp đến khi nhập đúng
        printf("%s", msg);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            xoaXuongDong(buffer);
            if (strlen(buffer) == 0) continue;     // Bỏ qua nếu chuỗi rỗng
            int val = (int)strtol(buffer, &endPtr, 10);  // Chuyển chuỗi sang số
            if (*endPtr == '\0') return val;       // Nếu chuyển đổi thành công
        }
    }
}
// Giải thích: 
// - Dùng `strtol()` thay vì `scanf()` để kiểm tra lỗi tốt hơn
// - `endPtr` trỏ đến ký tự đầu tiên không phải số
// - Nếu `*endPtr == '\0'` nghĩa là toàn bộ chuỗi là số hợp lệ

// 4.4 Hàm nhapSoThuc
float nhapSoThuc(const char* msg) {
    char buffer[100];
    char* endPtr;
    while (1) {
        printf("%s", msg);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            xoaXuongDong(buffer);
            if (strlen(buffer) == 0) continue;
            float val = strtof(buffer, &endPtr);   // Chuyển chuỗi sang số thực
            if (*endPtr == '\0') return val;
        }
    }
}

// 4.5 Hàm nhapSoThucDuong
float nhapSoThucDuong(const char* msg) {
    float val;
    do {
        val = nhapSoThuc(msg);
        if (val <= 0) 
            printf("   >> Loi: Gia tri phai > 0!\n");
    } while (val <= 0);                            // Lặp đến khi nhập số > 0
    return val;
}

// -----------------------------------------------------------------------------
// 5. CÁC HÀM XỬ LÝ NGÀY THÁNG
// -----------------------------------------------------------------------------

// 5.1 Hàm laNamNhuan
int laNamNhuan(int nam) {
    return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}
// Quy tắc năm nhuận:
// - Chia hết cho 4 NHƯNG không chia hết cho 100, HOẶC
// - Chia hết cho 400
// - Ví dụ: 2024 là năm nhuận, 1900 không phải, 2000 là năm nhuận

// 5.2 Hàm kiemTraNgayHopLe
int kiemTraNgayHopLe(int d, int m, int y) {
    if (y < 1900 || y > 2100) return 0;           // Năm phải từ 1900-2100
    if (m < 1 || m > 12) return 0;                // Tháng từ 1-12
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Mảng số ngày của mỗi tháng (index 0 không dùng)
    
    if (laNamNhuan(y)) daysInMonth[2] = 29;       // Tháng 2 có 29 ngày nếu năm nhuận
    
    if (d < 1 || d > daysInMonth[m]) return 0;    // Kiểm tra ngày hợp lệ
    return 1;
}

// 5.3 Hàm nhapNgay
void nhapNgay(Date* d, const char* msg) {
    printf("%s (dd mm yyyy): ", msg);
    char buffer[100];
    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        // Đọc 3 số: ngày, tháng, năm
        if (sscanf(buffer, "%d %d %d", &d->ngay, &d->thang, &d->nam) == 3) {
            if (kiemTraNgayHopLe(d->ngay, d->thang, d->nam)) 
                return;                            // Thoát nếu ngày hợp lệ
        }
        printf("   >> Ngay khong ton tai! Nhap lai (vi du: 29 02 2024): ");
    }
}

// 5.4 Hàm layNgayHienTai
Date layNgayHienTai(void) {
    time_t t = time(NULL);              // Lấy thời gian hiện tại (giây từ 1/1/1970)
    struct tm tm = *localtime(&t);      // Chuyển sang cấu trúc tm
    Date d;
    d.ngay = tm.tm_mday;                // Ngày trong tháng
    d.thang = tm.tm_mon + 1;            // Tháng (tm_mon từ 0-11 nên +1)
    d.nam = tm.tm_year + 1900;          // Năm (tm_year là số năm từ 1900)
    return d;
}

// 5.5 Hàm tinhTuoiTaiNgay
int tinhTuoiTaiNgay(Date ngaySinh, Date ngayTinh) {
    int age = ngayTinh.nam - ngaySinh.nam;        // Tính hiệu năm
    
    // Nếu chưa đến sinh nhật trong năm nay thì trừ 1
    if (ngayTinh.thang < ngaySinh.thang ||
        (ngayTinh.thang == ngaySinh.thang && ngayTinh.ngay < ngaySinh.ngay)) {
        age--;
    }
    return age;
}
// Ví dụ: 
// - Sinh: 15/03/2000, Tính: 10/02/2024 → Tuổi = 23 (chưa đến sinh nhật)
// - Sinh: 15/03/2000, Tính: 20/03/2024 → Tuổi = 24 (đã qua sinh nhật)

// 5.6 Hàm tinhThamNien
int tinhThamNien(Date ngayVao) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int namHienTai = tm.tm_year + 1900;
    return namHienTai - ngayVao.nam;    // Số năm làm việc
}

// 5.7 Hàm tinhPhuCapThamNien
float tinhPhuCapThamNien(int thamNien) {
    if (thamNien >= 10) return 2000000;  // >= 10 năm: 2 triệu
    if (thamNien >= 5) return 1000000;   // >= 5 năm: 1 triệu
    return 0;                            // < 5 năm: 0
}

// -----------------------------------------------------------------------------
// 6. CÁC HÀM XỬ LÝ LOGIC CHÍNH
// -----------------------------------------------------------------------------

// 6.1 Hàm timMaxID
int timMaxID(DanhSach ds) {
    int max = 0;
    for(int i=0; i<ds.n; i++) {
        if(ds.a[i].msnv > max) 
            max = ds.a[i].msnv;              // Tìm MSNV lớn nhất
    }
    return max;
}
// Mục đích: Tìm MSNV lớn nhất để tạo MSNV mới = max + 1

// 6.2 Hàm tinhTongLuong
void tinhTongLuong(NhanVien* nv) {
    float phuCapCV = 0;
    float tienHoaHong = 0;
    
    int thamNien = tinhThamNien(nv->ngayvao);
    float phuCapThamNien = tinhPhuCapThamNien(thamNien);
    
    if (nv->loai == TRUONG_PHONG) {
        phuCapCV = nv->rieng.tp.phuCapCV;    // Lấy phụ cấp chức vụ
    } else if (nv->loai == NV_KINH_DOANH) {
        // Tính tiền hoa hồng = doanh số * tỷ lệ / 100
        nv->rieng.kd.tienHoaHong = nv->rieng.kd.doanhSo * (nv->rieng.kd.tiLeHoaHong / 100.0);
        tienHoaHong = nv->rieng.kd.tienHoaHong;
    }
    
    // Tổng lương = Lương CB + PC chức vụ + PC thâm niên + Hoa hồng
    nv->tongLuong = nv->luongCoBan + phuCapCV + phuCapThamNien + tienHoaHong;
}


// 6.3 Hàm nhap1NV - Nhập thông tin 1 nhân viên
int nhap1NV(NhanVien* nv, int idMoi) {
    nv->msnv = idMoi;                              // Gán MSNV tự động
    printf("   >> Ma NV (Tu dong): %d\n", nv->msnv);
    
    nhapChuoi("- Ho dem: ", nv->ho, sizeof(nv->ho));
    nhapChuoi("- Ten: ", nv->ten, sizeof(nv->ten));
    
    printf("- Gioi tinh (1: Nam | 2: Nu): ");
    int gt;
    do {
        gt = nhapSoNguyen("");
    } while (gt < 1 || gt > 2);                    // Lặp đến khi nhập 1 hoặc 2
    nv->gioiTinh = (GioiTinh)gt;                   // Ép kiểu sang GioiTinh
    
    Date homNay = layNgayHienTai();                // Lấy ngày hiện tại
    while (1) {
        nhapNgay(&nv->namsinh, "- Ngay sinh");
        int tuoi = tinhTuoiTaiNgay(nv->namsinh, homNay);
        if (tuoi >= 18) break;                     // Đủ 18 tuổi thì thoát vòng lặp
        
        printf(">> Nhan vien chua du 18 tuoi, khong the nhan viec.\n");
        printf("1. Nhap lai ngay sinh\n2. Huy them nhan vien\nChon: ");
        int chon = nhapSoNguyen("");
        if (chon == 2) {
            memset(nv, 0, sizeof(NhanVien));       // Xóa toàn bộ dữ liệu nhân viên
            return 0;                              // Trả về 0 = hủy thêm
        }
    }
    
    nhapChuoi("- Noi sinh: ", nv->noisinh, sizeof(nv->noisinh));
    nhapChuoi("- Dia chi: ", nv->diachi, sizeof(nv->diachi));
    nhapChuoi("- Phong ban: ", nv->phongBan, sizeof(nv->phongBan));
    
    // Kiểm tra ngày vào làm phải >= 18 tuổi
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
    
    // Nhập thông tin riêng theo chức vụ
    if (nv->loai == TRUONG_PHONG) {
        nv->rieng.tp.phuCapCV = nhapSoThucDuong("- Phu cap chuc vu: ");
    } else if (nv->loai == NV_KINH_DOANH) {
        nv->rieng.kd.doanhSo = nhapSoThuc("- Doanh so: ");
        nv->rieng.kd.tiLeHoaHong = nhapSoThuc("- Ty le hoa hong (%): ");
    }
    
    tinhTongLuong(nv);                             // Tính tổng lương
    return 1;                                      // Trả về 1 = thành công
}

// 6.4 Hàm themMoiNV - Thêm nhân viên mới
void themMoiNV(DanhSach* ds) {
    if (ds->n >= MAX_SL) {                         // Kiểm tra danh sách đầy
        printf(">> Danh sach day!\n"); 
        return; 
    }
    
    printf("\n--- THEM NHAN VIEN ---\n");
    int newID = timMaxID(*ds) + 1;                 // Tạo MSNV mới = max + 1
    
    if (!nhap1NV(&ds->a[ds->n], newID)) {          // Nhập thông tin NV mới
        printf(">> Da huy them nhan vien.\n");
        return;
    }
    
    ds->n++;                                       // Tăng số lượng nhân viên
    printf(">> Them thanh cong!\n");
}

// -----------------------------------------------------------------------------
// 7. CÁC HÀM ĐỊNH DẠNG VÀ HIỂN THỊ
// -----------------------------------------------------------------------------

// 7.1 Hàm formatTien - Định dạng số tiền
void formatTien(long long value, char* out, int size) {
    if (size <= 0) return;
    
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%lld", value);  // Chuyển số sang chuỗi
    if (len <= 0) { out[0] = '\0'; return; }
    
    int outIdx = 0;
    for (int i = 0; i < len; i++) {
        if (outIdx >= size - 1) break;
        out[outIdx++] = buf[i];                    // Copy ký tự
        
        int remaining = len - i - 1;               // Số ký tự còn lại
        if (remaining > 0 && (remaining % 3 == 0)) {
            if (outIdx < size - 1) 
                out[outIdx++] = '.';               // Thêm dấu chấm phân cách
        }
    }
    out[outIdx] = '\0';
}
// Ví dụ: 
// - Input: 5000000 → Output: "5.000.000"
// - Input: 12345678 → Output: "12.345.678"

// 7.2 Hàm demKyTuHienThi - Đếm ký tự hiển thị (UTF-8)
int demKyTuHienThi(const char* s) {
    int len = 0;
    int i = 0;
    while (s[i] != '\0') {
        // Kiểm tra byte đầu tiên của ký tự UTF-8
        // 0xC0 = 11000000, 0x80 = 10000000
        // Byte tiếp theo của ký tự UTF-8 có dạng 10xxxxxx
        if (((unsigned char)s[i] & 0xC0) != 0x80) 
            len++;                                 // Chỉ đếm byte đầu
        i++;
    }
    return len;
}
// Giải thích UTF-8:
// - Ký tự ASCII (1 byte): 0xxxxxxx
// - Ký tự tiếng Việt (2-3 byte): 
//   - Byte đầu: 110xxxxx hoặc 1110xxxx
//   - Byte tiếp: 10xxxxxx
// - Hàm này đếm số ký tự thực tế, không phải số byte

// 7.3 Hàm inCot - In cột với độ rộng cố định
void inCot(const char* str, int width) {
    int lenBytes = (int)strlen(str);               // Độ dài byte
    int lenVisual = demKyTuHienThi(str);           // Độ dài hiển thị
    int diff = lenBytes - lenVisual;               // Chênh lệch
    printf("%-*s", width + diff, str);             // In với độ rộng điều chỉnh
}
// Tại sao cần điều chỉnh?
// - Chuỗi "Nguyen" có 6 byte, 6 ký tự hiển thị
// - Chuỗi "Nguyễn" có 8 byte, 7 ký tự hiển thị
// - Để căn đều cột, cần cộng thêm (8-7)=1 vào độ rộng

// -----------------------------------------------------------------------------
// 8. CÁC HÀM XUẤT DỮ LIỆU
// -----------------------------------------------------------------------------

// 8.1 Hàm xuat1NV - Xuất thông tin 1 nhân viên
void xuat1NV(NhanVien nv) {
    // Xác định chức vụ
    char chucVu[20];
    if (nv.loai == NV_BINH_THUONG) strcpy(chucVu, "Nhan vien");
    else if (nv.loai == TRUONG_PHONG) strcpy(chucVu, "Truong phong");
    else strcpy(chucVu, "Kinh doanh");
    
    // Ghép họ tên
    char hoTen[70];
    snprintf(hoTen, sizeof(hoTen), "%s %s", nv.ho, nv.ten);
    
    // Tính thâm niên và phụ cấp
    int thamNien = tinhThamNien(nv.ngayvao);
    float pcThamNien = tinhPhuCapThamNien(thamNien);
    
    // Định dạng ngày sinh
    char nsinh[16];
    snprintf(nsinh, sizeof(nsinh), "%02d/%02d/%04d", 
             nv.namsinh.ngay, nv.namsinh.thang, nv.namsinh.nam);
    
    // Định dạng các số tiền
    char luongCB[20], phuCapCV[20], phuCapTN[20], doanhSo[20], tienHH[20], tongLuong[20];
    formatTien((long long)nv.luongCoBan, luongCB, sizeof(luongCB));
    formatTien((long long)nv.tongLuong, tongLuong, sizeof(tongLuong));
    formatTien((long long)pcThamNien, phuCapTN, sizeof(phuCapTN));
    
    // Khởi tạo chuỗi rỗng
    char tiLeHH[16] = "";
    phuCapCV[0] = '\0';
    doanhSo[0] = '\0';
    tienHH[0] = '\0';
    
    // Định dạng thông tin riêng theo chức vụ
    if (nv.loai == TRUONG_PHONG) {
        formatTien((long long)nv.rieng.tp.phuCapCV, phuCapCV, sizeof(phuCapCV));
    } else if (nv.loai == NV_KINH_DOANH) {
        formatTien((long long)nv.rieng.kd.doanhSo, doanhSo, sizeof(doanhSo));
        snprintf(tiLeHH, sizeof(tiLeHH), "%.0f%%", nv.rieng.kd.tiLeHoaHong);
        formatTien((long long)nv.rieng.kd.tienHoaHong, tienHH, sizeof(tienHH));
    }
    
    // In ra màn hình
    printf("%-6d ", nv.msnv);
    inCot(hoTen, 24); printf(" ");
    printf("%-12.12s ", nsinh);
    inCot(nv.phongBan, 12); printf(" ");
    inCot(chucVu, 14); printf(" ");
    printf("%8d %12.12s %12.12s %12.12s %12.12s %8.8s %12.12s %12.12s\n",
           thamNien, luongCB, phuCapCV, phuCapTN, doanhSo, tiLeHH, tienHH, tongLuong);
}

// 8.2 Hàm xuatDanhSach - Xuất toàn bộ danh sách
void xuatDanhSach(DanhSach ds) {
    if (ds.n == 0) { 
        printf("\n>> Danh sach trong!\n"); 
        return; 
    }
    
    // In tiêu đề bảng
    printf("\n%-6s ", "MSNV");
    inCot("Ho va ten", 24); printf(" ");
    printf("%-12s ", "Ngay sinh");
    inCot("Phong ban", 12); printf(" ");
    inCot("Chuc vu", 14); printf(" ");
    printf("%8s %12s %20s %12s %12s %8s %15s %16s\n",
           "Tham nien", "Luong CB", "PC chuc vu", "PC tham nien", 
           "Doanh so", "TL HH", "Tien HH", "Tong luong");
    
    // In đường kẻ ngang
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
    
    // In từng nhân viên
    for (int i = 0; i < ds.n; i++) 
        xuat1NV(ds.a[i]);
    
    printf("---------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// -----------------------------------------------------------------------------
// 9. CÁC HÀM TÌM KIẾM
// -----------------------------------------------------------------------------

// 9.1 Hàm timHoTenTuanTu - Tìm kiếm tuần tự theo họ tên
void timHoTenTuanTu(DanhSach ds) {
    char hoTenCanTim[100];
    nhapChuoi("Nhap ho ten can tim: ", hoTenCanTim, sizeof(hoTenCanTim));
    
    printf("\nKET QUA:\n");
    int count = 0;
    
    for (int i = 0; i < ds.n; i++) {
        char hoTen[70];
        snprintf(hoTen, sizeof(hoTen), "%s %s", ds.a[i].ho, ds.a[i].ten);
        
        // strstr() tìm chuỗi con
        if (strstr(hoTen, hoTenCanTim)) {          // Nếu tìm thấy
            xuat1NV(ds.a[i]);
            count++;
        }
    }
    
    if (count == 0) printf("Khong tim thay!\n");
}
// Ví dụ: Nhập "Nguyen" sẽ tìm thấy "Nguyen Van A", "Tran Thi Nguyen"...

// 9.2 Hàm timMSNVTuanTu - Tìm kiếm tuần tự theo MSNV
int timMSNVTuanTu(DanhSach ds, int msnv) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.a[i].msnv == msnv) 
            return i;                              // Trả về vị trí tìm thấy
    }
    return -1;                                     // Không tìm thấy
}
// Độ phức tạp: O(n) - duyệt tuần tự từ đầu đến cuối

// 9.3 Hàm timMSNVNhiPhan - Tìm kiếm nhị phân theo MSNV
int timMSNVNhiPhan(DanhSach ds, int msnv) {
    sapXepChonTheoMSNV(&ds);                       // Phải sắp xếp trước
    
    int left = 0;
    int right = ds.n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;              // Vị trí giữa
        
        if (ds.a[mid].msnv == msnv) 
            return mid;                            // Tìm thấy
        
        if (ds.a[mid].msnv < msnv) 
            left = mid + 1;                        // Tìm bên phải
        else 
            right = mid - 1;                       // Tìm bên trái
    }
    
    return -1;                                     // Không tìm thấy
}
// Độ phức tạp: O(log n) - nhanh hơn tìm kiếm tuần tự
// Điều kiện: Danh sách phải được sắp xếp trước

// -----------------------------------------------------------------------------
// 10. CÁC HÀM SẮP XẾP
// -----------------------------------------------------------------------------

// 10.1 Hàm sapXepChonTheoMSNV - Sắp xếp chọn theo MSNV
void sapXepChonTheoMSNV(DanhSach* ds) {
    for (int i = 0; i < ds->n - 1; i++) {
        int minIdx = i;                            // Giả sử phần tử i là nhỏ nhất
        
        // Tìm phần tử nhỏ nhất từ i+1 đến cuối
        for (int j = i + 1; j < ds->n; j++) {
            if (ds->a[j].msnv < ds->a[minIdx].msnv) 
                minIdx = j;
        }
        
        // Hoán đổi nếu tìm thấy phần tử nhỏ hơn
        if (minIdx != i) {
            NhanVien temp = ds->a[i];
            ds->a[i] = ds->a[minIdx];
            ds->a[minIdx] = temp;
        }
    }
}
// Thuật toán Selection Sort:
// - Tìm phần tử nhỏ nhất, đưa về đầu
// - Tìm phần tử nhỏ nhất tiếp theo, đưa về vị trí thứ 2
// - Lặp lại cho đến hết
// Độ phức tạp: O(n²)

// 10.2 Hàm soSanhTenHo - So sánh tên và họ
int soSanhTenHo(const NhanVien* a, const NhanVien* b) {
    int cmp = strcmp(a->ten, b->ten);              // So sánh tên trước
    if (cmp != 0) return cmp;
    
    cmp = strcmp(a->ho, b->ho);                    // Nếu tên bằng nhau, so sánh họ
    if (cmp != 0) return cmp;
    
    // Nếu họ tên giống nhau, so sánh MSNV
    if (a->msnv < b->msnv) return -1;
    if (a->msnv > b->msnv) return 1;
    return 0;
}
// Quy tắc so sánh:
// 1. Ưu tiên so sánh TÊN (không phải họ)
// 2. Nếu tên giống nhau → so sánh HỌ
// 3. Nếu họ tên giống nhau → so sánh MSNV

// 10.3 Hàm sapXepNoiBotTheoLuong - Sắp xếp nổi bọt theo lương
void sapXepNoiBotTheoLuong(DanhSach* ds) {
    for (int i = 0; i < ds->n - 1; i++) {
        for (int j = 0; j < ds->n - 1 - i; j++) {
            // So sánh 2 phần tử kề nhau
            if (ds->a[j].tongLuong < ds->a[j + 1].tongLuong) {
                NhanVien temp = ds->a[j];
                ds->a[j] = ds->a[j + 1];
                ds->a[j + 1] = temp;
            }
        }
    }
}
// Thuật toán Bubble Sort:
// - So sánh từng cặp phần tử kề nhau
// - Hoán đổi nếu sai thứ tự
// - Phần tử lớn nhất "nổi" lên cuối sau mỗi vòng lặp
// Độ phức tạp: O(n²)
// Sắp xếp giảm dần: Lương cao nhất lên đầu

// =============================================================================
// HẾT PHẦN GIẢI THÍCH CHI TIẾT
// =============================================================================
