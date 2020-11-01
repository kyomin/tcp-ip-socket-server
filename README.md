##  개요   

C를 이용한 Tcp / Ip Socket 프로그래밍 연습입니다.
윈도우 운영체제 환경에서 작업하였습니다.  


#### 소스를 돌리기 위해 Visual Studio 상에서 다음 작업을 합니다.


1. 프로젝트 -> 속성 -> 구성 속성 -> 링커 -> 입력 -> 추가 종속성 -> ws2_32.lib 추가   


2. 프로젝트 -> 속성 -> 구성 속성 -> C/C++ -> 일반 -> SDL 검사 -> 아니오 체크   


3. Debug -> Release 모드로 변경(실행파일을 만들기 위함)   


4. cmd에서 Release 폴더로 이동한 후에 `실행파일명 포트번호` 입력      
   
   
   #
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
   
   
   #
## 서버 소켓의 개요   

- 1단계. 소켓 생성 `socket` 함수 호출   
- 2단계. IP주소와 PORT번호 할당 `bind` 함수 호출   
- 3단계. 연결요청 가능상태로 변경 `listen` 함수 호출   
- 4단계. 연결요청에 대한 수락 `accept` 함수 호출      
   
   
   #
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
   
   
   #
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

<pre>
<code>
int main(int argc, char* argv[]) {   

  WSADATA wsaData;

  ...
   
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");
  
  ...
   
  return 0;   
}   
</code>
</pre>
   
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
   
   
   #
## 윈도우 기반의 클라이언트 소켓 관련 함수   

#### 서버로의 연결 요청   

`int connect(SOCKET s, const struct sockaddr * name, int namelen);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   

#### 소켓 연결 닫기   

리눅스에서는 파일을 닫을 때에도, 소켓을 닫을 때에도 `close` 함수를 호출하지만,   
윈도우에서는 소켓을 닫을 때 호출하는 다음 함수가 별도로 마련되어 있다.   

`int closesocket(SOCKET s);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   
   
   
   #
## 윈도우 기반 입출력 함수   

리눅스는 소켓도 파일로 간주하기 때문에, 파일 입출력 함수인 `read`와 `write`를 이용해서 데이터를 송수신할 수 있다.   
그러나 윈도우는 상황이 좀 다르다.   
파일 입출력 함수와 소켓 입출력 함수가 엄연히 구분되기 때문이다.   
다음은 윈도우 소켓 기반의 데이터 입출력 함수이다.   

#### 데이터 보내기   

`int send(SOCKET s, const char * buf, int len, int flags);`   

- s : 데이터 전송 대상과의 연결을 의미하는 소켓의 핸들 값 전달.   
- buf : 전송할 데이터를 저장하고 있는 버퍼의 주소 값 전달.   
- len : 전송할 바이트 수 전달.   
- flags : 데이터 전송 시 적용할 다양한 옵션 정보 전달.   
- 성공 시 전송된 바이트 수, 실패 시 SOCKET_ERROR 반환   

#### 데이터 읽기   

`int recv(SOCKET s, const char * buf, int len, int flags);`   

- s : 데이터 수신 대상과의 연결을 의미하는 소켓의 핸들 값 전달.   
- buf : 수신된 데이터를 저장할 버퍼의 주소 값 전달.   
- len : 수신할 수 있는 최대 바이트 수 전달.   
- flags : 데이터 수신 시 적용할 다양한 옵션 정보 전달.   
- 성공 시 수신한 바이트 수(단 EOF 전송 시 0), 실패 시 SOCKET_ERROR 반환   
   
   
   #
## 소켓의 프로토콜과 그에 따른 데이터 전송 특성   

#### 프로토콜(Protocol)이란 무엇인가?   

프로토콜이란 대화에 필요한 통신규약을 의미한다.   
그리고 이러한 프로토콜의 개념은 컴퓨터의 관점에서 다음과 같이 정리할 수 있다.   

`컴퓨터 상호간의 대화에 필요한 통신규약`   

쉽게 말해서 프로토콜은 약속이다.   
서로 데이터를 주고받기 위해서 정의해 놓은 약속을 뜻한다.   

#### 소켓의 생성   

`int socket(int domain, int type, int protocol);`   

- domain : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달.   
- type : 소켓의 데이터 전송방식에 대한 정보 전달.   
- protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달.   

#### 프로토콜 체계(Protocol Family)   

- PF_INET : IPv4 인터넷 프로토콜 체계   
- PF_INET6 : IPv6 인터넷 프로토콜 체계   
- PF_LOCAL : 로컬 통신을 위한 UNIX 프로토콜 체계   
- PF_PACKET : Low Level 소켓을 위한 프로토콜 체계   
- PF_IPX : IPX 노벨 프로토콜 체계   

