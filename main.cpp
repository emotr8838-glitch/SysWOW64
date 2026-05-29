// GDI MULTI EFFECT + BYTEBEAT + FAKE BSOD
// ESC = keluar
//
// Effects:
// 1 = Rectangle Chaos
// 2 = SetPixel Chaos
// 3 = Screen Shake
// 4 = Eclipse
// 5 = Rotozoomer
//
// Setelah selesai:
// Fake BSOD fullscreen
//
// Compile:
// g++ main.cpp -lgdi32 -lwinmm -mwindows -o main.exe

#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#pragma comment(lib, "winmm.lib")

bool running = true;
int currentEffect = 1;
bool showBSOD = false;

// =====================================================
// TIMER
// =====================================================

DWORD WINAPI TimerThread(LPVOID lpParam) {

    while (running && !showBSOD) {

        Sleep(35000);

        currentEffect++;

        if (currentEffect > 5) {

            showBSOD = true;

            break;
        }
    }

    return 0;
}

// =====================================================
// MOUSE CHAOS
// =====================================================

void MouseChaos(HDC hdc) {

    POINT cursor;

    GetCursorPos(&cursor);

    // =========================================
    // GERAK RANDOM
    // =========================================

    int moveX =
        rand() % 41 - 20;

    int moveY =
        rand() % 41 - 20;

    int newX =
        cursor.x + moveX;

    int newY =
        cursor.y + moveY;

    SetCursorPos(
        newX,
        newY
    );

    // =========================================
    // ICON MERAH PAS DI CURSOR
    // =========================================

    int size = 64;

    int left =
        newX - size / 2;

    int top =
        newY - size / 2;

    int right =
        newX + size / 2;

    int bottom =
        newY + size / 2;

    // =========================================
    // LINGKARAN MERAH
    // =========================================

    HBRUSH redBrush =
        CreateSolidBrush(
            RGB(220,30,30)
        );

    HPEN borderPen =
        CreatePen(
            PS_SOLID,
            4,
            RGB(255,255,255)
        );

    SelectObject(
        hdc,
        redBrush
    );

    SelectObject(
        hdc,
        borderPen
    );

    Ellipse(
        hdc,
        left,
        top,
        right,
        bottom
    );

    // =========================================
    // GLOSSY HIGHLIGHT
    // =========================================

    HBRUSH shineBrush =
        CreateSolidBrush(
            RGB(255,120,120)
        );

    SelectObject(
        hdc,
        shineBrush
    );

    Ellipse(
        hdc,
        left + 10,
        top + 6,
        left + 34,
        top + 24
    );

    // =========================================
    // X PUTIH DI TENGAH
    // =========================================

    HPEN whitePen =
        CreatePen(
            PS_SOLID,
            8,
            RGB(255,255,255)
        );

    SelectObject(
        hdc,
        whitePen
    );

    MoveToEx(
        hdc,
        newX - 12,
        newY - 12,
        NULL
    );

    LineTo(
        hdc,
        newX + 12,
        newY + 12
    );

    MoveToEx(
        hdc,
        newX + 12,
        newY - 12,
        NULL
    );

    LineTo(
        hdc,
        newX - 12,
        newY + 12
    );

    DeleteObject(redBrush);
    DeleteObject(borderPen);
    DeleteObject(shineBrush);
    DeleteObject(whitePen);
}

// =====================================================
// FAKE BSOD
// =====================================================

