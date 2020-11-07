#  개요   

C를 이용한 Tcp / Ip Socket 프로그래밍 연습입니다.
윈도우 운영체제 환경에서 작업하였습니다.  


### 소스를 돌리기 위해 Visual Studio 상에서 다음 작업을 합니다.


1. 프로젝트 -> 속성 -> 구성 속성 -> 링커 -> 입력 -> 추가 종속성 -> ws2_32.lib 추가   


2. 프로젝트 -> 속성 -> 구성 속성 -> C/C++ -> 일반 -> SDL 검사 -> 아니오 체크   


3. Debug -> Release 모드로 변경(실행파일을 만들기 위함)   


4. cmd에서 Release 폴더로 이동한 후에 `실행파일명 포트번호` 입력      
   
   
   #
# 네트워크 프로그래밍과 소켓   

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
# 서버 소켓의 개요   

- 1단계. 소켓 생성 `socket` 함수 호출   
- 2단계. IP주소와 PORT번호 할당 `bind` 함수 호출   
- 3단계. 연결요청 가능상태로 변경 `listen` 함수 호출   
- 4단계. 연결요청에 대한 수락 `accept` 함수 호출      
   
   
   #
# 리눅스 기반 파일 조작하기   

리눅스는 소켓을 파일의 일종으로 구분한다.   
따라서 파일 입출력 함수를 소켓 입출력에, 다시 말해서 네트워크상에서의 데이터 송수신에 사용할 수 있다.      


### 파일 디스크립터   

파일 디스크립터란 시스템으로부터 할당 받은 파일 또는 소켓에 부여된 정수를 의미한다.      


### 파일 열기   

`int open(char *path, int flag);`   

- path : 파일 이름을 나타내는 문자열의 주소 값 전달.   
- flag : 파일의 오픈 모드 정보 전달.   
- 성공 시 파일 디스크립터, 실패 시 -1 반환      


### 파일 닫기   

`int close(int fd);`   

- fd : 닫고자 하는 파일 또는 소켓의 파일 디스크립터 전달.   
- 성공 시 0, 실패 시 -1 반환      


### 파일에 데이터 쓰기   

`ssize_t write(int fd, const void * buf, size_t nbytes);`   

- fd : 데이터 전송 대상을 나타내는 파일 디스크립터 전달.   
- buf : 전송할 데이터가 저장된 버퍼의 주소 값 전달.   
- nbytes : 전송할 데이터의 바이트 수 전달.   
- 성공 시 전달한 바이트 수, 실패 시 -1 반환      


### 파일에 저장된 데이터 읽기   

`ssize_t read(int fd, void *buf, size_t nbytes);`   

- fd : 데이터 수신 대상을 나타내는 파일 디스크립터 전달.   
- buf : 수신한 데이터를 저장할 버퍼의 주소 값 전달.   
- nbytes : 수신할 최대 바이트 수 전달.   
- 성공 시 수신한 바이트 수(단, 파일의 끝을 만나면 0), 실패 시 -1 반환      
   
   
   #
# 윈도우 기반의 서버 소켓 관련 함수   


### 윈속(winsock)의 초기화   

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


### 소켓 생성   

`SOCKET socket(int af, int type, int protocol);`   

- 성공 시 소켓 핸들, 실패 시 INVALID_SOCKET 반환   


### IP주소와 PORT번호 할당   

`int bind(SOCKET s, const struct sockaddr * name, int namelen);`   

- 성공 시 소켓 핸들, 실패 시 SOCKET_ERROR 반환   


### 연결 요청 가능 상태로 변경   

`int listen(SOCKET s, int backlog);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   


### 연결 요청에 대한 수락   

`SOCKET accept(SOCKET s, struct sockaddr * addr, int * addrlen);`   

- 성공 시 소켓 핸들, 실패 시 INVALID_SOCKET 반환   
   
   
   #
# 윈도우 기반의 클라이언트 소켓 관련 함수   

### 서버로의 연결 요청   

`int connect(SOCKET s, const struct sockaddr * name, int namelen);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   

### 소켓 연결 닫기   

리눅스에서는 파일을 닫을 때에도, 소켓을 닫을 때에도 `close` 함수를 호출하지만,   
윈도우에서는 소켓을 닫을 때 호출하는 다음 함수가 별도로 마련되어 있다.   

`int closesocket(SOCKET s);`   

- 성공 시 0, 실패 시 SOCKET_ERROR 반환   
   
   
   #
# 윈도우 기반 입출력 함수   

리눅스는 소켓도 파일로 간주하기 때문에, 파일 입출력 함수인 `read`와 `write`를 이용해서 데이터를 송수신할 수 있다.   
그러나 윈도우는 상황이 좀 다르다.   
파일 입출력 함수와 소켓 입출력 함수가 엄연히 구분되기 때문이다.   
다음은 윈도우 소켓 기반의 데이터 입출력 함수이다.   

### 데이터 보내기   

`int send(SOCKET s, const char * buf, int len, int flags);`   

- s : 데이터 전송 대상과의 연결을 의미하는 소켓의 핸들 값 전달.   
- buf : 전송할 데이터를 저장하고 있는 버퍼의 주소 값 전달.   
- len : 전송할 바이트 수 전달.   
- flags : 데이터 전송 시 적용할 다양한 옵션 정보 전달.   
- 성공 시 전송된 바이트 수, 실패 시 SOCKET_ERROR 반환   

### 데이터 읽기   

`int recv(SOCKET s, const char * buf, int len, int flags);`   

- s : 데이터 수신 대상과의 연결을 의미하는 소켓의 핸들 값 전달.   
- buf : 수신된 데이터를 저장할 버퍼의 주소 값 전달.   
- len : 수신할 수 있는 최대 바이트 수 전달.   
- flags : 데이터 수신 시 적용할 다양한 옵션 정보 전달.   
- 성공 시 수신한 바이트 수(단 EOF 전송 시 0), 실패 시 SOCKET_ERROR 반환   
   
   
   #
# 소켓의 프로토콜과 그에 따른 데이터 전송 특성   

### 프로토콜(Protocol)이란 무엇인가?   

프로토콜이란 대화에 필요한 통신규약을 의미한다.   
그리고 이러한 프로토콜의 개념은 컴퓨터의 관점에서 다음과 같이 정리할 수 있다.   

`컴퓨터 상호간의 대화에 필요한 통신규약`   

쉽게 말해서 프로토콜은 약속이다.   
서로 데이터를 주고받기 위해서 정의해 놓은 약속을 뜻한다.   

### 소켓의 생성   

`int socket(int domain, int type, int protocol);`   

- domain : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달.   
- type : 소켓의 데이터 전송방식에 대한 정보 전달.   
- protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달.   

### 프로토콜 체계(Protocol Family)   

- PF_INET : IPv4 인터넷 프로토콜 체계   
- PF_INET6 : IPv6 인터넷 프로토콜 체계   
- PF_LOCAL : 로컬 통신을 위한 UNIX 프로토콜 체계   
- PF_PACKET : Low Level 소켓을 위한 프로토콜 체계   
- PF_IPX : IPX 노벨 프로토콜 체계   

현재 아직까지 대부분 PF_INET 프로토콜 체계를 이용해 소켓 프로그래밍을 한다.   

### 소켓의 타입(Type)   

프로토콜 체계(Protocol Family)가 결정되었다고 해서 데이터의 전송방식까지 완전히 결정되는 것은 아니다.   
즉, socket 함수의 첫 번째 인자로 전달되는 `PF_INET`에 해당하는 프로토콜 체계에도 둘 이상의 데이터 전송방식이 존재한다.   
다음은 IPv4 인터넷 프로토콜 체계에서 사용되는 대표적인 데이터 전송방식 두 가지이다.   

### 연결지향형 소켓(SOCK_STREAM) - TCP   

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

### 비 연결지향형 소켓(SOCK_DGRAM) - UDP   

비 연결지향형 프로토콜인 `UDP`는 다음의 특성을 지닌다.   

- 전송된 순서에 상관없이 가장 빠른 전송을 지향한다.   

- 전송된 데이터는 손실의 우려가 있고, 파손의 우려가 있다.   

- 전송되는 데이터의 경계(Boundary)가 존재한다.   

- 한 번에 전송할 수 있는 데이터의 크기가 제한된다.   
   
   
   #
# 소켓에 할당되는 IP 주소와 PORT 번호   

### IP와 PORT   

