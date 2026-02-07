# 🎯 HƯỚNG DẪN CHẠY TRÊN VS CODE - TỪNG BƯỚC CHI TIẾT

## 📋 MỤC LỤC
1. [Cài đặt GCC](#bước-1-cài-đặt-gcc)
2. [Cài Extension VS Code](#bước-2-cài-extension-vs-code)
3. [Chạy chương trình](#bước-3-chạy-chương-trình)
4. [Xử lý lỗi](#xử-lý-lỗi-thường-gặp)

---

## 🔧 BƯỚC 1: CÀI ĐẶT GCC

### Cách A: Tải và cài thủ công (Khuyến nghị)

#### Bước 1.1: Tải MinGW

**Mở trình duyệt và vào:**
```
https://winlibs.com/
```

**Hoặc link trực tiếp:**
```
https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0-16.0.6-11.0.0-ucrt-r1/winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.0-r1.7z
```

**Tìm và tải file:**
- Chọn phần **Win64**
- Chọn **UCRT runtime**
- Click **Download**
- File tải về: `winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.0-r1.7z`
- Dung lượng: ~60 MB

#### Bước 1.2: Giải nén

**Nếu chưa có 7-Zip:**
1. Tải 7-Zip: https://www.7-zip.org/
2. Cài đặt 7-Zip

**Giải nén MinGW:**
1. Chuột phải vào file `.7z` vừa tải
2. Chọn **7-Zip** → **Extract to "winlibs-..."**
3. Mở thư mục vừa giải nén
4. Copy thư mục `mingw64` vào ổ `C:\`
5. Đường dẫn cuối cùng: `C:\mingw64\bin\gcc.exe`

#### Bước 1.3: Thêm vào PATH

**Cách 1: Dùng GUI**

1. Nhấn **Windows + R**
2. Gõ: `sysdm.cpl`
3. Nhấn **Enter**
4. Chọn tab **Advanced**
5. Click **Environment Variables**
6. Trong phần **System variables**, tìm và chọn **Path**
7. Click **Edit**
8. Click **New**
9. Gõ: `C:\mingw64\bin`
10. Click **OK** → **OK** → **OK**

**Cách 2: Dùng PowerShell (Nhanh hơn)**

1. Nhấn **Windows + X**
2. Chọn **Windows PowerShell (Admin)**
3. Gõ lệnh:
```powershell
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\mingw64\bin", "Machine")
```
4. Nhấn **Enter**

#### Bước 1.4: Kiểm tra

1. **Đóng tất cả Terminal/PowerShell đang mở**
2. Mở **Command Prompt MỚI**:
   - Nhấn **Windows + R**
   - Gõ: `cmd`
   - Nhấn **Enter**
3. Gõ lệnh:
```bash
gcc --version
```
4. Nếu hiện thông tin như này → **Thành công!** ✅
```
gcc (GCC) 13.2.0
Copyright (C) 2023 Free Software Foundation, Inc.
...
```

---

### Cách B: Dùng script tự động (Nâng cao)

**Chỉ dùng nếu bạn đã cài 7-Zip!**

1. Chuột phải vào file `cai-gcc-tu-dong.ps1`
2. Chọn **Run with PowerShell**
3. Nếu báo lỗi, chạy PowerShell as Admin:
   - Nhấn **Windows + X**
   - Chọn **Windows PowerShell (Admin)**
   - Gõ:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process -Force
   cd "D:\CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT - SOF307\hovanphuoc.c"
   .\cai-gcc-tu-dong.ps1
   ```

---

## 🔌 BƯỚC 2: CÀI EXTENSION VS CODE

### Bước 2.1: Cài Extension C/C++

1. Mở VS Code
2. Nhấn **Ctrl + Shift + X** (mở Extensions)
3. Trong ô tìm kiếm, gõ: `C/C++`
4. Tìm extension **C/C++** của **Microsoft** (có logo Microsoft)
5. Click **Install**
6. Đợi cài xong

### Bước 2.2: Cài Extension Code Runner (Tùy chọn)

1. Vẫn ở tab Extensions
2. Gõ: `Code Runner`
3. Tìm extension **Code Runner** của **Jun Han**
4. Click **Install**

### Bước 2.3: Reload VS Code

1. Nhấn **Ctrl + Shift + P**
2. Gõ: `Reload Window`
3. Nhấn **Enter**

---

## ▶️ BƯỚC 3: CHẠY CHƯƠNG TRÌNH

### Cách 1: Dùng Terminal (Khuyến nghị)

#### Bước 3.1: Mở Terminal

1. Trong VS Code, nhấn **Ctrl + `** (phím backtick, bên cạnh số 1)
2. Hoặc: Menu **Terminal** → **New Terminal**
3. Terminal sẽ hiện ở phía dưới

#### Bước 3.2: Kiểm tra đường dẫn

Trong Terminal, gõ:
```bash
pwd
```
Hoặc:
```bash
cd
```

Đảm bảo bạn đang ở thư mục chứa file `hovanphuoc.c`

Nếu không đúng, di chuyển đến thư mục:
```bash
cd "D:\CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT - SOF307\hovanphuoc.c"
```

#### Bước 3.3: Biên dịch

Gõ lệnh:
```bash
gcc hovanphuoc.c -o hovanphuoc.exe
```

Nhấn **Enter**

Nếu không có lỗi → **Biên dịch thành công!** ✅

#### Bước 3.4: Chạy chương trình

Gõ lệnh:
```bash
.\hovanphuoc.exe
```

Hoặc:
```bash
hovanphuoc.exe
```

Nhấn **Enter**

**Chương trình sẽ chạy!** 🎉

#### Bước 3.5: Gộp 2 lệnh (Nhanh hơn)

Lần sau, bạn chỉ cần gõ 1 lệnh:
```bash
gcc hovanphuoc.c -o hovanphuoc.exe && .\hovanphuoc.exe
```

---

### Cách 2: Dùng Code Runner

**Nếu đã cài extension Code Runner:**

1. Mở file `hovanphuoc.c`
2. Nhấn **Ctrl + Alt + N**
3. Hoặc: Chuột phải → **Run Code**
4. Chương trình sẽ chạy!

---

### Cách 3: Dùng Tasks (Chuyên nghiệp)

**Đã cấu hình sẵn trong `.vscode/tasks.json`**

1. Mở file `hovanphuoc.c`
2. Nhấn **Ctrl + Shift + B**
3. Chọn **Build and Run**
4. Chương trình sẽ biên dịch và chạy!

---

## 🎬 DEMO HOÀN CHỈNH

```
Bước 1: Mở VS Code
Bước 2: Mở file hovanphuoc.c
Bước 3: Nhấn Ctrl + ` (mở Terminal)
Bước 4: Gõ: gcc hovanphuoc.c -o hovanphuoc.exe && .\hovanphuoc.exe
Bước 5: Nhấn Enter
Bước 6: Chương trình chạy!

=== QUẢN LÝ NHÂN SỰ CÔNG TY ===
1. Thêm nhân viên
2. Xuất danh sách
...
Chọn: _
```

---

## 🐛 XỬ LÝ LỖI THƯỜNG GẶP

### Lỗi 1: 'gcc' is not recognized

**Nguyên nhân:** Chưa cài GCC hoặc chưa thêm vào PATH

**Giải pháp:**
1. Kiểm tra đã cài GCC chưa: `gcc --version`
2. Nếu chưa → Làm lại **Bước 1**
3. Nếu đã cài nhưng vẫn lỗi:
   - Kiểm tra PATH: `echo %PATH%`
   - Đảm bảo có `C:\mingw64\bin`
   - Đóng và mở lại Terminal MỚI
   - Đóng và mở lại VS Code

### Lỗi 2: #include errors detected

**Nguyên nhân:** VS Code chưa biết đường dẫn thư viện

**Giải pháp:**
1. Nhấn **Ctrl + Shift + P**
2. Gõ: `Reload Window`
3. Nhấn Enter
4. Lỗi sẽ mất

**Hoặc:**
- Lỗi này không ảnh hưởng đến việc chạy code
- Bạn vẫn có thể biên dịch và chạy bình thường

### Lỗi 3: Permission denied

**Nguyên nhân:** File đang được mở bởi chương trình khác

**Giải pháp:**
1. Đóng chương trình `hovanphuoc.exe` nếu đang chạy
2. Thử lại lệnh biên dịch

### Lỗi 4: No such file or directory

**Nguyên nhân:** Đang ở sai thư mục

**Giải pháp:**
1. Kiểm tra đường dẫn: `pwd` hoặc `cd`
2. Di chuyển đến đúng thư mục:
```bash
cd "D:\CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT - SOF307\hovanphuoc.c"
```

### Lỗi 5: Tiếng Việt bị lỗi

**Giải pháp:**
Trước khi chạy, gõ:
```bash
chcp 65001
```

Hoặc thêm vào đầu hàm main():
```c
system("chcp 65001 > nul");
```

---

## 💡 MẸO HAY

### 1. Chạy lại nhanh
Sau khi chạy lần đầu, nhấn **mũi tên lên ↑** trong Terminal để lấy lại lệnh cũ

### 2. Clear Terminal
```bash
cls
```

### 3. Xem file đã tạo
```bash
dir
```

### 4. Biên dịch với cảnh báo
```bash
gcc -Wall hovanphuoc.c -o hovanphuoc.exe
```

### 5. Tạo alias (Shortcut)
Thêm vào PowerShell profile:
```powershell
function run { gcc hovanphuoc.c -o hovanphuoc.exe && .\hovanphuoc.exe }
```

Sau đó chỉ cần gõ: `run`

---

## 📋 CHECKLIST HOÀN CHỈNH

- [ ] Đã tải MinGW
- [ ] Đã giải nén vào `C:\mingw64`
- [ ] Đã thêm `C:\mingw64\bin` vào PATH
- [ ] Đã kiểm tra: `gcc --version` → OK
- [ ] Đã cài extension C/C++ trong VS Code
- [ ] Đã mở file `hovanphuoc.c` trong VS Code
- [ ] Đã mở Terminal (Ctrl + `)
- [ ] Đã biên dịch: `gcc hovanphuoc.c -o hovanphuoc.exe` → OK
- [ ] Đã chạy: `.\hovanphuoc.exe` → OK
- [ ] Chương trình chạy thành công! ✅

---

## 🎯 TÓM TẮT LỆNH

```bash
# Kiểm tra GCC
gcc --version

# Biên dịch
gcc hovanphuoc.c -o hovanphuoc.exe

# Chạy
.\hovanphuoc.exe

# Gộp lại (Khuyến nghị)
gcc hovanphuoc.c -o hovanphuoc.exe && .\hovanphuoc.exe
```

---

## 🆘 VẪN KHÔNG CHẠY ĐƯỢC?

### Thử cách này:

1. **Đóng VS Code hoàn toàn**
2. **Mở Command Prompt** (không phải PowerShell):
   - Windows + R → `cmd` → Enter
3. **Di chuyển đến thư mục:**
   ```bash
   cd "D:\CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT - SOF307\hovanphuoc.c"
   ```
4. **Chạy:**
   ```bash
   gcc hovanphuoc.c -o hovanphuoc.exe
   hovanphuoc.exe
   ```

Nếu chạy được trong Command Prompt → Vấn đề ở cấu hình VS Code  
Nếu không chạy được → Vấn đề ở GCC

---

## 📞 CẦN TRỢ GIÚP THÊM?

Nếu vẫn gặp vấn đề:
1. Chụp màn hình lỗi
2. Kiểm tra: `gcc --version`
3. Kiểm tra: `where gcc`
4. Thử chạy trong Command Prompt
5. Nếu không được, dùng Dev-C++ (đơn giản hơn)

---

## ✅ HOÀN TẤT!

Sau khi làm theo hướng dẫn này, bạn đã có thể:
- ✅ Biên dịch code C trong VS Code
- ✅ Chạy chương trình
- ✅ Debug và phát triển tiếp

**Chúc bạn thành công!** 🎉
