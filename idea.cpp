/* Chuong trinh ma dich file bang he mat IDEA*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <dos.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long LONG;

#define IDEA_max 0x10001
#define IDEA_tich(x, y)((x * y) % IDEA_max)
#define do_dai_buff 0x8000

BYTE Buff[do_dai_buff];
WORD Block_vao[4], IDEA_khoa[52], IDEA_mam[8];

/* Ham tinh tich x*y (modulo 65537) */
WORD IDEA_nhan(WORD x, WORD y) {
    if ((x == 0) && (y == 0)) return 1;
    else {
        if (x == 0) return (WORD)(IDEA_max - y);
        else if (y == 0) return (WORD)(IDEA_max - x);
        else {
            LONG u = x, v = y;
            return (WORD) IDEA_tich(u, v);
        }
    }
}
/* Ham tinh x=nghich dao cua x (modulo 65537) */
WORD IDEA_nghich_dao(WORD x) {
    if (x < 2) return x;
    else {
        LONG a = IDEA_max;
        WORD q[200], r = x, b;
        BYTE i = 0;
        while (r > 1) {
            q[i] = a / r;
            b = r;
            r = a % r;
            a = b;
            i++;
        }
        WORD s = 1, t;
        r = 0;
        for (int j = i - 1; j >= 0; j--) {
            t = s;
            t *= q[j];
            t += r;
            r = s;
            s = t;
        }
        if (i & 1) {
            a = IDEA_max - t;
            t = a;
        }
        return t;
    }
}
/* Thu tuc lay 8 word thu k trong tep "IDEA.key" lam mam khoa (IDEA_mam) */
char IDEA_lay_khoa(short k) {
    char b;
    FILE * f;
    f = fopen("IDEA.key", "rb");
    long l = filelength(fileno(f)), m = 16;
    m *= k;
    if (l <= m) {
        b = 0;
        32
        Hệ mật IDEA
        gotoxy(36, 12);
        printf(" Khong lay duoc khoa");
    } else {
        b = 1;
        fseek(f, m, SEEK_SET);
        fread( & IDEA_mam, 16, 1, f);
    }
    fclose(f);
    return b;
}
/* Thu tuc tinh 52 word khoa ma hoa tu 8 word mam */
void IDEA_khoa_ma() {
    WORD tam[8];
    BYTE b, db, dw;
    for (int i = 0; i < 7; i++) {
        b = 25 * i;
        db = b & 15;
        dw = b >> 4;
        for (char j = 0; j < 8; j++)
            tam[j] = (IDEA_mam[(j + dw) & 7] >> db) ^ (IDEA_mam[(j + dw + 1) & 7] << (16 - db));
        if (i == 6) memcpy(IDEA_khoa + (8 * i), tam, 8);
        else memcpy(IDEA_khoa + (8 * i), tam, 16);
    }
}
/* Thu tuc tinh 52 word khoa giai ma tu 8 word mam */