IP는 Internet Protocol의 약자로 인터넷상에서 데이터를 송수신할 목적으로 컴퓨터에게 부여하는 값을 의미한다.   
반면 PORT 번호는 컴퓨터에게 부여하는 값이 아닌, 프로그램상에서 생성되는 소켓을 구분하기 위해 소켓에 부여되는 번호를 뜻한다.   

### 인터넷 주소(Internet Address)   

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

### 소켓의 구분에 활용되는 PORT 번호   

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
# 네트워크 바이트 순서와 인터넷 주소 변환   

### 바이트 순서(Order)와 네트워크 바이트 순서   

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

### INADDR_ANY   

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

### 서버 소켓 생성시 IP 주소가 필요한 이유   

서버 소켓은 생성시 자신이 속한 컴퓨터의 IP 주소로 초기화가 이뤄져야 한다.   
즉, 초기화할 IP 주소가 뻔하다.   
그럼에도 IP 주소의 초기화를 요구하는 것에 의문을 가질 수 있다.   
하지만 바로 위에서 언급한, 하나의 컴퓨터에 둘 이상의 IP 주소가 할당될 수 있다는 사실을 통해서 이 부분을 이해할 수 있다.   
   
IP 주소는 컴퓨터에 장착되어 있는 NIC(랜카드)의 개수만큼 부여가 가능하다.   
그리고 이러한 경우에는 서버 소켓이라 할지라도 어느 IP 주소로 들어오는(어느 NIC로 들어오는) 데이터를 수신할지 결정해야 한다.   
때문에, 서버 소켓의 초기화 과정에서 IP 주소 정보를 요구하는 것이다.   
반면, NIC가 하나뿐인 컴퓨터라면 주저 없이 INADDR_ANY를 이용해서 초기화하는 것이 편리하다.   
   
   
   #
# TCP와 UDP에 대한 이해   

### TCP / IP 프로토콜 스택   

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

### TCP / IP 프로토콜의 탄생 배경   

`인터넷을 통한 효율적인 데이터의 송수신`이라는 과제의 해결을 위해서 많은 전문가들이 모여있다.   
이들은 하드웨어 전문가부터 시작해서 시스템 전문가, 라우팅 알고리즘 전문가 등 아주 다양한 분야의 최고 전문가들이다.   
그렇다면 왜 이렇게 많은 분야의 전문가들이 필요한 것일까?   
   
지금까지 우리는 소켓의 생성 및 활용에만 관심을 둬서 생각하지 못했지만, 네트워크는 소프트웨어만 가지고 해결할 수 있는 문제가 아니다.   
소프트웨어가 존재하기 전에 하드웨어적으로 시스템이 구축되어 있어야 하고,   
그러한 물리적 환경을 기반으로 각종 소프트웨어적인 알고리즘을 필요로 한다.   
   
즉, `인터넷을 통한 효율적인 데이터의 송수신`이라는 이슈의 해결을 위해서는 아주 많은 분야의 전문가가 필요하며,   
이들간의 상호 논의로 만들어진 다양한 약속이 또한 필요하다.   
따라서 이 문제는 작은 문제로 나눠서 해결하는 것이 효율적이다.   
   
결국엔 문제를 영역별로 나눠서 `인터넷을 통한 효율적인 데이터의 송수신`에 대한 결론을 얻게 되었다.   
문제를 영역별로 나눠서 해결하다 보니 프로토콜이 여러 개 만들어졌으며,   
이들은 계층구조를 통해서 상호간에 관계를 맺게 되었다.   

### 개방형 시스템(Open System)   

구체적으로 프로토콜을 계층화해서 얻게 되는 장점에는 어떤 것들이 있을까?   
프로토콜 설계의 용이성? 물론 이것도 장점이라 말하기엔 충분하다.   
그러나 이보다 더 중요한 이유가 있으니, 이는 바로 표준화 작업을 통한 `개방형 시스템(Open System)`의 설계이다.   
   
표준이라는 것은 감추는 것이 아니라 활짝 열고 널리 알려서 많은 사람이 따르도록 유도하는 것이다.   
따라서 여러 개의 표준을 근거로 설계된 시스템을 가리켜 `개방형 시스템`이라 하며, 현재 다루고 있는 `TCP / IP 프로토콜 스택` 역시 개방형 시스템의 하나이다.   
   
그렇다면 개방형 시스템의 장점이 무엇인지 살펴보자.   
IP 계층을 담당하는 라우터라는 장비가 있다.   
회사에서 A사의 라우터 장비를 사용하고 있었는데, 이를 B사의 라우터 장비로 교체하려고 한다.   
교체가 가능할까? 물론 어렵지 않게 교체가 가능하다.   
반드시 같은 회사의 같은 모델로 교체해야 하는 것은 아니다.   
왜냐하면 모든 라우터 제조사들이 `IP 계층`의 표준에 맞춰서 라우터를 제작하기 때문이다.   
   
한 가지 예시를 더 살펴보자.   
현재 우리의 컴퓨터에 네트워크 카드, 소위 말하는 랜카드가 달려있는지 확인하자.   
만일 달려있지 않다면 우리는 어렵지 않게 랜카드를 선택할 수 있다.   
모든 랜카드 제조사가 `LINK 계층`의 표준을 따르기 때문이다.   
이것이 바로 개방형 시스템의 장점이다.   
   
이렇듯 표준이 존재한다는 것은 그만큼 빠른 속도의 기술발전이 가능하다는 것을 의미한다.   
그리고 이것이 시스템을 개방형으로 설계하는 가장 큰 이유이기도 하다.   
사실 소프트웨어 공학에서의 `객체지향(Object Oriented)`의 탄생배경에도 소프트웨어의 발전을 위해서는 표준이 필요하다는 생각이 큰 몫을 차지했다.   
그만큼 표준이라는 것은 기술의 발전에 있어서 중요한 요소이다.   

### LINK 계층   

LINK 계층은 물리적인 영역의 표준화에 대한 결과이다.   
이는 가장 기본이 되는 영역으로 LAN, WAN, MAN과 같은 네트워크 표준과 관련된 프로토콜을 정의하는 영역이다.   
두 호스트가 인터넷을 통해 데이터를 주고받으려면 물리적인 연결이 존재해야 하는 것은 당연하다.   
바로 이 부분에 대한 표준을 LINK 계층에서 담당하고 있다.   

### IP 계층   

이제 LINK 계층에 의한 물리적인 연결이 형성되었으니, 데이터를 보낼 기본 준비가 되었다고 할 수 있다.   
그런데 복잡하게 연결되어 있는 인터넷을 통한 데이터의 전송을 위해 선행되어야 할 일은 경로의 선택이다.   
`목적지로 데이터를 전성하기 위해서 중간에 어떤 경로를 거쳐갈 것인가`에 대한 문제를 해결하는 것이 IP 계층이고, 이 계층에서 사용하는 프로토콜이 `IP(Internet Protocol)`이다.   
   
