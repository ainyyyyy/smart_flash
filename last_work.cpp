#include <iostream>
#include <windows.h>
#include "tchar.h"

int main4(){
    std::string strr;
    //strr += '\r';
    //strr += '\n';
    char *cstr;
  DWORD TWR = 0;

    char input[1];
    int count = 0;
    std::string str = "";
    DWORD bytesRead, bytesWrite;
    HANDLE hComm, hSerial;

  LPCTSTR sPortName = _T("COM5");
hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    // hComm = CreateFileA("\\\\.\\COM3",
    // 		    GENERIC_READ | GENERIC_WRITE,
    // 		    0,
    // 		    NULL,
    // 		    OPEN_EXISTING,
    // 		    0,
    // 		    NULL);
    // if (hComm == INVALID_HANDLE_VALUE){
    // 	printf("Error in opening serial port");
    // 	std::cout << std::endl;
    // }
    // else{
    // 	printf("Opening serial port successful");
    // 	std::cout << std::endl;
    // }

//watching files

unsigned __int64 TotalNumberOfBytes;
unsigned __int64 TotalNumberOfFreeBytes;
ULARGE_INTEGER free;

/*
HANDLE file = CreateFile("D:/",
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL);

OVERLAPPED overlapped;
  overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

uint8_t change_buf[1024];

ReadDirectoryChangesW(
    file, change_buf, 1024, TRUE,
    FILE_NOTIFY_CHANGE_FILE_NAME  |
    FILE_NOTIFY_CHANGE_DIR_NAME   |
    FILE_NOTIFY_CHANGE_LAST_WRITE,
    NULL, &overlapped, NULL);
*/
char *path = "C:\\Users\\User\\Desktop\\project\\over2";
  std::cout << "watching for changes... " << path << std::endl;

  HANDLE file = CreateFileA(path,
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL);
  //assert(file != INVALID_HANDLE_VALUE);
  OVERLAPPED overlapped;
  overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

  uint8_t change_buf[1024];
  BOOL success = ReadDirectoryChangesW(
    file, change_buf, 1024, TRUE,
    FILE_NOTIFY_CHANGE_FILE_NAME  |
    FILE_NOTIFY_CHANGE_DIR_NAME   |
    FILE_NOTIFY_CHANGE_LAST_WRITE,
    NULL, &overlapped, NULL);

  while (true) {
    DWORD result = WaitForSingleObject(overlapped.hEvent, 0);

    if (result == WAIT_OBJECT_0) {
      DWORD bytes_transferred;
      GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE);

      FILE_NOTIFY_INFORMATION *event = (FILE_NOTIFY_INFORMATION*)change_buf;

      for (;;) {
        //DWORD name_len = event->FileNameLength;

        //switch (event->Action) {
    if((event->Action != FILE_ACTION_ADDED) and (event->Action != FILE_ACTION_RENAMED_OLD_NAME)){
    GetDiskFreeSpaceExA(path,&free,
    (PULARGE_INTEGER)&TotalNumberOfBytes,
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes);
    // DWORD dbcc_size = 19;
    strr = std::to_string(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
    // char *buffer = &(strr[0]);
    char *cstr = &(strr[0]);
    // DWORD data = DWORD(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
    char buffer[20];
    char str[20];
    strcpy(buffer, cstr);
    // itoa(data,buffer,20);
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if(!GetCommState(hSerial, &dcbSerialParams)){
        std::cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
        std::cout << "error setting serial port state\n";
    }
    // char data[] = {dbcc_size};  // строка для передачи
    DWORD dwSize = sizeof(buffer);   // размер этой строки
    DWORD dwBytesWritten;    // тут будет количество собственно переданных байт
    DWORD dwBytesRead;
    std::cout<<buffer<<"       write"<<std::endl<<std::endl;
    BOOL iRet = WriteFile(hSerial, buffer, dwSize, &dwBytesWritten, NULL);
    // std::cout<<iRet<<std::endl;
    if (iRet){
    BOOL oRet = ReadFile(hSerial, str, dwSize, &dwBytesRead, NULL);
    // std::cout<<oRet<<std::endl;
    std::cout<<str<<"       read"<<std::endl<<std::endl;

    }



    ///////////////DWORD dbcc_size = DWORD(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
    ///////////////char buffer[20];
    ///////////////itoa(dbcc_size,buffer,20);
        // // strr = "";
        // strr = std::to_string(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
        // strr += '\n';
    // strr += '\r';
    // delete [] cstr;
        // char *cstr = &(strr[0]);
    // char *cstr3 = "10";
    /////////////////std::cout << buffer << std::endl;
    ///////////////////char cstr2[50] = "";
    ////////////////////DWORD dwSize = sizeof(buffer);
        // strcpy(cstr, cstrr.c_str());
        ///////////////////WriteFile(hComm, buffer, dwSize, &bytesRead, NULL);
      // std::cout<<(strr.length() + 1)<<std::endl;
        // std::cout << strr.substr(0, strr.length() - 2) << " written" << std::endl;
        // str = "";
    // Sleep(500);
        // for(;;){
        ///////////////// ReadFile(hComm, cstr2, dwSize, &bytesRead, NULL);
        // 	str += input[0];
        // 	count++;
        // 	//std::cout << str << std::endl;
        // 	if (count >= 2){
        // 		if (str.substr(str.length() - 2, 2) == "\r\n"){
        // 			break;
        // 		}
        // 	}
        // }
        // count = 0;
        //////////////////std::cout << cstr2 << " read" << std::endl;

    }
    /*
          case FILE_ACTION_ADDED: {
        //wprintf(L"     Removed: %.*s\n", name_len, event->FileName);
            //std::cout << event->FileNameLength << std::endl;

        //std::cout << TotalNumberOfBytes << std::endl;
          } break;

          case FILE_ACTION_REMOVED: {
            //wprintf(L"     Removed: %.*s\n", name_len, event->FileName);
          } break;

          case FILE_ACTION_MODIFIED: {
              //wprintf(L"    Modified: %.*s\n", name_len, event->FileName);
          } break;

          case FILE_ACTION_RENAMED_OLD_NAME: {
            //wprintf(L"Renamed from: %.*s\n", name_len, event->FileName);
          } break;

          case FILE_ACTION_RENAMED_NEW_NAME: {
            wprintf(L"          to: %.*s\n", name_len, event->FileName);
          } break;

          default: {
            printf("Unknown action!\n");
          } break;
    */
        //}

        // Are there more events to handle?
        if (event->NextEntryOffset) {
          *((uint8_t**)&event) += event->NextEntryOffset;
        } else {
          break;
        }
      }

      // Queue the next event
      BOOL success = ReadDirectoryChangesW(
            file, change_buf, 1024, TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME  |
            FILE_NOTIFY_CHANGE_DIR_NAME   |
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            NULL, &overlapped, NULL);

    }

    // Do other loop stuff here...
  }
    CloseHandle(hComm);
}
