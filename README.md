##  개요   

C를 이용한 Tcp / Ip Socket 프로그래밍 연습입니다.
윈도우 운영체제 환경에서 작업하였습니다.  


#### 소스를 돌리기 위해 Visual Studio 상에서 다음 작업을 합니다.


1. 프로젝트 -> 속성 -> 구성 속성 -> 링커 -> 입력 -> 추가 종속성 -> ws2_32.lib 추가   


2. 프로젝트 -> 속성 -> 구성 속성 -> C/C++ -> 일반 -> SDL 검사 -> 아니오 체크   


3. Debug -> Release 모드로 변경(실행파일을 만들기 위함)   


4. cmd에서 Release 폴더로 이동한 후에 `실행파일명 포트번호` 입력      


   
## 네트워크 프로그래밍과 소켓   

네트워크로 연결되어 있는 서로 다른 두 컴퓨터가 데이터를 주고받을 수 있도록 하는 것이 `네트워크 프로그래밍`이다.    

프로그래머의 입장에서 이미 대부분의 컴퓨터가 인터넷이라는 거대한 네트워크로 연결되어 있으니,
물리적인 연결은 신경 쓸 필요가 없다.   

때문에 이 물리적인 연결을 기반으로 하는 소프트웨어적인 데이터의 송수신 방법만 고민하면 된다.   
그런데 이 역시도 고민할 필요가 없다.   

운영체제에서 `소켓(Socket)`이라는 것을 제공하기 때문이다.   
이는 물리적으로 연결된 네트워크상에서의 데이터 송수신에 사용할 수 있는 소프트웨어적인 장치를 의미한다.   
때문에 데이터 송수신의 원리를 이론적으로 잘 몰라도, 소켓이라는 것을 이용해서 데이터를 주고 받을 수 있다.   
그래서 네트워크 프로그래밍을 `소켓 프로그래밍`이라고도 한다.   

정리하자면, 프로그래밍에서의 `소켓`은 네트워크 망의 연결에 사용하는 도구이다.   
`소켓`은 네트워크를 통한 두 컴퓨터의 연결을 의미하기도 한다.      


   
## 서버 소켓의 개요   

- 1단계. 소켓 생성 `socket` 함수 호출   
- 2단계. IP주소와 PORT번호 할당 `bind` 함수 호출   
- 3단계. 연결요청 가능상태로 변경 `listen` 함수 호출   
- 4단계. 연결요청에 대한 수락 `accept` 함수 호출      


   
## 리눅스 기반 파일 조작하기   

리눅스는 소켓을 파일의 일종으로 구분한다.   
따라서 파일 입출력 함수를 소켓 입출력에, 다시 말해서 네트워크상에서의 데이터 송수신에 사용할 수 있다.      


#### 파일 디스크립터   

파일 디스크립터란 시스템으로부터 할당 받은 파일 또는 소켓에 부여된 정수를 의미한다.      


#### 파일 열기   

`int open(char *path, int flag);`   

- path : 파일 이름을 나타내는 문자열의 주소 값 전달.   
- flag : 파일의 오픈 모드 정보 전달.   
- 성공 시 파일 디스크립터, 실패 시 -1 반환      


#### 파일 닫기   

`int close(int fd);`   

- fd : 닫고자 하는 파일 또는 소켓의 파일 디스크립터 전달.   
- 성공 시 0, 실패 시 -1 반환      


#### 파일에 데이터 쓰기   

`ssize_t write(int fd, const void * buf, size_t nbytes);`   

- fd : 데이터 전송 대상을 나타내는 파일 디스크립터 전달.   
- buf : 전송할 데이터가 저장된 버퍼의 주소 값 전달.   
- nbytes : 전송할 데이터의 바이트 수 전달.   
- 성공 시 전달한 바이트 수, 실패 시 -1 반환      


#### 파일에 저장된 데이터 읽기   

`ssize_t read(int fd, void *buf, size_t nbytes);`   

- fd : 데이터 수신 대상을 나타내는 파일 디스크립터 전달.   
- buf : 수신한 데이터를 저장할 버퍼의 주소 값 전달.   
- nbytes : 수신할 최대 바이트 수 전달.   
- 성공 시 수신한 바이트 수(단, 파일의 끝을 만나면 0), 실패 시 -1 반환      


   
## 윈도우 기반의 서버 소켓 관련 함수   


#### 윈속(winsock)의 초기화   

윈속 프로그래밍을 할 때에는 반드시 `WSAStartup` 함수를 호출해서,   
프로그램에서 요구하는 윈도우 소켓의 버전을 알리고,   
해당 버전을 지원하는 라이브러리의 초기화 작업을 진행해야 한다.      

`int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);`   

- wVersionRequested : 프로그래머가 사용할 윈속의 버전정보 전달.   
- lpWSAData : WSADATA라는 구조체 변수의 주소 값 전달.   
- 성공 시 0, 실패 시 0이 아닌 에러코드 값 반환   

위의 함수를 호출하는 아래의 코드는 윈속 기반의 프로그래밍에서는 공식과 같이 등장한다.      

int main(int argc, char* argv[]) {   

  WSADATA wsaData;      
   
   
  ...   
   
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");   
   

  ...   
   
  return 0;   
}   
   
다음은 위의 윈속 라이브러리의 해제에 사용되는 함수이다.   

`int WSACleanup(void);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환      


#### 소켓 생성   

`SOCKET socket(int af, int type, int protocol);`   

- 성공 시 소켓 핸들, 실패 시 INVALID_SOCKET 반환   


#### IP주소와 PORT번호 할당   

`int bind(SOCKET s, const struct sockaddr * name, int namelen);`   

- 성공 시 소켓 핸들, 실패 시 SOCKET_ERROR 반환   


#### 연결 요청 가능 상태로 변경   

`int listen(SOCKET s, int backlog);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   


#### 연결 요청에 대한 수락   

`SOCKET accept(SOCKET s, struct sockaddr * addr, int * addrlen);`   

- 성공 시 소켓 핸들, 실패 시 INVALID_SOCKET 반환   