![네트워크 라우팅](https://user-images.githubusercontent.com/46395776/98361903-b3818280-206f-11eb-959c-305fc5b57e66.png)   
   
IP 자체는 `비 연결지향적`이며, `신뢰할 수 없는` 프로토콜이다.   
데이터를 전송할 때마다 거쳐야 할 경로를 선택해 주지만, 그 경로는 일정치 않다.   
특히 데이터 전송 도중에 경로상에 문제가 발생하면 다른 경로를 선택해 주는데, 이 과정에서 데이터가 손실되거나 오류가 발생하는 등의 문제가 발생한다고 해서 이를 해결해주지 않는다.   
즉, 오류발생에 대한 대비가 되어있지 않은 프로토콜이 IP이다.   

### TCP / UDP 게층   

데이터의 전송을 위한 경로의 검색을 IP계층에서 해결해주니, 그 경로를 기준으로 데이터를 전송만하면 된다.   
TCP와 UDP 계층은 이렇듯 IP 계층에서 알려준 경로 정보를 바탕으로 데이터의 실제 송수신을 담당한다.   
때문에, 이 계층을 가리켜 `전송(Transport) 계층`이라 한다.   
   
전송 계층에 존재하는 UDP는 TCP에 비해 상대적으로 간단하며, 이후에 별도로 언급하니 일단은 TCP에 대해서만 추가로 설명한다.   
TCP는 `신뢰성 있는` 데이터의 전송을 담당한다.   
그런데 TCP가 데이터를 보낼 때 기반이 되는 프로토콜이 IP이다.   
이것이 프로토콜이 스택의 구조로 계층화되어 있는 이유이다.   
그러면 이 둘의 관계를 어떻게 이해하면 좋을까?   
   
IP는 오로지 하나의 데이터 패킷(데이터 전송의 기본단위)이 전송되는 과정에만 중심을 두고 설계되었다.   
따라서 여러 개의 데이터 패킷을 전송한다 하더라도 각각의 패킷이 전송되는 과정은 IP에 의해서 진행되므로 전송의 순서는 물론이거니와 전송 그 자체를 신뢰할 수 없다.   
만약에 IP만을 이용해 데이터를 전송한다면 먼저 전송한 A 패킷보다 뒤에 전송한 B 패킷이 먼저 도달할 수도 있다.   
그리고 이어서 전송한 A, B, C 패킷 중에서 A와 C 패킷만 전송될 수 있으며, 그나마 C 패킷은 손상된 상태로 전송될 수도 있다.   
반면에 TCP 프로토콜이 추가되어 데이터를 송수신하면 다음과 같은 대화를 주고받게 된다.   
   
- 호스트 A : 두 번째 패킷까지 잘 받았소!
- 호스트 B : 네 알겠습니다.   
   
- 호스트 A : 세 번째 패킷까지는 잘 받았소!
- 호스트 B : 네 번째 패킷까지 보냈는데요? 네 번째 패킷은 못 받았나 보군요! 그러면 재전송 하겠습니다.   
   
이것이 바로 TCP의 역할이다.   
이렇듯 데이터를 주고받는 과정에서 서로 데이터의 주고 받음을 확인한다면, 그리고 분실된 데이터에 대해서 재전송해준다면, 데이터의 전송을 신뢰할 수 있다.   
비록 IP가 데이터의 전송을 보장하지 않더라도 말이다.   
   
결론적으로 IP의 상위 계층에서 호스트 대 호스트의 데이터 송수신 방식을 약속하는 것이 TCP 그리고 UDP이며,   
TCP는 확인절차를 걸쳐서 신뢰성 없는 IP에 신뢰성을 부여한 프로토콜이라 할 수 있다.   

### APPLICATION 계층   

지금까지 설명한 각 계층의 역할들은 소켓을 생성하면 데이터 송수신과정에서 자동으로 처리되는 것들이다.   
데이터의 전송 경로를 확인하는 과정이라든가 데이터 수신에 대한 응답의 과정이 소켓이라는 것 하나에 감춰져 있기 때문이다.   
그러나 감춰져 있다는 표현보다는 이러한 일들에 대해서 프로그래머를 자유롭게 해줬다는 표현이 더 정확하다.   
즉, 프로그래밍에 있어서 이러한 과정을 일일이 신경 쓰지 않아도 된다는 뜻이다.   
하지만 신경을 쓰지 않아도 될 뿐이지, 몰라도 된다는 뜻은 아니다.   
이러한 이론적인 내용들도 알고 있어야 필요에 맞는 네트워크 프로그램을 작성할 수 있다.   
   
최종적으로 소켓이라는 도구가 우리에게 주어졌고, 우리는 이 도구를 이용해서 무엇인가를 만들면 된다.   
이렇게 무엇인가를 만드는 과정에서 프로그램의 성격에 따라 클라이언트와 서버간의 데이터 송수신에 대한 약속(규칙)들이 정해지기 마련인데, 이를 가리켜 `APPLICATION 프로토콜`이라 한다.   
그리고 대부분의 네트워크 프로그래밍은 APPLICATION 프로토콜의 설계 및 구현이 상당 부분을 차지한다.   
   
   
   #
# TCP 기반 서버, 클라이언트 구현   

### TCP 서버에서의 기본적인 함수 호출 순서   

아래 그림은 TCP 서버 구현을 위한 기본적인 함수의 호출 순서를 보이고 있다.   
대부분의 TCP 서버 프로그램은 이 순서로 구현이 된다.   
   
![tcp 서버의 함수호출 순서](https://user-images.githubusercontent.com/46395776/98365305-8cc64a80-2075-11eb-8def-cda80297fe93.png)   
   
제일 먼저 `socket 함수`의 호출을 통해서 소켓을 생성한다.   
그리고 주소 정보를 담기 위한 구조체 변수를 선언 및 초기화해서 `bind 함수`를 호출하여 소켓에 주소를 할당한다.   
이 두 단계는 이미 위에서 설명한 내용이니, 그 이후의 과정에 대해 다룬다.   

### 연결요청 대기상태로의 진입   

bind 함수 호출을 통해서 소켓에 주소까지 할당했다면, 이번에는 listen 함수 호출을 통해서 `연결요청 대기상태`로 들어갈 차례이다.   
그리고 listen 함수가 호출되어야 클라이언트가 연결요청을 할 수 있는 상태가 된다.   
즉, listen 함수가 호출되어야 클라이언트는 연결요청을 위해서 `connect 함수`를 호출할 수 있다.
만일 listen 함수 호출 이전에 connect 함수를 호출하면 오류가 발생한다.   
   
`int listen(int sock, int backlog);`   

- sock : 연결요청 대기상태에 두고자 하는 소켓의 파일 디스크립터 전달. 이 함수의 인자로 전달된 디스크립터의 소켓이 서버 소켓(리스닝 소켓)이 된다.   
- backlog : 연결요청 대기 큐(Queue)의 크기정보 전달. 5가 전달되면 큐의 크기가 5가 되어 클라이언트의 연결요청을 5개까지 대기시킬 수 있다.   
- 성공 시 파일 디스크립터, 실패 시 -1 반환   
   
여기서 잠시 `연결요청 대기상태`의 의미와 `연결요청 대기 큐`라는 것에 대해서 별도의 설명을 추가하겠다.   
서버가 `연결요청 대기상태`에 있다는 것은 클라이언트가 연결요청을 했을 때, 연결이 수락될 때까지 연결요청 자체를 대기시킬 수 있는 상태에 있다는 것을 의미한다.   
이를 그림을 통해서 설명하겠다.   
   
![연결요청 대기상태](https://user-images.githubusercontent.com/46395776/98369289-11b46280-207c-11eb-96df-8d09b968d130.png)   
   
위 그림을 보면, listen 함수의 첫 번째 인자로 전달된 파일 디스크립터의 소켓이 어떤 용도로 사용되는지 알 수 있다.   
클라이언트의 연결요청도 인터넷을 통해서 흘러 들어오는 일종의 데이터 전송이기 때문에, 이것을 받아들이려면 당연히 소켓이 하나 있어야 한다.   
서버 소켓의 역할이 바로 이것이다.   
즉, 연결 요청을 맞이하는, 일종의 문지기 또는 문의 역할을 한다고 볼 수 있다.   
   
listen 함수가 호출되면, 이렇듯 문지기의 역할을 하는 서버 소켓이 만들어지고, listen 함수의 두 번째 인자로 전달되는 정수의 크기에 해당하는 대기실이 만들어진다.   
이 대기실을 가리켜 `연결요청 대기 큐`라 하며, 서버 소켓과 연결요청 대기 큐가 완전히 준비되어서 클라이언트의 연결요청을 받아들일 수 있는 상태를 가리켜 `연결요청 대기상태`라 한다.   

### 클라이언트의 연결요청 수락   

listen 함수 호출 이후에 클라이언트의 연결요청이 들어왔다면, 들어온 순서대로 연결요청을 수락해야 한다.   
연결요청을 수락한다는 것은 클라이언트와 데이터를 주고받을 수 있는 상태가 됨을 의미한다.   
   
따라서 이러한 상태가 되기 위해 무엇이 필요한지 짐작할 수 있을 것이다. 당연히 소켓이 필요하다!   
전혀 이상할 것이 없다. 데이터를 주고받으려면 소켓이 있어야 한다.   
물론, 위의 내용으로 서버 소켓(리스닝 소켓)을 사용하면 된다고 생각할 수 있다.   
그런데 서버 소켓은 단순히 클라이언트 요청에 대한 문지기 역할을 한다.   
클라이언트와의 데이터 송수신을 위해 이것을 사용하면 문은 누가 지키겠는가?   
때문에, 소켓을 하나 더 만들어야 한다.   
하지만 우리가 소켓을 직접 만들 필요는 없다.   
다음 함수의 호출결과로 소켓이 만들어지고, 이 소켓은 연결요청을 한 클라이언트 소켓과 자동으로 연결되니 말이다.   
   
`int accept(int sock, struct sockaddr* addr, socklen_t* addrlen);`   

- sock : 서버 소켓의 파일 디스크립터 전달.   
- addr : 연결요청 한 클라이언트의 주소정보를 담을 변수의 주소 값 전달. 함수호출이 완료되면 인자로 전달된 주소의 변수에는 클라이언트의 주소 정보가 채워진다.   
- addrlen : 두 번째 매개변수 addr에 전달된 주소의 변수 크기를 바이트 단위로 전달. 단, 크기 정보를 변수에 저장한 다음에 변수의 주소 값을 전달한다. 그리고 함수 호출이 완료되면 크기정보로 채워져 있던 변수에는 클라이언트의 주소 정보 길이가 바이트 단위로 계산되어 채워진다.   
   
accept 함수는 `연결요청 대기 큐`에서 대기중인 클라이언트의 연결요청을 수락하는 기능의 함수이다.   
따라서 accept 함수는 호출 성공 시 내부적으로 `데이터 입출력`에 사용할 소켓을 생성하고, 그 소켓의 파일 디스크립터를 반환한다.   
중요한 점은 소켓이 자동으로 생성되어, 연결요청을 한 클라이언트 소켓에 연결까지 이뤄진다는 점이다.   
아래 그림은 accept 함수 호출 시 일어나는 상황을 보이고 있다.   
   
![연결요청 수락상태](https://user-images.githubusercontent.com/46395776/98370831-8be5e680-207e-11eb-8e76-940171809e1c.jpeg)   
   
위 그림에서는 대기 큐(Queue)에 존재하던 연결요청 하나를 꺼내서 새로운 소켓을 생성한 후에 연결요청을 완료함을 보이고 있다.   
이렇듯 서버에서 별도로 생성한 소켓과 클라이언트 소켓이 직접 연결되었으니, 이제는 데이터를 주고받는 일만 남았다.   

### TCP 클라이언트의 기본적인 함수 호출 순서   

이번에는 클라이언트의 구현 순서에 대해서 이야기하자.   
클라이언트의 구현 과정은 서버에 비해 매우 단순하다.   
`소켓의 생성`, 그리고 `연결의 요청`이 전부이기 때문이다.   
   
![tcp 클라이언트 함수호출 순서](https://user-images.githubusercontent.com/46395776/98371407-7a510e80-207f-11eb-8b7c-9e8ba3055aab.png)   
   
서버의 구현 과정과 비교해서 차이가 있는 부분은 `연결요청`이라는 과정이다.   
이는 클라이언트 소켓을 생성한 후에 서버로 연결을 요청하는 과정이다.   
서버는 listen 함수를 호출한 이후부터 연결요청 대기 큐를 만들어 놓는다.   
따라서 그 이후부터 클라이언트는 연결요청을 할 수 있다.   
그렇다면 클라이언트는 어떻게 연결요청을 할까?   
다음 함수 호출을 통해서 연결요청을 한다.   
   
`int connect(int sock, const struct sockaddr* servaddr, socklen_t addrlen);`   

- sock : 클라이언트 소켓의 파일 디스크립터 전달.   
- servaddr : `연결요청 할 서버의 주소정보`를 담은 변수의 주소 값 전달   
- addrlen : 두 번째 매개변수 servaddr에 전달된 주소의 변수 크기를 바이트 단위로 전달   
   
클라이언트에 의해서 connect 함수가 호출되면 다음 둘 중 한 가지 상황이 되어야 함수가 반환된다(함수 호출이 완료된다).   
   
- 서버에 의해 연결요청이 접수되었다.
- 네트워크 단절 등 오류 상황이 발생해서 연결요청이 중단되었다.   
   
여기서 주의할 사실은 위에서 말하는 `연결요청의 접수`는 서버의 accept 함수 호출을 의미하는 것이 아니라는 점이다.   
이는 클라이언트의 연결요청 정보가 서버의 연결요청 대기 큐에 등록된 상황을 의미하는 것이다.   
때문에 connect 함수가 반환했더라도 당장에 서비스가 이뤄지지 않을 수도 있음을 기억해야 한다.   

### 클라이언트 소켓의 주소 정보는 어디에?   

서버를 구현하면서 반드시 거쳤던 과정 중 하나가 서버 소켓에 IP와 PORT를 할당하는 것이었다.   
그런데 생각해보면 클라이언트 프로그램의 구현 순서에는 소켓의 주소 할당 과정이 없었다.   
그저 소켓을 생성하고 서버로의 연결을 위해서 connect 함수를 호출한 것이 전부였다.   
그렇다면 클라이언트 소켓은 IP와 PORT의 할당이 불필요한 것일까? 물론 아니다!   
네트워크를 통해서 데이터를 송수신하려면 IP와 PORT가 반드시 할당되어야 한다.   
그렇다면 클라이언트 소켓은 언제, 어디서, 어떻게 할당이 가능했던 것일까?   
   
- 언제? : connect 함수가 호출될 때
- 어디서? : 운영체제에서. 보다 정확히 표현하면 `커널`에서
- 어떻게? : IP는 컴퓨터(호스트)에 할당된 IP로, PORT는 임의로 선택해서!   
   
즉, bind 함수를 통해서 소켓에 IP와 PORT를 직접 할당하지 않아도 connect 함수 호출 시 자동으로 소켓에 IP와 PORT가 할당된다.   
따라서 클라이언트 프로그램을 구현할 때에는 bind 함수를 명시적으로 호출할 필요가 없다.   

### TCP 기반 서버, 클라이언트의 함수 호출 관계   

지금까지 TCP 서버, TCP 클라이언트 프로그램의 구현 순서를 설명했는데, 사실 이 둘은 서로 독립된 과정이 아니기 때문에 하나의 과정으로 머리 속에 그릴 수 있어야 한다.   
그래서 이 두 과정을 하나의 그림으로 정리하면 다음과 같다.   
   
![tcp 기반 서버 클라이언트의 함수호출 관계](https://user-images.githubusercontent.com/46395776/98381386-25b49000-208d-11eb-9362-42245e745303.png)   
   
전체적인 흐름을 정리하면, 서버는 소켓 생성 이후에 bind, listen 함수의 연이은 호출을 통해 대기상태에 들어가고, 클라이언트는 connect 함수 호출을 통해서 연결요청을 하게 된다.   
특히 클라이언트는 서버 소켓의 listen 함수 호출 이후에 connect 함수 호출이 가능하다는 사실을 기억할 필요가 있다.   
뿐만 아니라 클라이언트가 connect 함수를 호출하기에 앞서 서버가 accept 함수를 먼저 호출할 수 있다는 사실도 함께 기억해야 한다.   
물론 이때는 클라이언트가 connect 함수를 호출할 때까지 서버는 accept 함수가 호출된 위치에서 블로킹 상태에 놓이게 된다.   
   
   
   #
# Iterative 기반의 서버, 클라이언트 구현   

### Iterative 서버의 구현   

기존에 다뤘던 `hello_server.c` 서버는 한 클라이언트의 요청에만 응답을 하고 바로 종료되어버렸다.   
때문에 연결요청 대기 큐의 크기도 사실상 의미가 없었다.   
그런데 이는 우리가 생각해 오던 서버의 모습이 아니다.   
큐의 크기까지 설정해 놓았다면, 연결요청을 하는 모든 클라이언트에게 약속되어 있는 서비스를 제공해야 한다.   
그렇다면 계속해서 들어오는 클라이언트의 연결요청을 수락하기 위해서는 서버의 코드 구현을 어떤 식으로 확장해야 할까?   
단순히 생각하자! 반복문을 삽입해서 accept 함수를 반복 호출하면 된다.   
   
![iterative 서버의 함수호출 순서](https://user-images.githubusercontent.com/46395776/98382702-dd966d00-208e-11eb-81c9-ec80aafe84c7.png)   
   
위 그림의 흐름도를 보충설명 한다면, accept 함수가 호출된 다음에 입력된 함수인 read, write 함수를 호출하고 있다.   
그리고 이어서 close 함수를 호출하고 있는데, 이는 서버 소켓을 대상으로 하는 것이 아니라, accept 함수의 호출과정에서 생성된 소켓을 대상으로 하는 것이다.   
close 함수까지 호출되었다면 한 클라이언트에 대한 서비스가 완료된 것이다.   
그럼 이어서 또 다른 클라이언트에게 서비스하기 위해서 무엇을 해야겠는가?   
또 다시 accept 함수부터 호출해야 한다.   
   
위의 모델은 한 순간에 하나의 클라이언트에게만 서비스를 제공할 수 있는 모델이다.   
이후에 `프로세스`와 `쓰레드`를 다루게 되면, 동시에 둘 이상의 클라이언트에게 서비스를 제공하는 서버를 만들 수 있게 된다.   

### Iterative 에코 서버, 에코 클라이언트   

앞서 설명한 형태의 서버를 가리켜 `Iterative 서버`라 한다.   
그리고 서버가 Iterative 형태로 동작한다 해도 클라이언트 코드에는 차이가 없음을 이해할 수 있다.   
그러면 이번에는 Iterative 형태로 동작하는 에코 서버, 그리고 이와 함께 동작하는 에코 클라이언트를 작성해 본다.   
먼저 프로그램의 기본 동작 방식을 정리하면 다음과 같다.   
   
- 서버는 한 순간에 하나의 클라이언트와 연결되어 에코 서비스를 제공한다.
- 서버는 총 다섯 개의 클라이언트에게 순차적으로 서비스를 제공하고 종료한다.
- 클라이언트는 프로그램 사용자로부터 문자열 데이터를 입력 받아서 서버에 전송한다.
- 서버는 전송 받은 문자열 데이터를 클라이언트에게 재전송한다. 즉, 에코 시킨다.
- 서버와 클라이언트 간의 문자열 에코는 클라이언트가 Q 또는 q를 입력할 때까지 계속한다.   
   
위의 요구사항에 맞춰 `echo_server.c`와 `tcp-ip-socket-client 저장소`의 `echo_client.c`를 작성하였으니 코드 참고 바란다.   

### 에코 클라이언트의 문제점   

다음은 `tcp-ip-socket-client 저장소`의 `echo_client.c` 코드 중 입출력 문장이다.   
   
<pre>
<code>

send(hSocket, message, strlen(message), 0);
strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
message[strLen] = 0;
printf("Message from server: %s", message);

</code>
</pre>   
   
위의 코드는 다음과 같은 잘못된 가정이 존재한다.   
   
- read(window에선 위의 recv), write(window에선 위의 send) 함수가 호출될 때마다 문자열 단위로 실제 입출력이 이뤄진다.   
   
물론 write 함수를 호출할 때마다 하나의 문장을 전송하니, 이렇게 가정하는 것도 무리는 아니다.   
하지만 TCP는 데이터의 경계가 존재하지 않는 특성이 있다는 것을 기억하는가?   
여기서 구현한 클라이언트는 TCP 클라이언트이기 때문에 둘 이상의 write 함수 호출로 전달된 문자열 정보가 묶여서 한 번에 서버로 전송될 수 있다.   
그리고 그러한 상황이 발생하면 클라이언트는 한 번에 둘 이상의 문자열 정보를 서버로부터 되돌려 받아서, 원하는 결과를 얻지 못할 수 있다.   
그리고 서버가 다음과 같이 판단하는 상황도 생각해봐야 한다.   
   
- 문자열의 길이가 제법 긴 편이니, 문자열을 두 개의 패킷에 나눠서 보내야겠군!   
   
서버는 한 번의 write 함수 호출로 데이터 전송을 명령했지만, 전송할 데이터의 크기가 크다면, 운영체제는 내부적으로 이를 여러 개의 조각으로 나눠서 클라이언트에게 전송할 수도 있는 일이다.   
그리고 이 과정에서 데이터의 모든 조각이 클라이언트에게 전송되지 않았음에도 불구하고, 클라이언트는 read 함수를 호출할지도 모른다.   
이 모든 문제가 TCP의 데이터 전송 특성에서 비롯된 것이다.   
   
물론 우리가 구현헌 에코 서버와 에코 클라이언트는 별 무리 없이 제대로 된 서비스 결과를 보이고 있다.   
그러나 이는 단순히 운이 좋았던 것이다! 송수신하는 데이터의 크기가 작고, 실제 실행환경이 하나의 컴퓨터 또는 근거리에 놓여있는 두 개의 컴퓨터이다 보니 오류가 발생하지 않는 것일 뿐, 오류의 발생 확률은 여전히 존재한다.   
   
   
   #
# 에코 클라이언트의 완벽 구현!   

### 에코 서버는 문제가 없고, 에코 클라이언트만 문제가 있을까?   

문제는 에코 서버에 있지 않고, 에코 클라이언트에 있다.   
그런데 코드만 놓고 비교하면, 이 부분이 이해되지 않을 수 있다.   
입출력에 사용된 함수의 호출문이 동일하기 때문이다.   
먼저 에코 서버의 입출력 문장을 다시 보자.   
   
<pre>
<code>

while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
	send(hClntSock, message, strLen, 0);

</code>
</pre>   
   
이어서 에코 클라이언트의 입출력 문장을 다시 보자.   
   
<pre>
<code>

send(hSocket, message, strlen(message), 0);
strLen = recv(hSocket, message, BUF_SIZE - 1, 0);

</code>
</pre>   
   
둘 다 recv 함수와 send 함수를 반복 호출하는데 차이가 없다.   
실제로 앞서 보인 에코 클라이언트는 자신이 서버로 전송한 데이터를 100% 수신한다.   
다만 수신하는 단위에 문제가 있을 뿐이다.   
클라이언트 코드를 조금 더 살펴보자.   
   
<pre>
<code>

while (1) {
	fputs("Input message(Q to quit): ", stdout);
	fgets(message, BUF_SIZE, stdin);

	if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		break;

	send(hSocket, message, strlen(message), 0);
	strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
	message[strLen] = 0;
	printf("Message from server: %s", message);
}

</code>
</pre>   
   
이제 이해가 되겠는가?   
에코 클라이언트는 문자열을 전송한다.   
그것도 send 함수 호출을 통해서 한방에 전송한다.   
그리고 recv 함수 호출을 통해서 자신이 전송한 문자열 데이터를 한방에 수신하기를 원하고 있다. 바로 이것이 문제이다!   
   
결국엔 서버로부터 에코 클라이언트에게 문자열 데이터가 전부 전송되니까 기다리면 될 수 있다.   
시간이 좀 지나서 recv 함수를 호출하면 한방에 문자열 데이터를 수신할 수는 있다.   
하지만 얼마나 기다려야 되겠는가? 얼마나 기다려야 서버로 보냈던 문자열이 모두 자신에게 전송되어 있는지 알 수 없다.   
이치에 맞는 클라이언트라면, 문자열 데이터가 전송되었을 때, 이를 모두 읽어서 출력해야 한다.   

### 에코 클라이언트의 해결책   

위의 문제는 해결이 쉽다.   
클라이언트가 수신해야 할 데이터의 크기를 미리 알고 있기 때문이다.   
예를 들어서 크기가 20바이트인 문자열을 전송했다면, 20바이트를 수신할 때까지 반복해서 read(위의 윈도우에서는 recv) 함수를 호출하면 된다.   
`tcp-ip-socket-client 저장소`의 `echo_client2.c`를 참고하면 된다.   
   
   
   #
# TCP의 이론적인 이야기   

### TCP 소켓에 존재하는 입출력 버퍼   

TCP 소켓의 데이터 송수신에는 경계가 없다는 특성이 있다.   
따라서 서버가 한 번의 write 함수 호출을 통해서 40바이트를 전송해도 클라이언트는 네 번의 read 함수 호출을 통해서 10바이트씩 데이터를 수신하는 것이 가능하다.   
그런데 이러한 현상에 의문이 생긴다.   
서버는 데이터를 한 번에 40바이트를 전송했는데, 클라이언트가 이를 여유 있게 조금씩 수신하니 말이다.   
클라이언트가 10바이트만 먼저 수신했다면, 서버가 보낸 나머지 30바이트는 어디서 대기하고 있는 것일까?   
   
사실 write 함수가 호출되는 순간이 데이터가 전송되는 순간이 아니고, read 함수가 호출되는 순간이 데이터가 수신되는 순간이 아니다.   
정확히 말하면 write 함수가 호출되는 순간 데이터는 `출력버퍼`로 이동을 하고, read 함수가 호출되는 순간 `입력버퍼`에 저장된 데이터를 읽어 들이게 된다.   
   
![tcp 소켓의 입출력 버퍼](https://user-images.githubusercontent.com/46395776/98434658-14af6180-2115-11eb-82a3-249de5cb2a69.png)   
   
위 그림이 보이듯이 write 함수가 호출되면 출력버퍼라는 곳에 데이터가 전달되어서 상황에 맞게 적절히(한 번에 보내든 나눠서 보내든) 데이터를 상대방의 입력버퍼로 전송한다.   
그러면 상대방은 read 함수 호출을 통해서 입력버퍼에 저장된 데이터를 읽게 되는 것이다.   
이러한 입출력 버퍼의 특성 몇 가지를 정리하면 다음과 같다.   
   
- 입출력 버퍼는 TCP 소켓 각각에 대해 별도로 존재한다.
- 입출력 버퍼는 소켓 생성시 자동으로 생성된다.
- 소켓을 닫아도 출력버퍼에 남아있는 데이터는 계속해서 전송이 이뤄진다.
- 소켓을 닫으면 입력버퍼에 남아있는 데이터는 소멸되어버린다.   
   
그렇다면 다음과 같은 상황이 발생하면 어떤 일이 일어날까?   
입출력 버퍼의 존재를 알았으니, 다음과 같은 상황에서의 흐름을 유추해볼 수 있다.   
   
- 클라이언트의 입력버퍼 크기가 50바이트인데, 서버에서 100바이트를 전송하였다.   
   
이는 문제가 아닐 수 없다. 입력버퍼의 크기가 50바이트인데, 100바이트가 전송되니 말이다.   
하지만 입력버퍼의 크기를 초과하는 분량의 데이터 전송은 발생하지 않는다!   
왜냐하면 TCP가 `데이터의 흐름`까지 컨트롤하기 때문이다.   
TCP에는 `슬라이딩 윈도우(Sliding Window)`라는 프로토콜이 존재한다.   
이 프로토콜로 인해 각 소켓의 `입력버퍼`는 여유 공간이 얼마인지 상대 소켓의 `출력버퍼`에 알리며 데이터 전송을 이뤄낸다.   
이렇듯 서로 대화를 주고받으면서 데이터를 송수신하기 때문에, 버퍼가 차고 넘쳐서 데이터가 소멸되는 일이 TCP에서는 발생하지 않는다.   

### TCP의 내부 동작원리1 : 상대 소켓과의 연결   

TCP 소켓의 생성에서 소멸의 과정까지 거치게 되는 일을 크게 나누면, 다음 세 가지로 구분할 수 있다.   
   
- 상대 소켓과의 연결
- 상대 소켓과의 데이터 송수신
- 상대 소켓과의 연결종료   
   
TCP 소켓은 연결설정 과정에서 총 세 번의 대화를 주고 받는다.   
그래서 이를 가리켜 `Three-way handshaking`이라 한다.   
   
![3 way handshaking](https://user-images.githubusercontent.com/46395776/98435559-54774880-2117-11eb-8b4b-abc06fb2a3f0.png)   
   
소켓은 전 이중(Full-duplex) 방식으로 동작하므로 양방향으로 데이터를 주고받을 수 있다.   
따라서 데이터 송수신에 앞서 준비과정이 필요하다.   
먼저 위의 그림에서 연결요청을 하는 Client가 Server에게 다음 메시지를 전달하고 있다.   
   
- [SYN] seq: 100, ack: -   
   
이는 seq가 100, ack는 비어있음을 뜻하는데, 여기서 seq 100이 의미하는 바는 다음과 같다.   
   
- 내가 지금 보내는 이 패킷에 100이라는 번호를 부여하니, 잘 받았다면 다음에는 101번 패킷을 전달하라고 내게 말해달라!   
   
이는 처음 연결요청에 사용되는 메시지이기 때문에 이 메시지를 가리켜 SYN이라 한다.   
그리고 SYN은 Synchronization의 줄임 말로써, 데이터 송수신에 앞서 전송되는 `동기화 메시지`라는 의미를 담고 있다.   
이어서 호스트 B가 호스트 A에게 다음 메시지를 전달하고 있다.   
   
- [SYN + ACK] seq: 200, ack: 101   
   
이는 seq가 200, ack가 101임을 뜻하는데, 여기서 seq 200이 의미하는 바는 다음과 같다.   
   
- 내가 지금 보내는 이 패킷에 200이라는 번호를 부여하니, 잘 받았다면 다음에는 201번 패킷을 전달하라고 내게 말해달라!   
   
그리고 ack 101이 의미하는 바는 다음과 같다.   
   
- 좀 전에 전송한 seq가 100인 패킷은 잘 받았으니, 다음 번에는 seq가 101인 패킷을 전송하기 바란다!   
   
즉, 처음 호스트 A가 전송한 패킷에 대한 `응답 메시지(ack 101)`와 함께 호스트 B의 데이터 전송을 위한 `동기화 메시지(seq 200)`를 함께 묶어서 보내고 있다.   
그래서 이러한 유형의 메시지를 가리켜 `SYN + ACK`라 한다.   
   
이렇듯 데이터의 송수신에 앞서, 송수신에 사용되는 패킷에 번호를 부여하고, 이 번호 정보를 상대방에게 알리는 이유는 데이터의 손실을 막기 위함이다.   
이렇게 패킷에 번호를 부여해서 확인하는 절차를 거치기 때문에 손실된 데이터의 확인 및 재전송이 가능한 것이고, 때문에 TCP는 손실 없는 데이터의 전송을 보장하는 것이다.   
그럼 마지막으로 Client가 Server에게 전송한 메시지를 살펴보자.   
   
- [ACK] seq: 101, ack: 201   
   
이미 앞서 한차례씩 송수신한 패킷에서 보였듯이 TCP의 연결과정에서 패킷을 보낼 때에는 항상 번호를 부여한다.   
그래서 seq 101이 부여되었다.   
앞서 보낸 패킷의 seq가 100이었으니, 이번에는 이보다 1이 증가한 101이 부여된 것이다.   
그리고 이 패킷은 다음의 메시지 전달을 목적으로 전송되었다.   
   
- 좀 전에 전송한 seq가 200인 패킷은 잘 받았으니, 다음 번에는 201인 패킷을 전송하기 바란다!   
   
때문에, ack 201이 추가된 형태의 ack 메시지가 전송되었다.   
이로써 Client(호스트 A), Server(호스트 B) 상호간에 데이터 송수신을 위한 준비가 모두 되었음을 서로 인식하게 되었다.   

### TCP의 내부 동작원리2 : 상대 소켓과의 데이터 송수신   

처음 진행한 Three-way handshaking을 통해서 데이터의 송수신 준비가 끝났으니, 이제 본격적으로 데이터를 송수신할 차례가 되었다.   
데이터 송수신의 기본 방식은 다음과 같다.   
   
![tcp 소켓의 데이터 송수신 과정](https://user-images.githubusercontent.com/46395776/98436523-5b09be00-211f-11eb-8cc4-6cb49c3fa303.png)   
   
위 그림은 호스트 A가 호스트 B에게 총 200바이트를 두 번에 나눠서(두 개의 패킷에 나눠서) 전송하는 과정을 보인 것이다.   
먼저 호스트 A가 100바이트의 데이터를 하나의 패킷에 실어 전송하였는데, 패킷의 SEQ를 1200으로 부여하고 있다.   
때문에 호스트 B는 이를 근거로 패킷이 제대로 수신되었음을 알려야 하기에, ACK 1301 메시지를 담은 패킷을 호스트 A에 전송하고 있다.   
이 때, ack 번호가 1201이 아닌 1301인 이유는 ACK 번호를 전송된 바이트 크기만큼 추가로 증가시켰기 때문이다.   
   
이렇듯 ACK 번호를 전송된 바이트 크기만큼 추가로 증가시키지 않으면, 패킷의 전송은 확인할 수 있을지 몰라도, 패킷에 담긴 100바이트가 전부 전송되었는지, 아니면 그 중 일부가 손실되고 80바이트만 전송되었는지 알 방법이 없다.   
그래서 다음의 공식을 기준으로 ACK 메시지를 전송한다.   
   
- ACK 번호 -> SEQ 번호 + 전송된 바이트 크기 + 1   
   
마지막에 1을 더한 이유는 Three-way handshaking에서도 보였듯이, 다음 번에 전달된 SEQ의 번호를 알리기 위함이다.   
다음은 중간에 패킷이 소멸되는 상황이다.   
   
![tcp 소켓의 데이터 송신 오류](https://user-images.githubusercontent.com/46395776/98436615-1b8fa180-2120-11eb-84e2-ce33e729246b.png)   
   
위 그림은 SEQ 1301인 패킷에 100바이트 데이터를 실어서 호스트 B로 전송되고 있음을 보이고 있다.   
그런데 중간에 문제가 발생해서 호스트 B에 전송되지 못했다.   
이러한 경우 호스트 A는 일정시간이 지나도 SEQ 1301에 대한 ACK 메시지를 받지 못하기 때문에 재전송을 진행한다.   
이렇듯 데이터의 손실에 대한 재전송을 위해서, TCP 소켓은 ACK 응답을 요구하는 패킷 전송 시에 `타이머`를 동작시킨다.   
그리고 해당 타이머가 `Time-out!`되었을 때, 패킷을 재전송한다.   

### TCP의 내부 동작원리3 : 상대 소켓과의 연결종료   

![tcp 소켓의 연결종료 과정](https://user-images.githubusercontent.com/46395776/98436695-ba1c0280-2120-11eb-83db-0f50bd5b75d4.png)   
   
위 그림에서 패킷 안에 삽입되어 있는 FIN은 종료를 알리는 메시지를 뜻한다.   
즉, 상호간에 FIN 메시지를 한 번씩 주고 받고서 연결이 종료되는데, 이 과정이 네 단계에 걸쳐서 진행되기 때문에 이를 가리켜 `Four-way handshaking`이라고 부른다.   
그리고 SEQ와 ACK의 의미는 앞서 설명한 내용과 같다.   
다만 ACK 5001이 호스트 A에 두 번 전달된 것이 낯설 것이다.   
그러나 FIN 메시지에 포함된 ACK 5001은 앞서 전송한 ACK 메시지가 수신된 이후로 데이터 수신이 없었기 때문에 재전송된 것이다.   
   
   
   #
# UDP에 대한 이해   

### UDP 소켓의 특성   

편지를 예로 들면서 UDP의 동작 원리를 설명한다.   
이는 UDP의 설명에 사용되는 전통적인 예로써 완벽히 UDP와 맞아 떨어지는 특징이 있다.   
편지를 보내기 위해서는 일단 편지봉투에다가 보내는 사람과 받는 사람의 주소정보를 써 넣어야 한다.   
그리고 우표를 붙여서 우체통에 넣어주면 끝이다.   
다만 편지의 특성상 보내고 나서 상대방의 수신여부를 확인할 길은 없다.   
물론 전송 도중에 편지가 분실될 확률도 없지 않다.   
즉, 편지는 신뢰할 수 없는 전송 방법이다.   
이와 마찬가지로 UDP 소켓은 신뢰할 수 없는 전송 방법을 제공한다.   
   
이렇듯 `신뢰성`만 놓고 보면 분명 TCP가 UDP보다 좋은 프로토콜이다.   
하지만 UDP는 TCP보다 훨씬 간결한 구조로 설계되어있다.   
ACK와 같은 응답 메시지를 보내는 일도 없으며, SEQ와 같이 패킷에 번호를 부여하는 일도 없다.   
때문에 상황에 따라서 TCP보다 훨씬 좋은 성능을 발휘한다.   
물론 프로그래밍의 관점에서 보더라도 UDP는 TCP보다 구현이 용이하다.   
게다가 UDP도 TCP만큼은 아니지만 생각만큼 데이터의 손실이 자주 발생하는 것은 아니기 때문에, 신뢰성보다 성능이 중요시되는 상황에서는 UDP가 좋은 선택이 될 수 있다.   
   
그렇다면 UDP의 역할은 어디까지일까?   
앞서 TCP는 신뢰성 없는 IP를 기반으로 신뢰성 있는 데이터의 송수신을 위해서 `흐름제어(Flow Control)`를 한다고 설명했는데, 바로 이 흐름제어가 UDP에는 존재하지 않는다.   
즉, 흐름제어가 UDP와 TCP를 구분 지어주는 가장 큰 차이점이다.   

### UDP의 내부 동작원리   

UDP는 TCP와 달리 흐름제어를 하지 않는다.   
UDP의 역할이 어디까지인지 구체적으로 설명한다. 이를 위해 다음 그림을 보자.   
   
![패킷 전송에 있어서의 UDP와 IP 역할](https://user-images.githubusercontent.com/46395776/98437915-9f9a5700-2129-11eb-86e4-321547afe2c1.png)   
   
위 그림에서 보이듯이 호스트 B를 떠난 UDP 패킷이 호스트 A에게 전달되도록 하는 것은 IP의 역할이다.   
그런데 이렇게 전달된 UDP 패킷을 호스트 A 내에 존재하는 UDP 소켓 중 하나에게 최종 전달하는 것은 IP의 역할이 아니다. 바로 UDP의 역할이다.   
즉, UDP의 역할 중 가장 중요한 것은 호스트로 수신된 패킷을 PORT 정보를 참조하여 최종 목적지인 UDP 소켓에 전달하는 것이다.   

### UDP의 효율적 사용   

네트워크 프로그래밍의 대부분이 TCP를 기반으로 구현될 것 같지만, UDP를 기반으로 구현되는 경우도 흔히 볼 수 있다.   
그러면 언제 UDP를 사용하는 것이 효율적인지 생각해보자.   
그런데 이에 앞서 UDP도 나름대로 상당히 신뢰할만하다는 것을 언급한다.   
인터넷의 특성상 손실되는 정보들이 많음에도 불구하고 생각보다는 신뢰할만하다.   
그래도 1만개의 패킷을 보냈는데, 그 중 1개만 손실 되어도 문제가 발생하는 `압축파일`의 경우에는 반드시 TCP를 기반으로 송수신이 이뤄져야 한다.   
왜냐하면 압축파일은 그 특성상 파일의 일부만 손실되어도 압축의 해제가 어렵기 때문이다.   
그러나 인터넷 기반으로 실시간 영상 및 음성을 전송하는 경우에는 얘기가 조금 다르다.   
멀티미디어 데이터는 그 특성상 일부가 손실되어도 크게 문제되지 않는다.   
잠깐의 화면 떨림, 또는 아주 작은 잡음 정도는 그냥 넘어갈만하다.   
하지만 실시간으로 서비스를 해야 하므로 속도가 상당히 중요한 요소가 된다.   
때문에 TCP가 하는 흐름제어의 과정은 귀찮게 느껴지는 것이다.   
이러한 경우가 UDP 기반의 구현을 고려할만한 상황이다.   
그러나 UDP가 TCP에 비해서 언제나 빠른 속도를 내는 것은 아니다.   
TCP가 UDP에 비해 느린 이유 두 가지만 달라고 하면, 다음 두 가지를 들 수 있다.   
   
- 데이터 송수신 이전, 이후에 거치는 연결설정 및 해제과정
- 데이터 송수신 과정에서 거치는 신뢰성보장을 위한 흐름제어   
   
따라서 송수신하는 데이터의 양은 작으면서 잦은 연결이 필요한 경우에는 UDP가 TCP보다 훨씬 효율적이고 빠르게 동작한다.   
   
   
   #
# UDP 기반 서버 / 클라이언트의 구현   

### UDP에서의 서버와 클라이언트는 연결되어 있지 않다.   

UDP 서버, 클라이언트는 TCP와 같이 연결된 상태로 데이터를 송수신하지 않는다.   
때문에 TCP와 달리 연결 설정의 과정이 필요 없다.   
따라서 TCP 서버 구현과정에서 거쳤던 listen 함수와 accept 함수의 호출은 불필요하다.   
UDP 소켓의 생성과 데이터의 송수신 과정만 존재할 뿐이다.   

### UDP에서는 서버건 클라이언트건 하나의 소켓만 있으면 된다.   

TCP는 1대 1의 연결을 필요로 하지만, UDP는 연결의 개념이 존재하지 않는다.   
따라서 UDP는 서버 소켓과 클라이언트 소켓의 구분이 없다.   
TCP는 서버에서 열 개의 클라이언트에게 서비스를 제공하려면 문지기 역할을 하는 서버 소켓을 제외하고도 열 개의 소켓이 더 필요했다.   
그러나 UDP는 연결의 개념이 존재하지 않으므로 하나의 소켓으로 둘 이상의 영역과 데이터 송수신이 가능하다.   

### UDP 기반의 데이터 입출력 함수   

TCP 소켓을 생성하고 나서 데이터를 전송하는 경우에는, 주소 정보를 따로 추가하는 과정이 필요 없다.   
왜냐하면 TCP 소켓은 목적지에 해당하는 소켓과 연결된 상태이기 때문이다.   
즉, TCP 소켓은 목적지의 주소 정보를 이미 알고 있는 상태이다.   
   
그러나 UDP 소켓은 연결 상태를 유지하지 않으므로(UDP 소켓은 단순히 우체통의 역할만 하므로), 데이터를 전송할 때마다 반드시 목적지의 주소 정보를 별도로 추가해야 한다.   
이는 우체통에 넣을 우편물에 주소 정보를 써 넣는 것에 비유할 수 있다.   
다음은 주소 정보를 써 넣으면서 데이터를 전송할 때 호출하는 UDP 관련 함수들이다.   
   
`ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* to, socklen_t addrlen);`   

- sock : 데이터 전송에 사용될 UDP 소켓의 파일 디스크립터를 인자로 전달.
- buff : 전송할 데이터를 저장하고 있는 버퍼의 주소 값 전달.
- nbytes : 전송할 데이터 크기를 바이트 단위로 전달.
- flags : 옵션 지정에 사용되는 매개변수. 지정할 옵션이 없다면 0 전달.
- to : 목적지 주소 정보를 담고 있는 sockaddr 구조체 변수의 주소 값 전달.
- addrlen : 매개변수 to로 전달된 주소 값의 구조체 변수 크기 전달.
   
위 함수가 이전에 소개한 TCP 기반의 출력함수와 가장 비교되는 것은 목적지 주소 정보를 요구하고 있다는 점이다.   
그러면 이어서 UDP 데이터 수신에 사용되는 함수를 소개한다.   
UDP 데이터는 발신지가 일정치 않기 때문에 발신지 정보를 얻을 수 있도록 함수가 정의되어 있다.   
즉, 이 함수는 UDP 패킷에 담겨 있는 발신지 정보를 함께 반환한다.   
   
`ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* from, socklen_t* addrlen);`   

- sock : 데이터 수신에 사용될 UDP 소켓의 파일 디스크립터를 인자로 전달.
- buff : 데이터 수신에 사용될 버퍼의 주소 값 전달.
- nbytes : 수신할 최대 바이트 수 전달. 때문에 매개변수 buff가 가리키는 버퍼의 크기를 넘을 수 없다.
- flags : 옵션 지정에 사용되는 매개변수. 지정할 옵션이 없다면 0 전달.
- from : 발신지 정보를 채워 넣을 sockaddr 구조체 변수의 주소 값 전달.
- addrlen : 매개변수 from으로 전달된 주소에 해당하는 구조체 변수의 크기 정보를 담고 있는 변수의 주소값 전달.   
   
### UDP 클라이언트 소켓의 주소정보 할당   

지금까지 위에서 UDP 기반의 서버, 클라이언트 프로그램 구현에 대해 설명했다.   
그런데, UDP 클라이언트 프로그램을 가만히 보면(tcp-ip-client 저장소의 uecho_client.c 참고!), IP와 PORT를 소켓에 할당하는 부분이 눈에 띄지 않는다.   
TCP 클라이언트의 경우에는 connect 함수 호출 시 자동으로 할당되었는데, UDP 클라이언트의 경우에는 그러한 기능을 대신 할만한 함수 호출문조차 보이지 않는다.   
도대체 어느 시점에 IP와 PORT가 할당되는 것일까?   
   
UDP 프로그램에서는 데이터를 전송하는 sendto 함수호출 이전에 해당 소켓에 주소 정보가 할당되어 있어야 한다.   
따라서 sendto 함수 호출 이전에 bind 함수를 호출해서 주소 정보를 할당해야 한다.   
물론 bind 함수는 TCP 프로그램의 구현에서 호출되었던 함수이다.   
그러나 이 함수는 TCP와 UDP를 가리지 않으므로 UDP 프로그램에서도 호출 가능하다.   
그리고 만약에 sendto 함수 호출 시까지 주소 정보가 할당되지 않았다면, sendto 함수가 처음 호출되는 시점에 해당 소켓에 IP와 PORT 번호가 자동으로 할당된다.   
또한, 이렇게 한 번 할당되면 프로그램이 종료될 때까지 주소 정보가 그대로 유지되기 때문에 다른 UDP 소켓과 데이터를 주고받을 수 있다.   
물론 IP는 호스트의 IP로, PORT는 사용하지 않는 PORT 번호 하나를 임의로 골라서 할당하게 된다.   
   
이렇듯 sendto 함수 호출 시 IP와 PORT 번호가 자동으로 할당되기 때문에 일반적으로 UDP의 클라이언트 프로그램에서는 주소 정보를 할당하는 별도의 과정이 불필요하다.   
그래서 tcp-ip-client 저장소의 uecho_client.c에서도 별도의 주소 정보 할당 과정을 생략했으며, 이것이 일반적인 구현 방법이다.   

### connected UDP 소켓, unconnected UDP 소켓   

TCP 소켓에는 데이터를 전송할 목적지의 IP와 PORT 번호를 등록하는 반면, UDP 소켓에는 데이터를 전송할 목적지의 IP와 PORT 번호를 등록하지 않는다.   
때문에, sendto 함수 호출을 통한 데이터의 전송 과정은 다음과 같이 크게 세 단계로 나눌 수 있다.   
   
- 1단계 : UDP 소켓에 목적지의 IP와 PORT 번호 등록
- 2단계 : 데이터 전송
- 3단계 : UDP 소켓에 등록된 목적지 정보 삭제   
   
즉, sendto 함수가 호출될 때마다 위의 과정을 반복하게 된다.   
이렇듯 목적지의 주소 정보가 계속해서 변경되기 때문에 하나의 UDP 소켓을 이용해서 다양한 목적지로 데이터 전송이 가능한 것이다.   
그리고 이렇게 목적지 정보가 등록되어 있지 않은 소켓을 가리켜 `unconnected 소켓`이라 하고, 반면 목적지 정보가 등록되어 있는 소켓을 가리켜 `connected 소켓`이라 한다.   
물론 UDP 소켓은 기본적으로 unconnected 소켓이다.   
그런데 이러한 UDP 소켓은 다음과 같은 상황에서는 매우 불합리하게 동작한다.   
   
- IP 211.210.147.82, PORT 82번으로 준비된 총 세 개의 데이터를 세 번의 sendto 함수 호출을 통해서 전송한다.   
   
이 경우 위에서 정리한 데이터 전송 세 단계를 총 3회 반복해야 한다.   
그래서 하나의 호스트와 오랜 시간 데이터를 송수신해야 한다면, UDP 소켓을 connected 소켓으로 만드는 것이 효율적이다.   
위의 1단계와 3단계가 UDP 데이터 전송 과정의 약 1/3에 해당한다고 하니, 이 시간을 줄임으로 적지 않은 성능향상을 기대할 수 있다.   

### connected UDP 소켓 생성   

connected UDP 소켓을 생성하는 방법은 의외로 간단하다.   
UDP 소켓을 대상으로 connect 함수만 호출해주면 된다.   
   
<pre>
<code>

sock = socket(PF_INET, SOCK_DRAM, 0);
memset(&adr, 0, sizeof(adr));
adr.sin_family = AF_INET;
adr.sin_addr.s_addr = ....
adr.sin_port = ....
connect(sock, (struct sockaddr*)&adr, sizeof(adr));

</code>
</pre>   
   
위의 코드를 언뜻 보면 TCP 소켓 생성 과정의 일부처럼 보인다.   
하지만 socket 함수의 두 번째 인자가 SOCK_DGRAM이지 않은가?   
이는 분명 UDP 소켓의 생성 과정이다.   
물론 UDP 소켓을 대상으로 connect 함수를 호출했다고 해서 목적지의 UDP 소켓과 연결 설정 과정을 거친다거나 하지는 않는다.   
다만 UDP 소켓에 목적지의 IP와 PORT 정보가 등록될 뿐이다.   
이로써 이후부터는 TCP 소켓과 마찬가지로 sendto 함수가 호출될 때마다 데이터 전송의 과정만 거치게 된다.   
뿐만 아니라 송수신의 대상이 정해졌기 때문에 sendto, recvfrom 함수가 아닌 write, read 함수의 호출로도 데이터를 송수신할 수 있다.   

### UDP 데이터그램(Datagram)   

UDP 소켓이 전송하는 패킷을 가리켜 데이터그램이라고 표현하기도 하는데, 사실 데이터그램도 패킷의 일종이다.   
다만 TCP 패킷과 달리 데이터의 일부가 아닌, 그 자체가 하나의 데이터로 의미를 가질 때 데이터그램이라 표현할 뿐이다.   
이는 UDP의 데이터 전송 특성과 관계가 있다.   
UDP는 데이터의 경계가 존재하기 때문에 하나의 패킷이 하나의 데이터로 간주된다.   
따라서 데이터그램이라고 표현하는 것이다.