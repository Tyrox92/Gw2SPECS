/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <locale>
#include <stdint.h>
#include <string>
#include <Windows.h>

namespace GW2 {

    class MumbleLink {
    public:
        #ifdef _WIN32
            #include <windows.h>
        #else
            #include <sys/mman.h>
            #include <fcntl.h> /* For O_* constants */
        #endif // _WIN32


        struct LinkedMem {
        #ifdef _WIN32
            UINT32	uiVersion;
            DWORD	uiTick;
        #else
            uint32_t uiVersion;
            uint32_t uiTick;
        #endif
            float	fAvatarPosition[3];
            float	fAvatarFront[3];
            float	fAvatarTop[3];
            wchar_t	name[256];
            float	fCameraPosition[3];
            float	fCameraFront[3];
            float	fCameraTop[3];
            wchar_t	identity[256];
        #ifdef _WIN32
            UINT32	context_len;
        #else
            uint32_t context_len;
        #endif
            unsigned char context[256];
            wchar_t description[2048];
        };

        LinkedMem *lm = NULL;

        inline bool initLink() {
                lm = NULL;

                HANDLE hMapObject = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"MumbleLink");
                if (hMapObject == NULL) {
                    return false;
                }

                lm = (LinkedMem*)MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
                if (lm == NULL) {
                    CloseHandle(hMapObject);
                    hMapObject = NULL;
                    return false;
                }

                return true;
            }

/*
        void initMumble() {

        #ifdef _WIN32
            HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
            if (hMapObject == NULL)
                return;

            lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
            if (lm == NULL) {
                CloseHandle(hMapObject);
                hMapObject = NULL;
                return;
            }
        #else
            char memname[256];
            snprintf(memname, 256, "/MumbleLink.%d", getuid());

            int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

            if (shmfd < 0) {
                return;
            }

            lm = (LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

            if (lm == (void *)(-1)) {
                lm = NULL;
                return;
            }
        #endif
        }
        */

        inline QString getIdent(){
             if (! lm)
                     return " ";

             try{
             QString infoString = QString::fromWCharArray(lm->identity);
             //qDebug() << infoString;

             infoString.remove(QChar('\"'), Qt::CaseInsensitive).remove(QChar('{'));
             QStringList infoList = infoString.split(",", QString::SkipEmptyParts);
             return infoList[0].split(":")[1].simplified() + ";" + infoList[1].split(":")[1].simplified();
             }catch(std::string &msg){
                 return "0:0";
             }

         }

         inline bool updateMumble() {
             if (! lm)
                     return false;

                 if(lm->uiVersion != 2) {
                     wcsncpy(lm->name, L"TestLink", 256);
                     wcsncpy(lm->description, L"TestLink is a test of the Link plugin.", 2048);
                     lm->uiVersion = 2;
                 }
                 lm->uiTick++;

                 // Left handed coordinate system.
                 // X positive towards "right".
                 // Y positive towards "up".
                 // Z positive towards "front".
                 //
                 // 1 unit = 1 meter

                 // Unit vector pointing out of the avatar's eyes aka "At"-vector.
                 lm->fAvatarFront[0] = 0.0f;
                 lm->fAvatarFront[1] = 0.0f;
                 lm->fAvatarFront[2] = 1.0f;

                 // Unit vector pointing out of the top of the avatar's head aka "Up"-vector (here Top points straight up).
                 lm->fAvatarTop[0] = 0.0f;
                 lm->fAvatarTop[1] = 1.0f;
                 lm->fAvatarTop[2] = 0.0f;

                 // Position of the avatar (here standing slightly off the origin)
                 lm->fAvatarPosition[0] = 0.001f;
                 lm->fAvatarPosition[1] = 0.0f;
                 lm->fAvatarPosition[2] = 0.5f;

                 // Same as avatar but for the camera.
                 lm->fCameraPosition[0] = 0.0f;
                 lm->fCameraPosition[1] = 0.0f;
                 lm->fCameraPosition[2] = 0.0f;

                 lm->fCameraFront[0] = 0.0f;
                 lm->fCameraFront[1] = 0.0f;
                 lm->fCameraFront[2] = 1.0f;

                 lm->fCameraTop[0] = 0.0f;
                 lm->fCameraTop[1] = 1.0f;
                 lm->fCameraTop[2] = 0.0f;

                 // Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
                 wcsncpy(lm->identity, L"0:0,0:0,0:0", 256);
                 // Context should be equal for players which should be able to hear each other positional and
                 // differ for those who shouldn't (e.g. it could contain the server+port and team)
                 memcpy(lm->context, "ContextBlob\x00\x01\x02\x03\x04", 16);
                 lm->context_len = 16;
                 return true;
         }

    };
}