void IDEA_khoa_dich() {
    WORD tam[8], k1[52];
    BYTE a, b, db, dw;
    for (int i = 0; i < 7; i++) {
        b = 25 * i;
        db = b & 15;
        dw = b >> 4;
        for (char j = 0; j < 8; j++)
            tam[j] = (IDEA_mam[(j + dw) & 7] >> db) ^ (IDEA_mam[(j + dw + 1) & 7] << (16 - db));
        if (i == 6) memcpy(k1 + (8 * i), tam, 8);
        else memcpy(k1 + (8 * i), tam, 16);
    }
    for (i = 0; i < 8; i++) {
        b = 6 * i;
        a = 48 - b;
        IDEA_khoa[b] = IDEA_nghich_dao(k1[a]);
        IDEA_khoa[b + 3] = IDEA_nghich_dao(k1[a + 3]);
        if (i) {
            IDEA_khoa[b + 1] = 0 - k1[a + 2];
            IDEA_khoa[b + 2] = 0 - k1[a + 1];
        } else {
            IDEA_khoa[b + 1] = 0 - k1[a + 1];
            IDEA_khoa[b + 2] = 0 - k1[a + 2];
        }
        b += 4;
        a = 50 - b;
        34
        Hệ mật IDEA
        IDEA_khoa[b] = k1[a];
        IDEA_khoa[b + 1] = k1[a + 1];
    }
    b = 6 * i;
    a = 48 - b;
    IDEA_khoa[b] = IDEA_nghich_dao(k1[a]);
    IDEA_khoa[b + 3] = IDEA_nghich_dao(k1[a + 3]);
    IDEA_khoa[b + 1] = 0 - k1[a + 1];
    IDEA_khoa[b + 2] = 0 - k1[a + 2];
}
/* Thu tuc ma hoa hoac giai ma theo he IDEA mot BLOCK 4 word */
void idea(WORD r) {
    WORD x[4], y[4], a, b;
    memcpy(y, Buff + r, 8);
    /* 8 vong dau */
    for (int i = 0; i < 8; i++) {
        BYTE j = 6 * i;
        x[0] = IDEA_nhan(y[0], IDEA_khoa[j]);
        x[1] = y[1] + IDEA_khoa[j + 1];
        x[2] = y[2] + IDEA_khoa[j + 2];
        x[3] = IDEA_nhan(y[3], IDEA_khoa[j + 3]);
        a = x[0] ^ x[2];
        b = x[1] ^ x[3];
        a = IDEA_nhan(a, IDEA_khoa[j + 4]);
        b += a;
        b = IDEA_nhan(b, IDEA_khoa[j + 5]);
        a += b;
        y[0] = x[0] ^ b;
        y[1] = x[2] ^ b;
        y[2] = x[1] ^ a;
        y[3] = x[3] ^ a;
    }
    /* vong cuoi */
    x[0] = IDEA_nhan(y[0], IDEA_khoa[48]);
    x[1] = y[2] + IDEA_khoa[49];
    x[2] = y[1] + IDEA_khoa[50];
    x[3] = IDEA_nhan(y[3], IDEA_khoa[51]);
    memcpy((Buff + r), x, 8);
}
/* Thu tuc ma hoa hoac giai ma theo he IDEA mot file */
/* Chu y ma hoa khi b=1 va giai ma khi b=0*/
void IDEA_ma_dich(char b, char file_in[50], char file_out[50]) {
    FILE * f_in, * f_out;
    LONG l, so_buff, so_byte_du;
    short ss;
    f_in = fopen(file_in, "rb");
    l = filelength(fileno(f_in));
    f_out = fopen(file_out, "wb");
    if (b) {
        randomize();
        ss = random(128);
        ss <<= 8;
        ss ^= random(256);
        IDEA_lay_khoa(ss);
        fwrite( & ss, 2, 1, f_out);
        IDEA_khoa_ma();
    } else {
        fread( & ss, 2, 1, f_in);
        IDEA_lay_khoa(ss);
        IDEA_khoa_dich();
        l -= 2;
    }
    so_buff = l >> 15;
    so_byte_du = l & 0x7fff;
    LONG i, j, k;
    if (so_byte_du > 7) {
        for (i = 0; i < so_buff; i++) {
            fread(Buff, do_dai_buff, 1, f_in);
            for (j = 0; j < 0x1000; j++) idea(8 * j);
            fwrite(Buff, do_dai_buff, 1, f_out);
        }
        fread(Buff, so_byte_du, 1, f_in);
        k = so_byte_du >> 3;
        if (b)
        {
                for (j = 0; j < k; j++) idea(8 * j);
                idea(so_byte_du - 8);
        } else {
            for (j = 0; j < k - 1; j++) idea(8 * j);
            idea(so_byte_du - 8);
            idea(so_byte_du - 8 - (so_byte_du & 7));
        }
        fwrite(Buff, so_byte_du, 1, f_out);
    } else {
        for (i = 0; i < so_buff - 1; i++) {
            fread(Buff, do_dai_buff, 1, f_in);
            for (j = 0; j < 0x1000; j++) idea(8 * j);
            fwrite(Buff, do_dai_buff, 1, f_out);
        }
        fread(Buff, do_dai_buff - 8, 1, f_in);
        for (j = 0; j < 0x1000 - 1; j++) idea(8 * j);
        fwrite(Buff, do_dai_buff - 8, 1, f_out);
        so_byte_du += 8;
        fread(Buff, so_byte_du, 1, f_in);
        if (b) {
            idea(0);
            idea(so_byte_du - 8);
        } else {
            idea(so_byte_du - 8);
            idea(so_byte_du - 8 - (so_byte_du & 7));
        }
        fwrite(Buff, so_byte_du, 1, f_out);
    }
    fcloseall();
}
char nhan_dung(char x[50]) {
    char m0[10] = "ma", m1[10] = "MA", m2[10] = "dich", m3[10] = "DICH";
    if ((!strncmp(x, m0, 10)) || (!strncmp(x, m1, 10))) return 1;
    else
    if ((!strncmp(x, m2, 10)) || (!strncmp(x, m3, 10))) return 0;
    else return 2;
}
void main() {
    textbackground(0);
    textcolor(15);
    clrscr();
    short hd, td;
    float tt;
    long far * time = (long far * ) MK_FP(0, 0x46C);
    long time1, time2;
    char st[50], f_in[50], f_out[50], ma_dich[5];
    gotoxy(20, 2);
    textbackground(4);
    textcolor(14);
    clreol();
    printf(" CHUONG TRINH MA DICH FILE THEO HE MA IDEA ");
    textbackground(0);
    clreol();
    printf("\n");
    printf("\n");
    textbackground(1);
    textcolor(15);
    clreol();
    printf(" Thuc hien MA hay DICH : ");
    textbackground(0);
    textcolor(10);
    clreol();
    printf(" ");
    hd = wherex();
    td = wherey();
    scanf("%s", ma_dich);
    while (!((nhan_dung(ma_dich) == 1) || (nhan_dung(ma_dich) == 0))) {
        printf(" NHAP sai dieu kien, De nghi nhap lai ");
        gotoxy(hd, td);
        textcolor(10);
        clreol();
        scanf("%s", ma_dich);
    }
    printf(" ");
    gotoxy(1, td + 1);
    if (nhan_dung(ma_dich) == 1) {
        textbackground(7);
        textcolor(1);
        clreol();
        printf(" NHAP TEN FILE CAN MA: ");
        textbackground(0);
        textcolor(10);
        clreol();
        printf(" ");
        scanf("%s", f_in);
        textbackground(7);
        textcolor(1);
        clreol();
        printf(" DAT TEN CHO FILE MA : ");
        textbackground(0);
        textcolor(10);
        clreol();
        printf(" ");
        scanf("%s", f_out);
    } else {
        textbackground(7);
        textcolor(1);
        clreol();
        printf(" NHAP TEN FILE CAN DICH: ");
        textbackground(0);
        textcolor(10);
        clreol();
        printf(" ");
        scanf("%s", f_in);
        textbackground(7);
        textcolor(1);
        clreol();
        printf(" DAT TEN CHO FILE DICH : ");
        textbackground(0);
        textcolor(10);
        clreol();
        printf(" ");
        scanf("%s", f_out);
    }
    time1 = * time;
    IDEA_ma_dich(nhan_dung(ma_dich), f_in, f_out);
    time2 = * time;
    tt = ((time2 - time1) / 18.2);
    textbackground(4);
    textcolor(14);
    clreol();
    if ((nhan_dung(ma_dich) == 1)) printf(" Thoi gian ma hoa file la: %4.4f giay; ", tt);
    else printf(" Thoi gian dich file la: %4.4f giay; ", tt);
    textbackground(0);
    textcolor(15);
    clreol();
    printf("\n");
    getch();
}