void DrawBSOD(HDC hdc) {

    RECT screen = {
        0,
        0,
        1920,
        1080
    };

    HBRUSH blue =
        CreateSolidBrush(
            RGB(0,120,215)
        );

    FillRect(
        hdc,
        &screen,
        blue
    );

    DeleteObject(blue);

    SetBkMode(
        hdc,
        TRANSPARENT
    );

    SetTextColor(
        hdc,
        RGB(255,255,255)
    );

    HFONT sadFont =
        CreateFont(
            120,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH,
            "Segoe UI"
        );

    SelectObject(
        hdc,
        sadFont
    );

    TextOut(
        hdc,
        80,
        80,
        ":(",
        2
    );

    HFONT textFont =
        CreateFont(
            36,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH,
            "Segoe UI"
        );

    SelectObject(
        hdc,
        textFont
    );

    const char* line1 =
        "Your PC ran into a problem and needs to restart.";

    const char* line2 =
        "We're just collecting some error info, and then we'll restart for you.";

    const char* line3 =
        "100% complete";

    const char* line4 =
        "Stop code: CRITICAL_PROCESS_DIED";

    TextOut(
        hdc,
        80,
        250,
        line1,
        lstrlen(line1)
    );

    TextOut(
        hdc,
        80,
        320,
        line2,
        lstrlen(line2)
    );

    TextOut(
        hdc,
        80,
        420,
        line3,
        lstrlen(line3)
    );

    TextOut(
        hdc,
        80,
        520,
        line4,
        lstrlen(line4)
    );

    DeleteObject(sadFont);
    DeleteObject(textFont);
}

// =====================================================
// GDI EFFECTS
// =====================================================

DWORD WINAPI GDIEffectThread(LPVOID lpParam) {

    HDC hdc =
        GetDC(NULL);

    int sw = 1920;
    int sh = 1080;

    int angle = 0;

    while (running) {

        // fake bsod

        if (showBSOD) {

            DrawBSOD(hdc);

            Sleep(10);

            continue;
        }

        // mouse chaos

        MouseChaos(hdc);

        // =============================================
        // GDI 1 - RECTANGLE
        // =============================================

        if (currentEffect == 1) {

            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;

            HBRUSH brush =
                CreateSolidBrush(
                    RGB(r,g,b)
                );

            SelectObject(
                hdc,
                brush
            );

            int x =
                rand() % sw;

            int y =
                rand() % sh;

            int w =
                rand() % 400;

            int h =
                rand() % 400;

            Rectangle(
                hdc,
                x,
                y,
                x + w,
                y + h
            );

            DeleteObject(brush);

            Sleep(10);
        }

        // =============================================
        // GDI 2 - SETPIXEL
        // =============================================

        else if (currentEffect == 2) {

            for (int i = 0; i < 2000; i++) {

                int x =
                    rand() % sw;

                int y =
                    rand() % sh;

                int r =
                    rand() % 256;

                int g =
                    rand() % 256;

                int b =
                    rand() % 256;

                SetPixel(
                    hdc,
                    x,
                    y,
                    RGB(r,g,b)
                );
            }

            Sleep(1);
        }

        // =============================================
        // GDI 3 - SHAKE
        // =============================================

        else if (currentEffect == 3) {

            int shakeX =
                rand() % 20 - 10;

            int shakeY =
                rand() % 20 - 10;

            BitBlt(
                hdc,
                shakeX,
                shakeY,
                sw,
                sh,
                hdc,
                0,
                0,
                SRCCOPY
            );

            Sleep(5);
        }

        // =============================================
        // GDI 4 - ECLIPSE
        // =============================================

        else if (currentEffect == 4) {

            int size =
                rand() % 500;

            int x =
                rand() % sw;

            int y =
                rand() % sh;

            int r =
                rand() % 256;

            int g =
                rand() % 256;

            int b =
                rand() % 256;

            HBRUSH brush =
                CreateSolidBrush(
                    RGB(r,g,b)
                );

            SelectObject(
                hdc,
                brush
            );

            Ellipse(
                hdc,
                x,
                y,
                x + size,
                y + size
            );

            DeleteObject(brush);

            Sleep(5);
        }

        // =============================================
        // GDI 5 - ROTOZOOMER
        // =============================================

        else if (currentEffect == 5) {

            int zoom =
                (int)(
                    sin(angle * 0.05)
                    * 40
                );

            int rotX =
                (int)(
                    cos(angle * 0.03)
                    * 20
                );

            int rotY =
                (int)(
                    sin(angle * 0.03)
                    * 20
                );

            StretchBlt(
                hdc,
                -zoom + rotX,
                -zoom + rotY,
                sw + zoom * 2,
                sh + zoom * 2,
                hdc,
                0,
                0,
                sw,
                sh,
                SRCCOPY
            );

            angle++;

            Sleep(5);
        }
    }

    ReleaseDC(NULL, hdc);

    return 0;
}