현재 아직까지 대부분 PF_INET 프로토콜 체계를 이용해 소켓 프로그래밍을 한다.   

#### 소켓의 타입(Type)   

프로토콜 체계(Protocol Family)가 결정되었다고 해서 데이터의 전송방식까지 완전히 결정되는 것은 아니다.   
즉, socket 함수의 첫 번째 인자로 전달되는 `PF_INET`에 해당하는 프로토콜 체계에도 둘 이상의 데이터 전송방식이 존재한다.   
다음은 IPv4 인터넷 프로토콜 체계에서 사용되는 대표적인 데이터 전송방식 두 가지이다.   

#### 연결지향형 소켓(SOCK_STREAM) - TCP   

연결지향형 프로토콜인 `TCP`는 다음의 특성을 지닌다.   

- 중간에 데이터가 소멸되지 않고 목적지로 전송된다.
	- 자신과 연결된 상대 소켓의 상태를 파악해가면서 데이터를 전송한다.   
	혹 데이터가 제대로 전송되지 않으면 데이터를 재전송하기까지 한다.   
- 전송 순서대로 데이터가 수신된다.   
- 전송되는 데이터의 경계(Boundary)가 존재하지 않는다.
	- 데이터를 송수신하는 소켓은 내부적으로 버퍼(buffer), 쉽게 말해서 바이트 배열을 지니고 있다.   
	그리고 소켓을 통해 전송되는 데이터는 일단 이 배열에 저장된다.   
	때문에 데이터가 수신되었다고 해서 바로 read 함수를 호출해야 하는 것은 아니다.   
	이 배열의 용량을 초과하지 않는 한, 데이터가 채워진 후에 한 번의 read 함수 호출을 통해서 데이터 전부를 읽어 들일 수도 있고,   
	반대로 한 번의 write 함수 호출로 전송된 데이터 전부를 여러 번의 read 함수 호출을 통해서 읽어 들일 수도 있다.   
	즉, read 함수의 호출 횟수와 write 함수의 호출 횟수는 연결지향형 소켓의 경우 큰 의미를 갖지 못한다.   
	때문에, 연결지향형 소켓은 데이터의 경계가 존재하지 않는다고 말하는 것이다.   

#### 비 연결지향형 소켓(SOCK_DGRAM) - UDP   

비 연결지향형 프로토콜인 `UDP`는 다음의 특성을 지닌다.   

- 전송된 순서에 상관없이 가장 빠른 전송을 지향한다.   

- 전송된 데이터는 손실의 우려가 있고, 파손의 우려가 있다.   

- 전송되는 데이터의 경계(Boundary)가 존재한다.   

- 한 번에 전송할 수 있는 데이터의 크기가 제한된다.   
   
   
   #
## 소켓에 할당되는 IP 주소와 PORT 번호   

#### IP와 PORT   

IP는 Internet Protocol의 약자로 인터넷상에서 데이터를 송수신할 목적으로 컴퓨터에게 부여하는 값을 의미한다.   
반면 PORT 번호는 컴퓨터에게 부여하는 값이 아닌, 프로그램상에서 생성되는 소켓을 구분하기 위해 소켓에 부여되는 번호를 뜻한다.   

#### 인터넷 주소(Internet Address)   

인터넷에 컴퓨터를 연결해서 데이터를 주고받기 위해서는 IP 주소를 부여 받아야 한다.   
이러한 IP 주소체계는 다음과 같이 두 종류로 나뉜다.   

- IPv4(Internet Protocol version 4) : 4바이트 주소체계   
- IPv6(Internet Protocol version 6) : 16바이트 주소체계   

IPv4와 IPv6의 차이점은 IP 주소의 표현에 사용되는 바이트 크기에 있다.   
오늘날 우리가 범용적으로 사용하는 주소체계는 IPv4이다.   
IPv6는 IPv4 체계의 주소가 모두 고갈될 일을 염려하여 만들어진 표준인데, 아직은 IPv4가 주로 사용되고 있다.   
   
IPv4 기준의 4바이트 IP 주소는 네트워크 주소와 호스트(컴퓨터를 의미함) 주소로 나뉘며,   
주소의 형태에 따라서 A, B, C, D, E 클래스로 분류가 된다.   
다음 그림은 IPv4의 주소체계를 보여준다.   
   
