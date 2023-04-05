# OS_Pthreads-Synchronization
숭실대학교 운영체제 2020 가을학기 - 과제4 : Pthread  
  
본 과제의 목표는 다중 쓰레드 프로그래밍의 기본 개념을 이해하고, lock pthread 사용자 수준 쓰레드 라이브러리를 활용하여 조건 변수를 사용하는 쓰레드 프로그램을 구현하는 것입니다.  

**쓰레드1 : 마스터 & 워커 쓰레드 풀**  
마스터가 버퍼에 쓰기 작업을 계속하다가 버퍼가 꽉 찬 경우, 기다리고 있던 워커가 읽기 작업을 진행하면서 버퍼를 다시 빈 상태로 만들고,  
이를 기다리고 있던 마스터가 다시 쓰기 작업을 하며 또 버퍼가 꽉 찰 때 다시 워커가 일하도록 합니다.  

**쓰레드2 : 독자 & 저자 락 쓰레드 프로그램**
독자 쓰레드가 lock을 얻기 위해 기다려서는 안되며, 저자 쓰레드가 락을 얻기 위해 기다릴 때는 이전 독자 쓰레드와 동시에 락을 획득 가능하게 해야합니다.

**쓰레드3 : pthread를 이용한 사용자 수준 세마포어**  
pthread를 이용하여 세마포어를 구현하기 위해 SSU_Sem이라는 구조체를 정의 합니다.이 안에는 counter 정수형 변수가 있습니다.

Soongsil University. Operating System 2020-2 - Assignment#4 : Pthread  
  
basics of multi-threaded programming, and synchronizing multiple threads using locks and condition variables.  
  
**Thread1 : Master & Worker Thread Pool**  
master produce numbers and place them in a buffer, and worker threads consume them.  
  
**Thread2 : Reader-Writer Locks**  
an application where multiple threads of a process wish to read and write data shared between them.  
  
**Thread3 : Semaphores**  
implementation of the synchronization functionality of semaphores using pthreads mutexes and condition variables.  