// =====================================================
// AUDIO
// =====================================================

DWORD WINAPI AudioThread(LPVOID lpParam) {

    HWAVEOUT hWaveOut;

    WAVEFORMATEX wfx = {
        WAVE_FORMAT_PCM,
        1,
        8000,
        8000,
        1,
        8,
        0
    };

    waveOutOpen(
        &hWaveOut,
        WAVE_MAPPER,
        &wfx,
        0,
        0,
        CALLBACK_NULL
    );

    while (running && !showBSOD) {

        const int samples =
            8000 * 35;

        char* buffer =
            new char[samples];

        if (currentEffect == 1) {

            for (int t = 0; t < samples; t++) {

                buffer[t] =
                    (t * ((t >> 5 | t >> 8)))
                    >> (t >> 16);
            }
        }

        else if (currentEffect == 2) {

            for (int t = 0; t < samples; t++) {

                buffer[t] =
                    10 * (t >> 7 | t | t >> 6)
                    + 4 * (t & t >> 13 | t >> 6);
            }
        }

        else if (currentEffect == 3) {

            for (int t = 0; t < samples; t++) {

                buffer[t] =
                    t * (t & 16384 ? 7 : 5)
                    * (3 + (3 & t >> 14))
                    >> (3 & t >> 9)
                    | t >> 6;
            }
        }

        else if (currentEffect == 4) {

            for (int t = 0; t < samples; t++) {

                buffer[t] =
                    t * (
                        ((t >> 12 | t >> 8)
                        & 63
                        & t >> 4)
                    );
            }
        }

        else if (currentEffect == 5) {

            for (int t = 0; t < samples; t++) {

                buffer[t] =
                    ((t >> 4)
                    * (t >> 5))
                    | (t << 3)
                    | (t >> 2);
            }
        }

        WAVEHDR header;

        ZeroMemory(
            &header,
            sizeof(WAVEHDR)
        );

        header.lpData =
            buffer;

        header.dwBufferLength =
            samples;

        waveOutPrepareHeader(
            hWaveOut,
            &header,
            sizeof(WAVEHDR)
        );

        waveOutWrite(
            hWaveOut,
            &header,
            sizeof(WAVEHDR)
        );

        Sleep(35000);

        waveOutReset(hWaveOut);

        waveOutUnprepareHeader(
            hWaveOut,
            &header,
            sizeof(WAVEHDR)
        );

        delete[] buffer;
    }

    waveOutClose(hWaveOut);

    return 0;
}

// =====================================================
// MAIN
// =====================================================

int WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPrev,
    LPSTR lpCmdLine,
    int nShowCmd
) {

    srand(
        GetTickCount()
    );

    HANDLE timer =
        CreateThread(
            NULL,
            0,
            TimerThread,
            NULL,
            0,
            NULL
        );

    HANDLE gdi =
        CreateThread(
            NULL,
            0,
            GDIEffectThread,
            NULL,
            0,
            NULL
        );

    HANDLE audio =
        CreateThread(
            NULL,
            0,
            AudioThread,
            NULL,
            0,
            NULL
        );

    while (running) {

        if (
            GetAsyncKeyState(
                VK_ESCAPE
            ) & 0x8000
        ) {
            running = false;
        }

        Sleep(10);
    }

    WaitForSingleObject(
        timer,
        INFINITE
    );

    WaitForSingleObject(
        gdi,
        INFINITE
    );

    WaitForSingleObject(
        audio,
        INFINITE
    );

    CloseHandle(timer);
    CloseHandle(gdi);
    CloseHandle(audio);

    return 0;
}