![네트워크 클래스](https://user-images.githubusercontent.com/46395776/97806351-98dd9100-1c9e-11eb-917e-12358b5ed7b0.png)   
   
네트워크 주소(네트워크 ID)란 네트워크의 구분을 위한 IP 주소의 일부를 말한다.   
해당 네트워크 주소로 우선 데이터를 전송한 후,   
호스트 ID를 참조하여 해당 네트워크 내의 호스트(PC)로 데이터를 최종 전달하는 것이다.   

#### 소켓의 구분에 활용되는 PORT 번호   

IP는 컴퓨터를 구분하기 위한 목적으로 존재한다.   
때문에, IP만 있다면 목적지 컴퓨터로 데이터를 전송할 수는 있다.   
그러나 이것만 가지고 데이터를 수신해야 하는 최종 목적지인 응용프로그램(애플리케이션)까지 데이터를 전송할 순 없다.   
   
대부분의 컴퓨터에는 `NIC(네트워크 인터페이스 카드)`라 불리는 데이터 송수신장치가 하나씩 달려있다.   
IP는 데이터를 NIC를 통해 컴퓨터 내부로 전송하는데 사용된다.   
그러나 컴퓨터 내부로 전송된 데이터를 소켓에 적절히 분배하는 작업은 운영체제가 담당한다.   
이 때, 운영체제는 PORT 번호를 활용한다.   
즉, NIC를 통해서 수신된 데이터 안에는 PORT 번호가 새겨져 있다.   
운영체제는 바로 이 정보를 참조해서 일치하는 PORT 번호의 소켓에 데이터를 전달하는 것이다.   
   
이렇듯 PORT 번호는 하나의 운영체제 내에서 소켓을 구분하는 목적으로 사용되기 때문에, 하나의 운영체제 내에서 동일한 PORT 번호를 둘 이상의 소켓에 할당할 수 없다.   
그리고 PORT 번호는 16비트로 표현된다.   
때문에, 할당할 수 있는 PORT의 범위는 0 ~ 65535이다.   
그러나 0부터 1023번까지는 `잘 알려진 PORT(Well-known PORT)`라 해서, 특정 프로그램에 할당하기로 예약되어있기 때문에,   
이 범위의 값을 제외한 다른 값을 할당해야 한다.   
   
   
   #
## 네트워크 바이트 순서와 인터넷 주소 변환   

#### 바이트 순서(Order)와 네트워크 바이트 순서   

CPU에 따라서 4바이트 정수 1을 메모리공간에 저장하는 방식이 달라질 수 있다.   
4바이트 정수 1을 2진수로 표현하면 다음과 같다.   
   
`00000000 00000000 00000000 00000001`   
   
이 순서 그대로 메모리에 저장하는 CPU가 있는가 하면, 다음과 같이 거꾸로 저장하는 CPU도 있다.   
   
`00000001 00000000 00000000 00000000`   
   
때문에, 이러한 부분을 고려하지 않고서 데이터를 송수신하면 문제가 발생할 수 있다.   
저장순서가 다르다는 것은 전송되어 온 데이터의 해석 순서가 다름을 뜻하기 때문이다.   
   
CPU가 데이터를 메모리에 저장하는 방식은 다음과 같이 두 가지로 나뉜다.   
참고로 CPU가 데이터를 메모리에 저장하는 방식이 두 가지로 나뉜다는 것은 CPU가 데이터를 해석하는 방식도 두 가지로 나뉜다는 뜻이다.   
   
- 빅 엔디안(Big Endian) : 상위 바이트의 값을 작은 번지수에 저장하는 방식   
- 리틀 엔디안(Little Endian) : 상위 바이트의 값을 큰 번지수에 저장하는 방식   

예를 들어, 0x1000번지를 시작으로 4바이트 int형 정수 0x12345678을 저장한다고 가정해 보자.   
그러면 다음 그림과 같이 저장된다.   
   
![엔디안](https://user-images.githubusercontent.com/46395776/97808170-b1eb3f80-1ca8-11eb-9b18-c94df0226025.jpeg)   
   
이렇게 CPU마다 차이나는 바이트 순서는 호스트끼리 데이터 전송 시에 문제가 된다.   
바로 이러한 문제점 때문에 네트워크를 통해서 데이터를 전송할 때에는 통일된 기준으로 데이터를 전송하기로 약속했으며,   
이 약석을 가리켜 `네트워크 바이트 순서(Network Byte Order)`라 한다.   
네트워크 바이트 순서의 약속은 매우 간단하다.   
   
`빅 엔디안 방식으로 통일하자!`   
   
즉, 네트워크상으로 데이터를 전송할 때에는 데이터의 배열을 빅 엔디안 기준으로 변경해서 송수신하기로 약속한 것이다.   
때문에, 모든 컴퓨터는 수신된 데이터가 네트워크 바이트 순서로 정렬되어 있음을 인식해야 하며,   
리틀 엔디안 시스템에서는 데이터를 전송하기에 앞서 빅 엔디안의 정렬방식으로 데이터를 재정렬해야 한다.   

#### INADDR_ANY   

서버 소켓의 생성과정에서 매번 서버의 IP 주소를 입력하는 것은 귀찮은 일이 될 수 있다.   
그렇다면 다음과 같이 주소 정보를 초기화해도 된다.   
   
<pre>
<code>

struct sockaddr_in addr;
char *serv_port = "9190";
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(serv_port));

</code>
</pre>   
   
위의 코드에서 INADDR_ANY라는 이름의 상수를 통해서 서버의 IP 주소를 할당하고 있다.   
소켓의 IP 주소를 이렇게 초기화할 경우, 소켓이 동작하는 컴퓨터의 IP 주소가 자동으로 할당되기 때문에 IP 주소를 직접 입력하는 수고를 덜 수 있다.   
뿐만 아니라, 컴퓨터 내에 두 개 이상의 IP를 할당 받아서 사용하는 경우(이를 가리켜 Multi-homed 컴퓨터라 하며, 일반적으로 라우터가 이에 해당한다),   
할당 받은 IP중 어떤 주소를 통해서 데이터가 들어오더라도 PORT 번호만 일치하면 수신할 수 있게 된다.   
따라서, 서버 프로그램의 구현에 많이 선호되는 방법이다.   

#### 서버 소켓 생성시 IP 주소가 필요한 이유   

서버 소켓은 생성시 자신이 속한 컴퓨터의 IP 주소로 초기화가 이뤄져야 한다.   
즉, 초기화할 IP 주소가 뻔하다.   
그럼에도 IP 주소의 초기화를 요구하는 것에 의문을 가질 수 있다.   
하지만 바로 위에서 언급한, 하나의 컴퓨터에 둘 이상의 IP 주소가 할당될 수 있다는 사실을 통해서 이 부분을 이해할 수 있다.   
   
IP 주소는 컴퓨터에 장착되어 있는 NIC(랜카드)의 개수만큼 부여가 가능하다.   
그리고 이러한 경우에는 서버 소켓이라 할지라도 어느 IP 주소로 들어오는(어느 NIC로 들어오는) 데이터를 수신할지 결정해야 한다.   
때문에, 서버 소켓의 초기화 과정에서 IP 주소 정보를 요구하는 것이다.   
반면, NIC가 하나뿐인 컴퓨터라면 주저 없이 INADDR_ANY를 이용해서 초기화하는 것이 편리하다.   
   
   
   #
## TCP와 UDP에 대한 이해   

#### TCP / IP 프로토콜 스택   

다음은 `TCP / IP 프로토콜 스택(Stack, 계층)을 보여준다.   
   
![TCP IP 프로토콜 스택](https://user-images.githubusercontent.com/46395776/97809140-8bc89e00-1cae-11eb-8260-3dddca437b3c.png)   
   
위 그림을 통해서 TCP / IP 스택이 총 네 개의 계층으로 나뉨을 알 수 있는데,   
이는 데이터 송수신의 과정을 네 개의 영역으로 계층화했다는 의미로 받아들일 수 있다.   
즉 `인터넷 기반의 효율적인 데이터 전송`이라는 커다란 하나의 문제를 하나의 덩치 큰 프로토콜 설계로 해결한 것이 아니라,   
그 문제를 작게 나눠서 계층화하려는 노력이 시도되었고,   
그 결과로 탄생한 것이 `TCP / IP 프로토콜 스택`인 것이다.   
   
참고로 좀 더 작게는 OSI 7 계층으로 세분화된다.   
하지만 위의 그림과 같이 4계층으로 구분 짓기도 한다.   
프로그래머의 관점에서는 4계층으로 이해하고 있어도 충분하다.   
   
따라서 우리가 TCP 소켓을 생성해서 데이터를 송수신할 경우에는 다음 네 계층의 도움을 받아 데이터를 송수신하게 된다.   
   
`Application Layer <=> TCP Layer <=> IP Layer <=> Link Layer`   
   
반면에 UDP 소켓을 생성해서 데이터를 송수신할 경우에는 다음 네 계층의 도움을 통해서 데이터를 송수신하게 된다.   
   
`Application Layer <=> UDP Layer <=> IP Layer <=> Link Layer`   
   
그리고 각각의 계층을 담당하는 것은 운영체제와 같은 소프트웨어이기도 하고,   
NIC와 같은 물리적인 장치이기도 하다.